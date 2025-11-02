/**
 * @file encryption.h
 * @brief Seyahat Gideri Takibi - Şifreleme Fonksiyonları
 * 
 * Bu dosya, veri şifreleme ve hash fonksiyonlarının bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <cstdint>
#include <cstddef>
#include "export.h"

namespace TravelExpense {

    /**
     * @namespace Encryption
     * @brief Şifreleme fonksiyonları modülü
     */
    namespace Encryption {
        /**
         * @brief SHA-256 hash hesapla
         * 
         * @param input Hash'lenecek veri
         * @param inputLen Veri uzunluğu
         * @param output Hash çıktısı (32 byte - 64 karakter hex string)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool sha256Hash(const void* input, size_t inputLen, char* output);

        /**
         * @brief Salt oluştur (rastgele)
         * 
         * @param salt Salt çıktısı (32 byte - 64 karakter hex string)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool generateSalt(char* salt);

        /**
         * @brief Şifreyi hash'le (SHA-256 + Salt)
         * 
         * @param password Şifre
         * @param salt Salt değeri
         * @param hash Hash çıktısı (64 karakter hex string)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool hashPassword(const char* password, const char* salt, char* hash);

        /**
         * @brief Şifre doğrulama
         * 
         * @param password Girilen şifre
         * @param salt Salt değeri
         * @param storedHash Saklanan hash değeri
         * @return true Şifre doğru, false Yanlış
         */
        TRAVELEXPENSE_API bool verifyPassword(const char* password, const char* salt, const char* storedHash);

        /**
         * @brief AES-256 ile veri şifreleme (basit implementasyon - başlangıç)
         * 
         * @param plaintext Şifrelenecek veri
         * @param plaintextLen Veri uzunluğu
         * @param key Şifreleme anahtarı (32 byte)
         * @param iv Initialization Vector (16 byte)
         * @param ciphertext Şifrelenmiş veri çıktısı
         * @param ciphertextLen Şifrelenmiş veri uzunluğu (çıktı)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool encryptAES256(const void* plaintext, size_t plaintextLen,
                                            const uint8_t* key, const uint8_t* iv,
                                            void* ciphertext, size_t& ciphertextLen);

        /**
         * @brief AES-256 ile veri şifre çözme
         * 
         * @param ciphertext Şifrelenmiş veri
         * @param ciphertextLen Şifrelenmiş veri uzunluğu
         * @param key Şifreleme anahtarı (32 byte)
         * @param iv Initialization Vector (16 byte)
         * @param plaintext Şifre çözülmüş veri çıktısı
         * @param plaintextLen Şifre çözülmüş veri uzunluğu (çıktı)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool decryptAES256(const void* ciphertext, size_t ciphertextLen,
                                            const uint8_t* key, const uint8_t* iv,
                                            void* plaintext, size_t& plaintextLen);
    }

} // namespace TravelExpense

#endif // ENCRYPTION_H

