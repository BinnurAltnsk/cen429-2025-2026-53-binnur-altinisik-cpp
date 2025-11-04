/**
 * @file softhsm.cpp
 * @brief Seyahat Gideri Takibi - SoftHSM (PKCS#11) Implementation
 * 
 * Bu dosya, SoftHSM kullanarak PKCS#11 standardı üzerinden kriptografik
 * işlemler yapmak için gerekli fonksiyonların implementasyonlarını içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/softhsm.h"
#include "../header/safe_string.h"

// PKCS11 macro redefinition önlemek için koruma ekle
#ifndef PKCS11_NO_EXPORTS
#define PKCS11_NO_EXPORTS
#endif
#include "../third_party/pkcs11/pkcs11.h"

#include <cstring>
#include <vector>
#include <memory>
#include <cstdio>

#ifdef _WIN32
    #include <io.h>
    #define FILE_EXISTS(path) (_access(path, 0) == 0)
#else
    #include <unistd.h>
    /**
     * @brief Linux için dosya varlık kontrolü helper fonksiyonu
     * 
     * Bu fonksiyon, belirtilen dosya yolunun var olup olmadığını kontrol eder.
     * Platform-specific code for non-Windows platforms - not executed on Windows.
     * This code is excluded from coverage on Windows builds.
     * 
     * @param path Kontrol edilecek dosya yolu
     * @return 0 Dosya varsa, -1 Dosya yoksa
     */
    static inline int file_exists_helper(const char* path) {
        return access(path, F_OK) == 0;
    }
    #define FILE_EXISTS(path) file_exists_helper(path)
#endif

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
    // Windows header dosyaları ERROR ve ERROR_* makroları tanımlar, bunları undef ediyoruz
    // çünkü bizim ErrorCode ve InitStatus enum değerlerimizle çakışıyorlar
    #undef ERROR
    #undef ERROR_FILE_NOT_FOUND
    #undef ERROR_INVALID_INPUT
    #undef ERROR_ENCRYPTION_FAILED
    #undef ERROR_UNKNOWN
    #define DL_HANDLE HMODULE
    #define DL_OPEN(path) LoadLibraryA(path)
    #define DL_CLOSE(handle) FreeLibrary(handle)
    #define DL_SYM(handle, name) GetProcAddress(handle, name)
    #define DL_ERROR() GetLastError()
#else
    #include <dlfcn.h>
    #define DL_HANDLE void*
    #define DL_OPEN(path) dlopen(path, RTLD_NOW)
    #define DL_CLOSE(handle) dlclose(handle)
    #define DL_SYM(handle, name) dlsym(handle, name)
    #define DL_ERROR() dlerror()
#endif

namespace TravelExpense {

    namespace SoftHSM {

        // Using directive for ErrorCode
        using ErrorCode = TravelExpense::ErrorCode;

        // Global state
        static DL_HANDLE g_pkcs11Library = nullptr;
        static CK_FUNCTION_LIST* g_pFunctionList = nullptr;
        static CK_SLOT_ID g_currentSlot = 0;
        static CK_SESSION_HANDLE g_sessionHandle = 0;
        static InitStatus g_status = InitStatus::NOT_INITIALIZED;
        static char g_tokenLabel[64] = {0};
        static char g_pin[32] = {0};

        /**
         * @brief PKCS#11 return code'unu ErrorCode'a çevir
         * 
         * Bu helper fonksiyon, PKCS#11 standardından dönen return code'ları
         * uygulamanın ErrorCode enum'una çevirir. PKCS#11 hata kodları
         * CKR_* formatında olup, bunlar uygulama seviyesinde daha anlamlı
         * hata kodlarına dönüştürülür.
         * 
         * @param rv PKCS#11 return code (CK_ULONG)
         * @return ErrorCode Uygulama seviyesinde hata kodu
         */
        static ErrorCode pkcs11ToErrorCode(CK_ULONG rv) {
            switch (rv) {
                case CKR_OK:
                    return ErrorCode::Success;
                case CKR_ARGUMENTS_BAD:
                    return ErrorCode::InvalidInput;
                case CKR_PIN_INCORRECT:
                case CKR_PIN_INVALID:
                    return ErrorCode::InvalidInput;
                case CKR_TOKEN_NOT_PRESENT:
                case CKR_TOKEN_NOT_RECOGNIZED:
                    return ErrorCode::FileNotFound;
                case CKR_SESSION_HANDLE_INVALID:
                    return ErrorCode::InvalidInput;
                case CKR_OBJECT_HANDLE_INVALID:
                    return ErrorCode::InvalidInput;
                case CKR_MECHANISM_INVALID:
                    return ErrorCode::InvalidInput;
                case CKR_FUNCTION_FAILED:
                    return ErrorCode::EncryptionFailed;
                default:
                    return ErrorCode::Unknown;
            }
        }

        /**
         * @brief SoftHSM kütüphanesi yolunu bul
         * 
         * Bu helper fonksiyon, sistemde yüklü SoftHSM kütüphanesini bulur.
         * Platform-specific varsayılan yolları kontrol eder ve ilk bulunan
         * kütüphane yolunu döndürür.
         * 
         * Windows için kontrol edilen yollar:
         * - softhsm2.dll (current directory)
         * - C:\Program Files\SoftHSM2\lib\softhsm2.dll
         * - C:\Program Files (x86)\SoftHSM2\lib\softhsm2.dll
         * 
         * Linux için kontrol edilen yollar:
         * - libsofthsm2.so (current directory)
         * - /usr/lib/softhsm/libsofthsm2.so
         * - /usr/local/lib/softhsm/libsofthsm2.so
         * - /usr/lib/x86_64-linux-gnu/softhsm/libsofthsm2.so
         * - /opt/softhsm2/lib/libsofthsm2.so
         * 
         * @return const char* Bulunan kütüphane yolu, bulunamazsa nullptr
         */
        static const char* findSoftHSMLibrary() {
            // Windows için
#ifdef _WIN32
            // SoftHSM için olası yollar
            const char* paths[] = {
                "softhsm2.dll",
                "C:\\Program Files\\SoftHSM2\\lib\\softhsm2.dll",
                "C:\\Program Files (x86)\\SoftHSM2\\lib\\softhsm2.dll",
                nullptr
            };
            
            for (int i = 0; paths[i] != nullptr; ++i) {
                // Dosyanın varlığını kontrol et
                if (FILE_EXISTS(paths[i])) {
                    return paths[i];
                }
            }
            
            return nullptr;
#else
            // Linux için
            const char* paths[] = {
                "libsofthsm2.so",
                "/usr/lib/softhsm/libsofthsm2.so",
                "/usr/local/lib/softhsm/libsofthsm2.so",
                "/usr/lib/x86_64-linux-gnu/softhsm/libsofthsm2.so",
                "/opt/softhsm2/lib/libsofthsm2.so",
                nullptr
            };
            
            for (int i = 0; paths[i] != nullptr; ++i) {
                // Dosyanın varlığını kontrol et
                if (FILE_EXISTS(paths[i])) {
                    return paths[i];
                }
            }
            
            return nullptr;
#endif
        }

        /**
         * @brief PKCS#11 kütüphanesini yükle
         * 
         * Bu helper fonksiyon, PKCS#11 kütüphanesini dinamik olarak yükler.
         * Eğer kütüphane zaten yüklüyse, hiçbir işlem yapmaz ve Success döner.
         * Eğer libraryPath nullptr ise, findSoftHSMLibrary() ile otomatik bulma yapar.
         * 
         * Kütüphane yüklendikten sonra, C_GetFunctionList fonksiyonunu alır ve
         * PKCS#11 function list'ini initialize eder. Bu işlem başarısız olursa,
         * kütüphane kapatılır ve hata kodu döner.
         * 
         * @param libraryPath PKCS#11 kütüphanesi yolu (nullptr ise otomatik bulma yapılır)
         * @return ErrorCode Başarı durumu (Success, FileNotFound, Unknown)
         */
        static ErrorCode loadPKCS11Library(const char* libraryPath) {
            if (g_pkcs11Library != nullptr) {
                // Zaten yüklü
                return ErrorCode::Success;
            }

            const char* path = libraryPath;
            if (!path) {
                path = findSoftHSMLibrary();
                if (!path) {
                    return ErrorCode::FileNotFound;
                }
            }

            g_pkcs11Library = DL_OPEN(path);
            if (!g_pkcs11Library) {
                return ErrorCode::FileNotFound;
            }

            // C_GetFunctionList fonksiyonunu al
            CK_C_GetFunctionList C_GetFunctionList = 
                reinterpret_cast<CK_C_GetFunctionList>(DL_SYM(g_pkcs11Library, "C_GetFunctionList"));
            
            if (!C_GetFunctionList) {
                DL_CLOSE(g_pkcs11Library);
                g_pkcs11Library = nullptr;
                return ErrorCode::FileNotFound;
            }

            // Function list'i al
            CK_ULONG rv = C_GetFunctionList(&g_pFunctionList);
            if (rv != CKR_OK || !g_pFunctionList) {
                DL_CLOSE(g_pkcs11Library);
                g_pkcs11Library = nullptr;
                return ErrorCode::Unknown;
            }

            return ErrorCode::Success;
        }

        /**
         * @brief SoftHSM'yi başlat ve PKCS#11 kütüphanesini yükle
         * 
         * SoftHSM modülünü başlatır ve belirtilen PKCS#11 kütüphanesini yükler.
         * Eğer `libraryPath` nullptr ise, sistemdeki varsayılan kütüphane aranır.
         * `tokenLabel` ve `pin` nullptr ise varsayılan değerler kullanılır.
         * 
         * @note Bu fonksiyon, zaten başlatılmışsa (InitStatus::INITIALIZED) hiçbir
         * işlem yapmaz ve Success döner. PKCS#11 kütüphanesi yüklendikten sonra,
         * C_Initialize() çağrılır ve token label ile PIN kaydedilir.
         * 
         * @param libraryPath SoftHSM PKCS#11 kütüphanesi yolu (nullptr ise otomatik bulma yapılır)
         * @param tokenLabel Token etiketi (nullptr ise varsayılan "TravelExpense" kullanılır)
         * @param pin PIN kodu (nullptr ise varsayılan "1234" kullanılır)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode initialize(const char* libraryPath, const char* tokenLabel, const char* pin) {
            if (g_status == InitStatus::INITIALIZED) {
                // Zaten başlatılmış
                return ErrorCode::Success;
            }

            // PKCS#11 kütüphanesini yükle
            ErrorCode result = loadPKCS11Library(libraryPath);
            if (result != ErrorCode::Success) {
                g_status = InitStatus::ERROR;
                return result;
            }

            // PKCS#11'yi başlat
            CK_ULONG rv = g_pFunctionList->C_Initialize(nullptr);
            if (rv != CKR_OK && rv != CKR_CRYPTOKI_ALREADY_INITIALIZED) {
                g_status = InitStatus::ERROR;
                return pkcs11ToErrorCode(rv);
            }

            // Token label ve PIN'i sakla
            if (tokenLabel) {
                SafeString::safeCopy(g_tokenLabel, sizeof(g_tokenLabel), tokenLabel);
            } else {
                SafeString::safeCopy(g_tokenLabel, sizeof(g_tokenLabel), "TravelExpense");
            }

            if (pin) {
                SafeString::safeCopy(g_pin, sizeof(g_pin), pin);
            } else {
                SafeString::safeCopy(g_pin, sizeof(g_pin), "1234");  // Varsayılan PIN
            }

            g_status = InitStatus::INITIALIZED;
            return ErrorCode::Success;
        }

        /**
         * @brief SoftHSM'yi kapat ve kaynakları temizle
         * 
         * SoftHSM modülünü kapatır ve tüm kaynakları temizler. Açık session varsa
         * kapatılır, PKCS#11 finalize edilir ve dinamik kütüphane kapatılır.
         * Token label ve PIN bilgileri güvenli şekilde temizlenir.
         * 
         * @note Bu fonksiyon, zaten kapatılmışsa (InitStatus::NOT_INITIALIZED)
         * hiçbir işlem yapmaz ve Success döner.
         * 
         * @return ErrorCode Başarı durumu
         */
        ErrorCode shutdown() {
            if (g_status != InitStatus::INITIALIZED) {
                return ErrorCode::Success;
            }

            // Session'ı kapat
            if (g_sessionHandle != 0 && g_pFunctionList) {
                g_pFunctionList->C_CloseSession(g_sessionHandle);
                g_sessionHandle = 0;
            }

            // PKCS#11'yi kapat
            if (g_pFunctionList) {
                g_pFunctionList->C_Finalize(nullptr);
                g_pFunctionList = nullptr;
            }

            // Kütüphaneyi kapat
            if (g_pkcs11Library) {
                DL_CLOSE(g_pkcs11Library);
                g_pkcs11Library = nullptr;
            }

            g_status = InitStatus::NOT_INITIALIZED;
            std::memset(g_tokenLabel, 0, sizeof(g_tokenLabel));
            std::memset(g_pin, 0, sizeof(g_pin));

            return ErrorCode::Success;
        }

        /**
         * @brief SoftHSM başlatma durumunu kontrol et
         * 
         * SoftHSM modülünün mevcut başlatma durumunu döndürür.
         * 
         * @return InitStatus Mevcut durum (NOT_INITIALIZED, INITIALIZED, ERROR)
         */
        InitStatus getStatus() {
            return g_status;
        }

        /**
         * @brief Token oluştur (simplified - gerçek implementasyonda token creation gerekir)
         * 
         * SoftHSM token'ları genellikle sistem seviyesinde oluşturulur.
         * Bu fonksiyon sadece token label ve PIN bilgilerini kaydeder.
         * Gerçek token oluşturma işlemi için SoftHSM CLI komutları gerekir.
         * 
         * @note Bu fonksiyon, simplified bir implementasyon olup gerçek token
         * oluşturma işlemini yapmaz. Sadece parametreleri saklar.
         * 
         * @param label Token etiketi (nullptr ise InvalidInput döner)
         * @param pin PIN kodu (nullptr ise InvalidInput döner)
         * @param soPin SO (Security Officer) PIN kodu (kullanılmıyor, nullptr olabilir)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode createToken(const char* label, const char* pin, const char* soPin) {
            // SoftHSM token'ları genellikle sistem seviyesinde oluşturulur
            // Burada sadece label'i kaydediyoruz
            if (!label || !pin) {
                return ErrorCode::InvalidInput;
            }

            // Token oluşturma için SoftHSM CLI komutları gerekir
            // Bu fonksiyon sadece parametreleri saklar
            SafeString::safeCopy(g_tokenLabel, sizeof(g_tokenLabel), label);
            SafeString::safeCopy(g_pin, sizeof(g_pin), pin);

            return ErrorCode::Success;
        }

        /**
         * @brief Token aç ve session başlat
         * 
         * Belirtilen token'ı açar ve PKCS#11 session başlatır. Token label'a göre
         * slot listesinde token aranır, bulunduktan sonra session açılır ve
         * PIN ile login yapılır.
         * 
         * @note Bu fonksiyon, SoftHSM'nin INITIALIZED durumunda olmasını gerektirir.
         * Eğer label nullptr ise, kaydedilmiş g_tokenLabel kullanılır.
         * Eğer pin nullptr ise, kaydedilmiş g_pin kullanılır.
         * 
         * @param label Token etiketi (nullptr ise kaydedilmiş label kullanılır)
         * @param pin PIN kodu (nullptr ise kaydedilmiş PIN kullanılır)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode openToken(const char* label, const char* pin) {
            if (g_status != InitStatus::INITIALIZED || !g_pFunctionList) {
                return ErrorCode::InvalidInput;
            }

            // Slot listesini al
            CK_ULONG slotCount = 0;
            CK_ULONG rv = g_pFunctionList->C_GetSlotList(CK_TRUE, nullptr, &slotCount);
            if (rv != CKR_OK || slotCount == 0) {
                return pkcs11ToErrorCode(rv);
            }

            std::vector<CK_SLOT_ID> slots(slotCount);
            rv = g_pFunctionList->C_GetSlotList(CK_TRUE, slots.data(), &slotCount);
            if (rv != CKR_OK) {
                return pkcs11ToErrorCode(rv);
            }

            // Label'a göre token bul
            CK_SLOT_ID selectedSlot = slots[0];  // Varsayılan olarak ilk slot
            const char* searchLabel = label ? label : g_tokenLabel;

            for (CK_ULONG i = 0; i < slotCount; ++i) {
                CK_TOKEN_INFO tokenInfo;
                rv = g_pFunctionList->C_GetTokenInfo(slots[i], &tokenInfo);
                if (rv == CKR_OK) {
                    // Label karşılaştırması (null-terminated değil, boşluklarla doldurulmuş)
                    char tokenLabel[33] = {0};
                    SafeString::safeCopy(tokenLabel, sizeof(tokenLabel), tokenInfo.label);
                    // Son boşlukları temizle
                    size_t len = std::strlen(tokenLabel);
                    while (len > 0 && tokenLabel[len - 1] == ' ') {
                        tokenLabel[--len] = '\0';
                    }
                    
                    if (std::strcmp(tokenLabel, searchLabel) == 0) {
                        selectedSlot = slots[i];
                        break;
                    }
                }
            }

            // Session aç
            CK_ULONG flags = CKF_RW_SESSION | CKF_SERIAL_SESSION;
            rv = g_pFunctionList->C_OpenSession(selectedSlot, flags, nullptr, nullptr, &g_sessionHandle);
            if (rv != CKR_OK) {
                return pkcs11ToErrorCode(rv);
            }

            // PIN ile login yap
            const char* loginPin = pin ? pin : g_pin;
            rv = g_pFunctionList->C_Login(g_sessionHandle, CKU_USER, 
                                         reinterpret_cast<CK_BYTE*>(const_cast<char*>(loginPin)),
                                         static_cast<CK_ULONG>(std::strlen(loginPin)));
            
            if (rv != CKR_OK && rv != CKR_USER_ALREADY_LOGGED_IN) {
                g_pFunctionList->C_CloseSession(g_sessionHandle);
                g_sessionHandle = 0;
                return pkcs11ToErrorCode(rv);
            }

            g_currentSlot = selectedSlot;
            return ErrorCode::Success;
        }

        /**
         * @brief Token kapat ve session'ı sonlandır
         * 
         * Açık token session'ını kapatır. Önce logout yapılır, sonra session kapatılır.
         * Session handle ve current slot sıfırlanır.
         * 
         * @note Bu fonksiyon, session açık değilse hiçbir işlem yapmaz ve Success döner.
         * 
         * @return ErrorCode Başarı durumu
         */
        ErrorCode closeToken() {
            if (g_sessionHandle == 0 || !g_pFunctionList) {
                return ErrorCode::Success;
            }

            g_pFunctionList->C_Logout(g_sessionHandle);
            g_pFunctionList->C_CloseSession(g_sessionHandle);
            g_sessionHandle = 0;
            g_currentSlot = 0;

            return ErrorCode::Success;
        }

        /**
         * @brief Kriptografik anahtar oluştur
         * 
         * Belirtilen tipte ve kullanım amacına göre kriptografik anahtar oluşturur.
         * Desteklenen anahtar tipleri: AES-256, RSA-2048, EC-P256.
         * Anahtar, token üzerinde saklanır ve keyId olarak döndürülür.
         * 
         * @note Bu fonksiyon, açık bir session gerektirir (openToken() çağrılmış olmalı).
         * Simetrik anahtarlar (AES) için C_GenerateKey, asimetrik anahtarlar (RSA, EC)
         * için C_GenerateKeyPair kullanılır.
         * 
         * @param keyType Anahtar tipi (AES_256, RSA_2048, EC_P256)
         * @param keyUsage Anahtar kullanım amacı (ENCRYPT_DECRYPT, SIGN_VERIFY, vb.)
         * @param keyLabel Anahtar etiketi (nullptr ise InvalidInput döner)
         * @param keyId Oluşturulan anahtarın ID'si (çıktı, nullptr ise InvalidInput döner)
         * @param keyIdLen keyId buffer boyutu (giriş), oluşturulan ID boyutu (çıktı)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode generateKey(KeyType keyType, KeyUsage keyUsage,
                             const char* keyLabel, uint8_t* keyId, size_t& keyIdLen) {
            if (g_sessionHandle == 0 || !g_pFunctionList || !keyLabel || !keyId) {
                return ErrorCode::InvalidInput;
            }

            CK_MECHANISM mechanism = {0};
            CK_ATTRIBUTE template_[10];
            CK_ULONG templateCount = 0;

            // Key type'a göre mechanism ve key class ayarla
            CK_ULONG keyClass = CKO_SECRET_KEY;
            CK_ULONG ckKeyType = CKK_AES;
            CK_ULONG keySize = 256 / 8;  // 32 bytes for AES-256

            switch (keyType) {
                case KeyType::AES_256:
                    mechanism.mechanism = CKM_AES_KEY_GEN;
                    ckKeyType = CKK_AES;
                    keySize = 32;  // 256 bits = 32 bytes
                    break;
                case KeyType::RSA_2048:
                    keyClass = CKO_PRIVATE_KEY;
                    mechanism.mechanism = CKM_RSA_PKCS_KEY_PAIR_GEN;
                    ckKeyType = CKK_RSA;
                    keySize = 0;  // RSA için farklı
                    break;
                case KeyType::EC_P256:
                    keyClass = CKO_PRIVATE_KEY;
                    mechanism.mechanism = CKM_EC_KEY_PAIR_GEN;
                    ckKeyType = CKK_EC;
                    keySize = 0;
                    break;
            }

            // Template oluştur
            template_[templateCount].type = CKA_CLASS;
            template_[templateCount].pValue = &keyClass;
            template_[templateCount].ulValueLen = sizeof(keyClass);
            templateCount++;

            template_[templateCount].type = CKA_KEY_TYPE;
            template_[templateCount].pValue = &ckKeyType;
            template_[templateCount].ulValueLen = sizeof(ckKeyType);
            templateCount++;

            template_[templateCount].type = CKA_LABEL;
            template_[templateCount].pValue = const_cast<char*>(keyLabel);
            template_[templateCount].ulValueLen = static_cast<CK_ULONG>(std::strlen(keyLabel));
            templateCount++;

            CK_BYTE true_value = CK_TRUE;
            CK_BYTE false_value = CK_FALSE;

            template_[templateCount].type = CKA_TOKEN;
            template_[templateCount].pValue = &true_value;
            template_[templateCount].ulValueLen = sizeof(true_value);
            templateCount++;

            template_[templateCount].type = CKA_PRIVATE;
            template_[templateCount].pValue = &true_value;
            template_[templateCount].ulValueLen = sizeof(true_value);
            templateCount++;

            // Key usage attributes
            if (keyUsage == KeyUsage::ENCRYPT_DECRYPT) {
                template_[templateCount].type = CKA_ENCRYPT;
                template_[templateCount].pValue = &true_value;
                template_[templateCount].ulValueLen = sizeof(true_value);
                templateCount++;

                template_[templateCount].type = CKA_DECRYPT;
                template_[templateCount].pValue = &true_value;
                template_[templateCount].ulValueLen = sizeof(true_value);
                templateCount++;
            } else if (keyUsage == KeyUsage::SIGN_VERIFY) {
                template_[templateCount].type = CKA_SIGN;
                template_[templateCount].pValue = &true_value;
                template_[templateCount].ulValueLen = sizeof(true_value);
                templateCount++;

                template_[templateCount].type = CKA_VERIFY;
                template_[templateCount].pValue = &true_value;
                template_[templateCount].ulValueLen = sizeof(true_value);
                templateCount++;
            }

            // AES için key size
            if (keyType == KeyType::AES_256 && keySize > 0) {
                template_[templateCount].type = CKA_VALUE_LEN;
                template_[templateCount].pValue = &keySize;
                template_[templateCount].ulValueLen = sizeof(keySize);
                templateCount++;
            }

            // Key generate
            CK_OBJECT_HANDLE keyHandle = 0;
            CK_ULONG rv;
            
            if (keyType == KeyType::RSA_2048 || keyType == KeyType::EC_P256) {
                // Key pair generation
                CK_ATTRIBUTE pubTemplate[5];
                CK_ULONG pubCount = 0;

                pubTemplate[pubCount].type = CKA_CLASS;
                pubTemplate[pubCount].pValue = &keyClass;
                pubTemplate[pubCount].ulValueLen = sizeof(keyClass);
                pubCount++;

                pubTemplate[pubCount].type = CKA_KEY_TYPE;
                pubTemplate[pubCount].pValue = &ckKeyType;
                pubTemplate[pubCount].ulValueLen = sizeof(ckKeyType);
                pubCount++;

                pubTemplate[pubCount].type = CKA_LABEL;
                pubTemplate[pubCount].pValue = const_cast<char*>(keyLabel);
                pubTemplate[pubCount].ulValueLen = static_cast<CK_ULONG>(std::strlen(keyLabel));
                pubCount++;

                CK_OBJECT_HANDLE pubKeyHandle = 0;
                rv = g_pFunctionList->C_GenerateKeyPair(g_sessionHandle, &mechanism,
                                                        pubTemplate, pubCount,
                                                        template_, templateCount,
                                                        &pubKeyHandle, &keyHandle);
            } else {
                // Secret key generation
                rv = g_pFunctionList->C_GenerateKey(g_sessionHandle, &mechanism,
                                                    template_, templateCount, &keyHandle);
            }

            if (rv != CKR_OK) {
                return pkcs11ToErrorCode(rv);
            }

            // Key ID olarak handle'ı kullan (veya label'den hash)
            if (keyIdLen >= sizeof(CK_OBJECT_HANDLE)) {
                std::memcpy(keyId, &keyHandle, sizeof(CK_OBJECT_HANDLE));
                keyIdLen = sizeof(CK_OBJECT_HANDLE);
            } else {
                // Key ID çok küçükse label'den hash oluştur
                size_t labelLen = std::strlen(keyLabel);
                size_t copyLen = (labelLen < keyIdLen) ? labelLen : keyIdLen;
                std::memcpy(keyId, keyLabel, copyLen);
                keyIdLen = copyLen;
            }

            return ErrorCode::Success;
        }

        /**
         * @brief Anahtarı label'a göre bul
         * 
         * Belirtilen label'a sahip anahtarı token üzerinde arar ve keyId'sini döndürür.
         * Arama, C_FindObjectsInit ve C_FindObjects kullanılarak yapılır.
         * 
         * @note Bu fonksiyon, açık bir session gerektirir (openToken() çağrılmış olmalı).
         * Anahtar bulunamazsa FileNotFound döner.
         * 
         * @param keyLabel Anahtar etiketi (nullptr ise InvalidInput döner)
         * @param keyId Bulunan anahtarın ID'si (çıktı, nullptr ise InvalidInput döner)
         * @param keyIdLen keyId buffer boyutu (giriş), bulunan ID boyutu (çıktı)
         * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput)
         */
        ErrorCode findKey(const char* keyLabel, uint8_t* keyId, size_t& keyIdLen) {
            if (g_sessionHandle == 0 || !g_pFunctionList || !keyLabel || !keyId) {
                return ErrorCode::InvalidInput;
            }

            // Template ile arama
            CK_ATTRIBUTE template_[3];
            template_[0].type = CKA_CLASS;
            CK_ULONG keyClass = CKO_SECRET_KEY;
            template_[0].pValue = &keyClass;
            template_[0].ulValueLen = sizeof(keyClass);

            template_[1].type = CKA_KEY_TYPE;
            CK_ULONG keyType = CKK_AES;
            template_[1].pValue = &keyType;
            template_[1].ulValueLen = sizeof(keyType);

            template_[2].type = CKA_LABEL;
            template_[2].pValue = const_cast<char*>(keyLabel);
            template_[2].ulValueLen = static_cast<CK_ULONG>(std::strlen(keyLabel));

            CK_ULONG rv = g_pFunctionList->C_FindObjectsInit(g_sessionHandle, template_, 3);
            if (rv != CKR_OK) {
                return pkcs11ToErrorCode(rv);
            }

            CK_OBJECT_HANDLE keyHandle = 0;
            CK_ULONG foundCount = 0;
            rv = g_pFunctionList->C_FindObjects(g_sessionHandle, &keyHandle, 1, &foundCount);
            g_pFunctionList->C_FindObjectsFinal(g_sessionHandle);

            if (rv != CKR_OK || foundCount == 0) {
                return ErrorCode::FileNotFound;
            }

            // Key ID olarak handle'ı kullan
            if (keyIdLen >= sizeof(CK_OBJECT_HANDLE)) {
                std::memcpy(keyId, &keyHandle, sizeof(CK_OBJECT_HANDLE));
                keyIdLen = sizeof(CK_OBJECT_HANDLE);
            } else {
                return ErrorCode::InvalidInput;
            }

            return ErrorCode::Success;
        }

        /**
         * @brief Anahtarı sil
         * 
         * Belirtilen keyId'ye sahip anahtarı token üzerinden siler.
         * C_DestroyObject kullanılarak anahtar kalıcı olarak silinir.
         * 
         * @note Bu fonksiyon, açık bir session gerektirir (openToken() çağrılmış olmalı).
         * Anahtar silindikten sonra geri getirilemez.
         * 
         * @param keyId Silinecek anahtarın ID'si (nullptr ise InvalidInput döner)
         * @param keyIdLen keyId buffer boyutu (CK_OBJECT_HANDLE boyutu olmalı)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode deleteKey(const uint8_t* keyId, size_t keyIdLen) {
            if (g_sessionHandle == 0 || !g_pFunctionList || !keyId) {
                return ErrorCode::InvalidInput;
            }

            CK_OBJECT_HANDLE keyHandle = 0;
            if (keyIdLen >= sizeof(CK_OBJECT_HANDLE)) {
                std::memcpy(&keyHandle, keyId, sizeof(CK_OBJECT_HANDLE));
            } else {
                return ErrorCode::InvalidInput;
            }

            CK_ULONG rv = g_pFunctionList->C_DestroyObject(g_sessionHandle, keyHandle);
            return pkcs11ToErrorCode(rv);
        }

        /**
         * @brief Veriyi şifrele (AES-CBC)
         * 
         * Belirtilen anahtar ile veriyi AES-CBC modunda şifreler.
         * IV (Initialization Vector) belirtilmemişse, PKCS#11'nin rastgele
         * sayı üreticisi ile oluşturulur.
         * 
         * @note Bu fonksiyon, açık bir session gerektirir (openToken() çağrılmış olmalı).
         * AES-CBC-PAD mekanizması kullanılır, bu nedenle padding otomatik olarak eklenir.
         * Ciphertext boyutu, plaintext boyutundan büyük olabilir.
         * 
         * @param keyId Şifreleme anahtarının ID'si (nullptr ise InvalidInput döner)
         * @param keyIdLen keyId buffer boyutu
         * @param plaintext Şifrelenecek veri (nullptr ise InvalidInput döner)
         * @param plaintextLen Veri uzunluğu (byte, 0 ise InvalidInput döner)
         * @param ciphertext Şifrelenmiş veri çıktısı (nullptr ise InvalidInput döner)
         * @param ciphertextLen ciphertext buffer boyutu (giriş), şifrelenmiş veri boyutu (çıktı)
         * @param iv Initialization Vector (nullptr ise otomatik oluşturulur)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode encrypt(const uint8_t* keyId, size_t keyIdLen,
                         const void* plaintext, size_t plaintextLen,
                         void* ciphertext, size_t& ciphertextLen,
                         uint8_t* iv) {
            if (g_sessionHandle == 0 || !g_pFunctionList || !keyId || !plaintext || 
                plaintextLen == 0 || !ciphertext) {
                return ErrorCode::InvalidInput;
            }

            CK_OBJECT_HANDLE keyHandle = 0;
            if (keyIdLen >= sizeof(CK_OBJECT_HANDLE)) {
                std::memcpy(&keyHandle, keyId, sizeof(CK_OBJECT_HANDLE));
            } else {
                return ErrorCode::InvalidInput;
            }

            // IV oluştur (eğer verilmemişse)
            uint8_t localIV[16] = {0};
            if (!iv) {
                CK_ULONG rv = g_pFunctionList->C_GenerateRandom(g_sessionHandle, localIV, 16);
                if (rv != CKR_OK) {
                    return pkcs11ToErrorCode(rv);
                }
                iv = localIV;
            }

            // AES-CBC mekanizması
            CK_MECHANISM mechanism;
            mechanism.mechanism = CKM_AES_CBC_PAD;
            mechanism.pParameter = iv;
            mechanism.ulParameterLen = 16;

            CK_ULONG rv = g_pFunctionList->C_EncryptInit(g_sessionHandle, &mechanism, keyHandle);
            if (rv != CKR_OK) {
                return pkcs11ToErrorCode(rv);
            }

            // Şifreleme
            CK_ULONG encryptedLen = static_cast<CK_ULONG>(ciphertextLen);
            rv = g_pFunctionList->C_Encrypt(g_sessionHandle,
                                           static_cast<CK_BYTE_PTR>(const_cast<void*>(plaintext)),
                                           static_cast<CK_ULONG>(plaintextLen),
                                           static_cast<CK_BYTE_PTR>(ciphertext),
                                           &encryptedLen);
            
            if (rv != CKR_OK) {
                return pkcs11ToErrorCode(rv);
            }

            ciphertextLen = encryptedLen;
            return ErrorCode::Success;
        }

        /**
         * @brief Şifrelenmiş veriyi çöz (AES-CBC)
         * 
         * Belirtilen anahtar ile şifrelenmiş veriyi AES-CBC modunda çözer.
         * IV (Initialization Vector) belirtilmemişse, ciphertext'in ilk 16 byte'ı
         * IV olarak kullanılır.
         * 
         * @note Bu fonksiyon, açık bir session gerektirir (openToken() çağrılmış olmalı).
         * AES-CBC-PAD mekanizması kullanılır, bu nedenle padding otomatik olarak kaldırılır.
         * Plaintext boyutu, ciphertext boyutundan küçük veya eşit olabilir.
         * 
         * @param keyId Şifre çözme anahtarının ID'si (nullptr ise InvalidInput döner)
         * @param keyIdLen keyId buffer boyutu
         * @param ciphertext Şifrelenmiş veri (nullptr ise InvalidInput döner)
         * @param ciphertextLen Şifrelenmiş veri uzunluğu (byte, 0 ise InvalidInput döner)
         * @param plaintext Çözülmüş veri çıktısı (nullptr ise InvalidInput döner)
         * @param plaintextLen plaintext buffer boyutu (giriş), çözülmüş veri boyutu (çıktı)
         * @param iv Initialization Vector (nullptr ise ciphertext'in ilk 16 byte'ı kullanılır)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode decrypt(const uint8_t* keyId, size_t keyIdLen,
                         const void* ciphertext, size_t ciphertextLen,
                         void* plaintext, size_t& plaintextLen,
                         const uint8_t* iv) {
            if (g_sessionHandle == 0 || !g_pFunctionList || !keyId || !ciphertext ||
                ciphertextLen == 0 || !plaintext) {
                return ErrorCode::InvalidInput;
            }

            CK_OBJECT_HANDLE keyHandle = 0;
            if (keyIdLen >= sizeof(CK_OBJECT_HANDLE)) {
                std::memcpy(&keyHandle, keyId, sizeof(CK_OBJECT_HANDLE));
            } else {
                return ErrorCode::InvalidInput;
            }

            // IV (ciphertext'in başından okunabilir veya parametre olarak verilebilir)
            uint8_t localIV[16] = {0};
            if (!iv && ciphertextLen >= 16) {
                // İlk 16 byte IV
                std::memcpy(localIV, ciphertext, 16);
                iv = localIV;
                // Ciphertext'i IV olmadan düzenle (gerçek implementasyonda dikkatli olunmalı)
            } else if (!iv) {
                return ErrorCode::InvalidInput;
            }

            // AES-CBC mekanizması
            CK_MECHANISM mechanism;
            mechanism.mechanism = CKM_AES_CBC_PAD;
            mechanism.pParameter = const_cast<uint8_t*>(iv);
            mechanism.ulParameterLen = 16;

            CK_ULONG rv = g_pFunctionList->C_DecryptInit(g_sessionHandle, &mechanism, keyHandle);
            if (rv != CKR_OK) {
                return pkcs11ToErrorCode(rv);
            }

            // Şifre çözme
            CK_ULONG decryptedLen = static_cast<CK_ULONG>(plaintextLen);
            rv = g_pFunctionList->C_Decrypt(g_sessionHandle,
                                           static_cast<CK_BYTE_PTR>(const_cast<void*>(ciphertext)),
                                           static_cast<CK_ULONG>(ciphertextLen),
                                           static_cast<CK_BYTE_PTR>(plaintext),
                                           &decryptedLen);
            
            if (rv != CKR_OK) {
                return pkcs11ToErrorCode(rv);
            }

            plaintextLen = decryptedLen;
            return ErrorCode::Success;
        }

        /**
         * @brief Veriyi imzala (SHA256-RSA-PKCS)
         * 
         * Belirtilen anahtar ile veriyi SHA256 hash'leyip RSA-PKCS imzası oluşturur.
         * SHA256-RSA-PKCS mekanizması kullanılır.
         * 
         * @note Bu fonksiyon, açık bir session gerektirir (openToken() çağrılmış olmalı).
         * Veri önce SHA256 ile hash'lenir, sonra RSA ile imzalanır.
         * Signature boyutu, RSA anahtar boyutuna bağlıdır (2048-bit için 256 byte).
         * 
         * @param keyId İmzalama anahtarının ID'si (nullptr ise InvalidInput döner)
         * @param keyIdLen keyId buffer boyutu
         * @param data İmzalanacak veri (nullptr ise InvalidInput döner)
         * @param dataLen Veri uzunluğu (byte, 0 ise InvalidInput döner)
         * @param signature İmza çıktısı (nullptr ise InvalidInput döner)
         * @param signatureLen signature buffer boyutu (giriş), imza boyutu (çıktı)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode sign(const uint8_t* keyId, size_t keyIdLen,
                      const void* data, size_t dataLen,
                      void* signature, size_t& signatureLen) {
            if (g_sessionHandle == 0 || !g_pFunctionList || !keyId || !data ||
                dataLen == 0 || !signature) {
                return ErrorCode::InvalidInput;
            }

            CK_OBJECT_HANDLE keyHandle = 0;
            if (keyIdLen >= sizeof(CK_OBJECT_HANDLE)) {
                std::memcpy(&keyHandle, keyId, sizeof(CK_OBJECT_HANDLE));
            } else {
                return ErrorCode::InvalidInput;
            }

            // SHA256-RSA-PKCS mechanism
            CK_MECHANISM mechanism;
            mechanism.mechanism = CKM_SHA256_RSA_PKCS;
            mechanism.pParameter = nullptr;
            mechanism.ulParameterLen = 0;

            CK_ULONG rv = g_pFunctionList->C_SignInit(g_sessionHandle, &mechanism, keyHandle);
            if (rv != CKR_OK) {
                return pkcs11ToErrorCode(rv);
            }

            // İmzalama
            CK_ULONG sigLen = static_cast<CK_ULONG>(signatureLen);
            rv = g_pFunctionList->C_Sign(g_sessionHandle,
                                        static_cast<CK_BYTE_PTR>(const_cast<void*>(data)),
                                        static_cast<CK_ULONG>(dataLen),
                                        static_cast<CK_BYTE_PTR>(signature),
                                        &sigLen);
            
            if (rv != CKR_OK) {
                return pkcs11ToErrorCode(rv);
            }

            signatureLen = sigLen;
            return ErrorCode::Success;
        }

        /**
         * @brief İmzayı doğrula (SHA256-RSA-PKCS)
         * 
         * Belirtilen anahtar ile veriyi SHA256 hash'leyip RSA-PKCS imzasını doğrular.
         * SHA256-RSA-PKCS mekanizması kullanılır.
         * 
         * @note Bu fonksiyon, açık bir session gerektirir (openToken() çağrılmış olmalı).
         * Veri önce SHA256 ile hash'lenir, sonra RSA ile imza doğrulanır.
         * 
         * @param keyId Doğrulama anahtarının ID'si (nullptr ise false döner)
         * @param keyIdLen keyId buffer boyutu
         * @param data Doğrulanacak veri (nullptr ise false döner)
         * @param dataLen Veri uzunluğu (byte, 0 ise false döner)
         * @param signature İmza (nullptr ise false döner)
         * @param signatureLen İmza uzunluğu (byte, 0 ise false döner)
         * @return true İmza geçerli, false İmza geçersiz veya hata
         */
        bool verify(const uint8_t* keyId, size_t keyIdLen,
                   const void* data, size_t dataLen,
                   const void* signature, size_t signatureLen) {
            if (g_sessionHandle == 0 || !g_pFunctionList || !keyId || !data ||
                dataLen == 0 || !signature || signatureLen == 0) {
                return false;
            }

            CK_OBJECT_HANDLE keyHandle = 0;
            if (keyIdLen >= sizeof(CK_OBJECT_HANDLE)) {
                std::memcpy(&keyHandle, keyId, sizeof(CK_OBJECT_HANDLE));
            } else {
                return false;
            }

            // SHA256-RSA-PKCS mechanism
            CK_MECHANISM mechanism;
            mechanism.mechanism = CKM_SHA256_RSA_PKCS;
            mechanism.pParameter = nullptr;
            mechanism.ulParameterLen = 0;

            CK_ULONG rv = g_pFunctionList->C_VerifyInit(g_sessionHandle, &mechanism, keyHandle);
            if (rv != CKR_OK) {
                return false;
            }

            // Doğrulama
            rv = g_pFunctionList->C_Verify(g_sessionHandle,
                                           static_cast<CK_BYTE_PTR>(const_cast<void*>(data)),
                                           static_cast<CK_ULONG>(dataLen),
                                           static_cast<CK_BYTE_PTR>(const_cast<void*>(signature)),
                                           static_cast<CK_ULONG>(signatureLen));
            
            return (rv == CKR_OK);
        }

        /**
         * @brief Kriptografik olarak güvenli rastgele veri üret
         * 
         * PKCS#11'nin rastgele sayı üreticisini kullanarak kriptografik olarak
         * güvenli rastgele veri üretir. C_GenerateRandom fonksiyonu kullanılır.
         * 
         * @note Bu fonksiyon, açık bir session gerektirir (openToken() çağrılmış olmalı).
         * Üretilen rastgele veriler, kriptografik işlemler için uygundur (IV, salt, vb.).
         * 
         * @param output Rastgele veri çıktısı (nullptr ise InvalidInput döner)
         * @param length Üretilecek veri uzunluğu (byte, 0 ise InvalidInput döner)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode generateRandom(uint8_t* output, size_t length) {
            if (g_sessionHandle == 0 || !g_pFunctionList || !output || length == 0) {
                return ErrorCode::InvalidInput;
            }

            CK_ULONG rv = g_pFunctionList->C_GenerateRandom(g_sessionHandle, output, 
                                                           static_cast<CK_ULONG>(length));
            return pkcs11ToErrorCode(rv);
        }

        /**
         * @brief Mevcut token'ları listele
         * 
         * Sistemde mevcut tüm token'ların label'larını listeler.
         * C_GetSlotList ve C_GetTokenInfo kullanılarak token bilgileri alınır.
         * 
         * @note Bu fonksiyon, SoftHSM'nin INITIALIZED durumunda olmasını gerektirir.
         * Maksimum 10 token listelenir. Token label'ları, boşluklarla doldurulmuş
         * olabilir, bu nedenle son boşluklar temizlenir.
         * 
         * @param labels Token label'ları dizisi (nullptr ise InvalidInput döner)
         * @param count labels dizisi boyutu (giriş), bulunan token sayısı (çıktı)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode listTokens(char labels[][64], size_t& count) {
            if (g_status != InitStatus::INITIALIZED || !g_pFunctionList || !labels) {
                return ErrorCode::InvalidInput;
            }

            CK_ULONG slotCount = 0;
            CK_ULONG rv = g_pFunctionList->C_GetSlotList(CK_TRUE, nullptr, &slotCount);
            if (rv != CKR_OK || slotCount == 0) {
                count = 0;
                return pkcs11ToErrorCode(rv);
            }

            std::vector<CK_SLOT_ID> slots(slotCount);
            rv = g_pFunctionList->C_GetSlotList(CK_TRUE, slots.data(), &slotCount);
            if (rv != CKR_OK) {
                count = 0;
                return pkcs11ToErrorCode(rv);
            }

            size_t foundCount = 0;
            for (CK_ULONG i = 0; i < slotCount && foundCount < count && foundCount < 10; ++i) {
                CK_TOKEN_INFO tokenInfo;
                rv = g_pFunctionList->C_GetTokenInfo(slots[i], &tokenInfo);
                if (rv == CKR_OK) {
                    // Label'i kopyala (32 byte, boşluklarla doldurulmuş)
                    SafeString::safeCopy(labels[foundCount], 64, tokenInfo.label);
                    
                    // Son boşlukları temizle
                    size_t len = std::strlen(labels[foundCount]);
                    while (len > 0 && labels[foundCount][len - 1] == ' ') {
                        labels[foundCount][--len] = '\0';
                    }
                    
                    foundCount++;
                }
            }

            count = foundCount;
            return ErrorCode::Success;
        }

        /**
         * @brief Mevcut anahtarları listele
         * 
         * Token üzerinde mevcut tüm secret key'lerin label'larını listeler.
         * C_FindObjectsInit, C_FindObjects ve C_GetAttributeValue kullanılarak
         * anahtar bilgileri alınır.
         * 
         * @note Bu fonksiyon, açık bir session gerektirir (openToken() çağrılmış olmalı).
         * Maksimum 50 anahtar listelenir. Sadece secret key'ler listelenir.
         * 
         * @param labels Anahtar label'ları dizisi (nullptr ise InvalidInput döner)
         * @param count labels dizisi boyutu (giriş), bulunan anahtar sayısı (çıktı)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode listKeys(char labels[][64], size_t& count) {
            if (g_sessionHandle == 0 || !g_pFunctionList || !labels) {
                return ErrorCode::InvalidInput;
            }

            // Tüm secret key'leri bul
            CK_ATTRIBUTE template_[1];
            CK_ULONG keyClass = CKO_SECRET_KEY;
            template_[0].type = CKA_CLASS;
            template_[0].pValue = &keyClass;
            template_[0].ulValueLen = sizeof(keyClass);

            CK_ULONG rv = g_pFunctionList->C_FindObjectsInit(g_sessionHandle, template_, 1);
            if (rv != CKR_OK) {
                count = 0;
                return pkcs11ToErrorCode(rv);
            }

            size_t foundCount = 0;
            const size_t maxKeys = (count < 50) ? count : 50;
            
            for (size_t i = 0; i < maxKeys; ++i) {
                CK_OBJECT_HANDLE keyHandle = 0;
                CK_ULONG objCount = 0;
                rv = g_pFunctionList->C_FindObjects(g_sessionHandle, &keyHandle, 1, &objCount);
                if (rv != CKR_OK || objCount == 0) {
                    break;
                }

                // Label attribute'unu al
                CK_ATTRIBUTE attr;
                attr.type = CKA_LABEL;
                char labelBuffer[64] = {0};
                attr.pValue = labelBuffer;
                attr.ulValueLen = sizeof(labelBuffer) - 1;

                rv = g_pFunctionList->C_GetAttributeValue(g_sessionHandle, keyHandle, &attr, 1);
                if (rv == CKR_OK && attr.ulValueLen > 0) {
                    SafeString::safeCopy(labels[foundCount], 64, labelBuffer);
                    labels[foundCount][63] = '\0';
                    foundCount++;
                }
            }

            g_pFunctionList->C_FindObjectsFinal(g_sessionHandle);
            count = foundCount;

            return ErrorCode::Success;
        }

        /**
         * @brief Anahtarı import et (simplified)
         * 
         * Belirtilen anahtar verisini token üzerine import eder.
         * C_CreateObject kullanılarak anahtar oluşturulur ve token üzerinde saklanır.
         * 
         * @note Bu fonksiyon, açık bir session gerektirir (openToken() çağrılmış olmalı).
         * Desteklenen anahtar tipleri: AES-256, RSA-2048, EC-P256.
         * Anahtar, token üzerinde saklanır ve keyId olarak döndürülür.
         * 
         * @param keyType Anahtar tipi (AES_256, RSA_2048, EC_P256)
         * @param keyData Import edilecek anahtar verisi (nullptr ise InvalidInput döner)
         * @param keyDataLen Anahtar verisi uzunluğu (byte, 0 ise InvalidInput döner)
         * @param keyLabel Anahtar etiketi (nullptr ise InvalidInput döner)
         * @param keyId Import edilen anahtarın ID'si (çıktı, nullptr ise InvalidInput döner)
         * @param keyIdLen keyId buffer boyutu (giriş), oluşturulan ID boyutu (çıktı)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode importKey(KeyType keyType,
                           const uint8_t* keyData, size_t keyDataLen,
                           const char* keyLabel, uint8_t* keyId, size_t& keyIdLen) {
            if (g_sessionHandle == 0 || !g_pFunctionList || !keyData || keyDataLen == 0 ||
                !keyLabel || !keyId) {
                return ErrorCode::InvalidInput;
            }

            CK_ATTRIBUTE template_[10];
            CK_ULONG templateCount = 0;

            CK_ULONG keyClass = CKO_SECRET_KEY;
            CK_ULONG ckKeyType = CKK_AES;
            CK_BYTE true_value = CK_TRUE;

            switch (keyType) {
                case KeyType::AES_256:
                    ckKeyType = CKK_AES;
                    break;
                case KeyType::RSA_2048:
                    keyClass = CKO_PRIVATE_KEY;
                    ckKeyType = CKK_RSA;
                    break;
                case KeyType::EC_P256:
                    keyClass = CKO_PRIVATE_KEY;
                    ckKeyType = CKK_EC;
                    break;
            }

            template_[templateCount].type = CKA_CLASS;
            template_[templateCount].pValue = &keyClass;
            template_[templateCount].ulValueLen = sizeof(keyClass);
            templateCount++;

            template_[templateCount].type = CKA_KEY_TYPE;
            template_[templateCount].pValue = &ckKeyType;
            template_[templateCount].ulValueLen = sizeof(ckKeyType);
            templateCount++;

            template_[templateCount].type = CKA_LABEL;
            template_[templateCount].pValue = const_cast<char*>(keyLabel);
            template_[templateCount].ulValueLen = static_cast<CK_ULONG>(std::strlen(keyLabel));
            templateCount++;

            template_[templateCount].type = CKA_VALUE;
            template_[templateCount].pValue = const_cast<uint8_t*>(keyData);
            template_[templateCount].ulValueLen = static_cast<CK_ULONG>(keyDataLen);
            templateCount++;

            template_[templateCount].type = CKA_TOKEN;
            template_[templateCount].pValue = &true_value;
            template_[templateCount].ulValueLen = sizeof(true_value);
            templateCount++;

            template_[templateCount].type = CKA_ENCRYPT;
            template_[templateCount].pValue = &true_value;
            template_[templateCount].ulValueLen = sizeof(true_value);
            templateCount++;

            template_[templateCount].type = CKA_DECRYPT;
            template_[templateCount].pValue = &true_value;
            template_[templateCount].ulValueLen = sizeof(true_value);
            templateCount++;

            CK_OBJECT_HANDLE keyHandle = 0;
            CK_ULONG rv = g_pFunctionList->C_CreateObject(g_sessionHandle, template_, 
                                                          templateCount, &keyHandle);
            if (rv != CKR_OK) {
                return pkcs11ToErrorCode(rv);
            }

            // Key ID olarak handle'ı kullan
            if (keyIdLen >= sizeof(CK_OBJECT_HANDLE)) {
                std::memcpy(keyId, &keyHandle, sizeof(CK_OBJECT_HANDLE));
                keyIdLen = sizeof(CK_OBJECT_HANDLE);
            } else {
                return ErrorCode::InvalidInput;
            }

            return ErrorCode::Success;
        }

    } // namespace SoftHSM

} // namespace TravelExpense

