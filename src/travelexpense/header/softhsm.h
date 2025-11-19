/**
 * @file softhsm.h
 * @brief Seyahat Gideri Takibi - SoftHSM (PKCS#11) Entegrasyonu
 *
 * Bu dosya, SoftHSM kullanarak PKCS#11 standardı üzerinden kriptografik
 * işlemler yapmak için gerekli fonksiyonların bildirimlerini içerir.
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef SOFTHSM_H
#define SOFTHSM_H

#include "commonTypes.h"
#include "export.h"
#include <cstdint>
#include <cstddef>

namespace TravelExpense { // LCOV_EXCL_LINE

/**
 * @namespace SoftHSM
 * @brief SoftHSM (PKCS#11) kriptografik işlemler modülü
 */
namespace SoftHSM { // LCOV_EXCL_LINE

/**
 * @brief SoftHSM başlatma durumu
 *
 * SoftHSM modülünün başlatma durumunu belirtir.
 */
enum class InitStatus {
  NOT_INITIALIZED = 0,  /**< @brief SoftHSM henüz başlatılmadı */
  INITIALIZED = 1,      /**< @brief SoftHSM başarıyla başlatıldı */
  ERROR = -1           /**< @brief SoftHSM başlatma hatası */
};

/**
 * @brief Key tipi
 *
 * Desteklenen kriptografik anahtar tipleri.
 */
enum class KeyType {
  AES_256 = 0,   /**< @brief AES-256 simetrik şifreleme anahtarı */
  RSA_2048 = 1,  /**< @brief RSA-2048 asimetrik şifreleme anahtarı */
  EC_P256 = 2    /**< @brief EC-P256 (secp256r1) eliptik eğri anahtarı */
};

/**
 * @brief Key kullanım amaçları
 *
 * Anahtarın hangi kriptografik işlemler için kullanılabileceğini belirtir.
 */
enum class KeyUsage {
  ENCRYPT_DECRYPT = 0,  /**< @brief Şifreleme ve şifre çözme işlemleri için */
  SIGN_VERIFY = 1,      /**< @brief İmzalama ve doğrulama işlemleri için */
  WRAP_UNWRAP = 2,     /**< @brief Anahtar sarma/çözme işlemleri için */
  DERIVE = 3           /**< @brief Anahtar türetme işlemleri için */
};

/**
 * @brief SoftHSM'yi başlat ve PKCS#11 kütüphanesini yükle
 *
 * @param libraryPath SoftHSM PKCS#11 kütüphanesi yolu (nullptr ise otomatik bulma yapılır)
 * @param tokenLabel Token etiketi (nullptr ise varsayılan token oluşturulur)
 * @param pin PIN kodu (nullptr ise varsayılan "1234" kullanılır)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode initialize(const char *libraryPath = nullptr,
                                       const char *tokenLabel = nullptr,
                                       const char *pin = nullptr);

/**
 * @brief SoftHSM'yi kapat ve kaynakları temizle
 *
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode shutdown();

/**
 * @brief SoftHSM başlatma durumunu kontrol et
 *
 * @return InitStatus Başlatma durumu
 */
TRAVELEXPENSE_API InitStatus getStatus();

/**
 * @brief Token oluştur (eğer yoksa)
 *
 * @param label Token etiketi
 * @param pin PIN kodu
 * @param soPin Security Officer PIN kodu
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode createToken(const char *label, const char *pin, const char *soPin);

/**
 * @brief Token aç (oturum başlat)
 *
 * @param label Token etiketi (nullptr ise varsayılan token)
 * @param pin PIN kodu
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode openToken(const char *label, const char *pin);

/**
 * @brief Token'ı kapat (oturum kapat)
 *
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode closeToken();

/**
 * @brief Anahtar oluştur (generate key)
 *
 * @param keyType Anahtar tipi (AES, RSA, EC)
 * @param keyUsage Anahtar kullanım amacı
 * @param keyLabel Anahtar etiketi (identifier)
 * @param keyId Çıktı: Oluşturulan anahtar ID'si
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode generateKey(KeyType keyType, KeyUsage keyUsage,
                                        const char *keyLabel, uint8_t *keyId, size_t &keyIdLen);

/**
 * @brief Anahtarı içe aktar (import key)
 *
 * @param keyType Anahtar tipi
 * @param keyData Anahtar verisi
 * @param keyDataLen Anahtar verisi uzunluğu
 * @param keyLabel Anahtar etiketi
 * @param keyId Çıktı: İçe aktarılan anahtar ID'si
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode importKey(KeyType keyType,
                                      const uint8_t *keyData, size_t keyDataLen,
                                      const char *keyLabel, uint8_t *keyId, size_t &keyIdLen);

/**
 * @brief Anahtarı bul (find key by label)
 *
 * @param keyLabel Anahtar etiketi
 * @param keyId Çıktı: Bulunan anahtar ID'si
 * @param keyIdLen Çıktı: Anahtar ID uzunluğu
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode findKey(const char *keyLabel, uint8_t *keyId, size_t &keyIdLen);

/**
 * @brief Anahtarı sil
 *
 * @param keyId Anahtar ID'si
 * @param keyIdLen Anahtar ID uzunluğu
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode deleteKey(const uint8_t *keyId, size_t keyIdLen);

/**
 * @brief Veriyi şifrele (encrypt using HSM key)
 *
 * @param keyId Anahtar ID'si
 * @param keyIdLen Anahtar ID uzunluğu
 * @param plaintext Şifrelenecek veri
 * @param plaintextLen Veri uzunluğu
 * @param ciphertext Şifrelenmiş veri çıktısı
 * @param ciphertextLen Şifrelenmiş veri uzunluğu (çıktı)
 * @param iv IV çıktısı (nullptr ise otomatik oluşturulur)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode encrypt(const uint8_t *keyId, size_t keyIdLen,
                                    const void *plaintext, size_t plaintextLen,
                                    void *ciphertext, size_t &ciphertextLen,
                                    uint8_t *iv = nullptr);

/**
 * @brief Veriyi şifre çöz (decrypt using HSM key)
 *
 * @param keyId Anahtar ID'si
 * @param keyIdLen Anahtar ID uzunluğu
 * @param ciphertext Şifrelenmiş veri
 * @param ciphertextLen Şifrelenmiş veri uzunluğu
 * @param plaintext Şifre çözülmüş veri çıktısı
 * @param plaintextLen Şifre çözülmüş veri uzunluğu (çıktı)
 * @param iv IV (nullptr ise ciphertext'in başından okunur)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode decrypt(const uint8_t *keyId, size_t keyIdLen,
                                    const void *ciphertext, size_t ciphertextLen,
                                    void *plaintext, size_t &plaintextLen,
                                    const uint8_t *iv = nullptr);

/**
 * @brief Veriyi imzala (sign data)
 *
 * @param keyId Anahtar ID'si
 * @param keyIdLen Anahtar ID uzunluğu
 * @param data İmzalanacak veri
 * @param dataLen Veri uzunluğu
 * @param signature İmza çıktısı
 * @param signatureLen İmza uzunluğu (çıktı)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode sign(const uint8_t *keyId, size_t keyIdLen,
                                 const void *data, size_t dataLen,
                                 void *signature, size_t &signatureLen);

/**
 * @brief İmzayı doğrula (verify signature)
 *
 * @param keyId Anahtar ID'si
 * @param keyIdLen Anahtar ID uzunluğu
 * @param data İmzalanmış veri
 * @param dataLen Veri uzunluğu
 * @param signature İmza
 * @param signatureLen İmza uzunluğu
 * @return true İmza geçerli, false Geçersiz
 */
TRAVELEXPENSE_API bool verify(const uint8_t *keyId, size_t keyIdLen,
                              const void *data, size_t dataLen,
                              const void *signature, size_t signatureLen);

/**
 * @brief Rastgele veri üret (HSM'den güvenli random)
 *
 * @param output Rastgele veri çıktısı
 * @param length İstenen uzunluk (byte)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode generateRandom(uint8_t *output, size_t length);

/**
 * @brief Mevcut token'ları listele
 *
 * @param labels Çıktı: Token etiketleri dizisi (max 10 token)
 * @param count Çıktı: Bulunan token sayısı
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode listTokens(char labels[][64], size_t &count);

/**
 * @brief Mevcut anahtarları listele
 *
 * @param labels Çıktı: Anahtar etiketleri dizisi (max 50 key)
 * @param count Çıktı: Bulunan anahtar sayısı
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode listKeys(char labels[][64], size_t &count);
} // LCOV_EXCL_LINE

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // SOFTHSM_H
