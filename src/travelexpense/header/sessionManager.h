/**
 * @file sessionManager.h
 * @brief Seyahat Gideri Takibi - Oturum Yönetimi ve İletim Güvenliği
 * 
 * Bu dosya, İletim Sırasındaki Veri Güvenliği gereksinimlerini karşılar:
 * - Oturum Anahtarı: Şifrelenmiş oturum anahtarları kullanılması
 * - Cihaz Bağlantısı ve Sürüm Bağlantısı: Belirli cihazlar ve sürümlerde çalışma
 * - Gizli Veri (Confidential Payload): Taşınan verilerin şifrelenmesi
 * - Bütünlük Kontrolü ve Kimlik Doğrulama: Verilerin doğruluğu ve kaynağı
 * - Sunucu Doğrulama Kodu: Dijital imza doğrulaması
 * - Oturum Anahtarı Şifreleme: Oturum anahtarlarının şifrelenerek iletilmesi
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "commonTypes.h"
#include "export.h"
#include <cstdint>
#include <cstddef>

namespace TravelExpense {

    /**
     * @namespace SessionManager
     * @brief Oturum yönetimi ve iletişim güvenliği modülü
     * 
     * Bu modül, oturum anahtarı yönetimi, cihaz ve sürüm kontrolü,
     * gizli veri (confidential payload) şifreleme, bütünlük kontrolü
     * ve dijital imza işlemlerini yönetir. İletim sırasındaki veri
     * güvenliği gereksinimlerini karşılar.
     */
    namespace SessionManager {

        // ============================================
        // OTURUM ANAHTARI YÖNETİMİ
        // ============================================

        /**
         * @brief Oturum anahtarı oluştur
         * 
         * @param sessionKey Çıktı oturum anahtarı (32 byte)
         * @param sessionKeyLen Oturum anahtarı uzunluğu (32)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode generateSessionKey(uint8_t* sessionKey, size_t sessionKeyLen);

        /**
         * @brief Oturum anahtarını şifrele
         * 
         * Oturum anahtarlarının şifrelenerek iletilmesi gereksinimini karşılar.
         * 
         * @param plainSessionKey Şifrelenmemiş oturum anahtarı (32 byte)
         * @param encryptedSessionKey Şifrelenmiş oturum anahtarı çıktısı
         * @param encryptedLen Şifrelenmiş veri uzunluğu (çıktı)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode encryptSessionKey(const uint8_t* plainSessionKey,
                                                       uint8_t* encryptedSessionKey,
                                                       size_t& encryptedLen);

        /**
         * @brief Şifrelenmiş oturum anahtarını çöz
         * 
         * @param encryptedSessionKey Şifrelenmiş oturum anahtarı
         * @param encryptedLen Şifrelenmiş veri uzunluğu
         * @param plainSessionKey Şifre çözülmüş oturum anahtarı çıktısı (32 byte)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode decryptSessionKey(const uint8_t* encryptedSessionKey,
                                                      size_t encryptedLen,
                                                      uint8_t* plainSessionKey);

        // ============================================
        // CİHAZ BAĞLANTISI VE SÜRÜM KONTROLÜ
        // ============================================

        /**
         * @brief Cihaz fingerprint'ini al
         * 
         * Uygulamanın yalnızca belirli cihazlarda çalışması gereksinimini karşılar.
         * 
         * @param fingerprint Cihaz fingerprint çıktısı (64 karakter hex string)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode getDeviceFingerprint(char* fingerprint);

        /**
         * @brief Uygulama sürümünü al
         * 
         * @param version Sürüm string çıktısı
         * @param versionLen Maksimum uzunluk
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode getApplicationVersion(char* version, size_t versionLen);

        /**
         * @brief Cihaz ve sürüm kontrolü yap
         * 
         * Uygulamanın yalnızca belirli cihazlar ve sürümlerde çalışması gereksinimini karşılar.
         * 
         * @param deviceFingerprint Kontrol edilecek cihaz fingerprint'i
         * @param appVersion Kontrol edilecek uygulama sürümü
         * @return ErrorCode Başarı durumu (SUCCESS = geçerli, ERROR = geçersiz)
         */
        TRAVELEXPENSE_API ErrorCode validateDeviceAndVersion(const char* deviceFingerprint,
                                                             const char* appVersion);

        // ============================================
        // GİZLİ VERİ (CONFIDENTIAL PAYLOAD)
        // ============================================

        /**
         * @brief Veriyi şifrele (Confidential Payload)
         * 
         * Taşınan verilerin şifrelenmesi gereksinimini karşılar.
         * 
         * @param plaintext Şifrelenecek veri
         * @param plaintextLen Veri uzunluğu
         * @param sessionKey Oturum anahtarı (32 byte)
         * @param ciphertext Şifrelenmiş veri çıktısı
         * @param ciphertextLen Şifrelenmiş veri uzunluğu (çıktı)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode encryptPayload(const void* plaintext, size_t plaintextLen,
                                                   const uint8_t* sessionKey,
                                                   void* ciphertext, size_t& ciphertextLen);

        /**
         * @brief Şifrelenmiş veriyi çöz (Confidential Payload)
         * 
         * @param ciphertext Şifrelenmiş veri
         * @param ciphertextLen Şifrelenmiş veri uzunluğu
         * @param sessionKey Oturum anahtarı (32 byte)
         * @param plaintext Şifre çözülmüş veri çıktısı
         * @param plaintextLen Şifre çözülmüş veri uzunluğu (çıktı)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode decryptPayload(const void* ciphertext, size_t ciphertextLen,
                                                   const uint8_t* sessionKey,
                                                   void* plaintext, size_t& plaintextLen);

        // ============================================
        // BÜTÜNLÜK KONTROLÜ VE KİMLİK DOĞRULAMA
        // ============================================

        /**
         * @brief Veri bütünlük kontrolü (HMAC)
         * 
         * Verilerin doğruluğu ve kaynağı kontrolü gereksinimini karşılar.
         * 
         * @param data Kontrol edilecek veri
         * @param dataLen Veri uzunluğu
         * @param sessionKey Oturum anahtarı (HMAC için)
         * @param hmac HMAC çıktısı (32 byte - 64 karakter hex string)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode calculateHMAC(const void* data, size_t dataLen,
                                                    const uint8_t* sessionKey,
                                                    char* hmac);

        /**
         * @brief HMAC doğrulama
         * 
         * @param data Kontrol edilecek veri
         * @param dataLen Veri uzunluğu
         * @param sessionKey Oturum anahtarı
         * @param expectedHMAC Beklenen HMAC değeri (64 karakter hex string)
         * @return ErrorCode Başarı durumu (SUCCESS = geçerli, ERROR_CHECKSUM_MISMATCH = geçersiz)
         */
        TRAVELEXPENSE_API ErrorCode verifyHMAC(const void* data, size_t dataLen,
                                                const uint8_t* sessionKey,
                                                const char* expectedHMAC);

        // ============================================
        // SUNUCU DOĞRULAMA KODU (DİJİTAL İMZA)
        // ============================================

        /**
         * @brief Veri imzala
         * 
         * Sunucu tarafında dijital imza doğrulaması gereksinimini karşılar.
         * 
         * @param data İmzalanacak veri
         * @param dataLen Veri uzunluğu
         * @param signature Dijital imza çıktısı (64 byte - 128 karakter hex string)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode signData(const void* data, size_t dataLen,
                                             char* signature);

        /**
         * @brief Dijital imza doğrulama
         * 
         * @param data Doğrulanacak veri
         * @param dataLen Veri uzunluğu
         * @param signature Beklenen dijital imza (128 karakter hex string)
         * @return ErrorCode Başarı durumu (SUCCESS = geçerli, ERROR_CHECKSUM_MISMATCH = geçersiz)
         */
        TRAVELEXPENSE_API ErrorCode verifySignature(const void* data, size_t dataLen,
                                                    const char* signature);

    } // namespace SessionManager

} // namespace TravelExpense

#endif // SESSION_MANAGER_H

