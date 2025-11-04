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

        /**
         * @brief HMAC-SHA256 hesapla (Message Authentication Code)
         * 
         * @param key HMAC anahtarı
         * @param keyLen Anahtar uzunluğu
         * @param message Mesaj
         * @param messageLen Mesaj uzunluğu
         * @param output HMAC çıktısı (32 byte - 64 karakter hex string)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool hmacSHA256(const uint8_t* key, size_t keyLen,
                                         const void* message, size_t messageLen,
                                         char* output);

        /**
         * @brief PBKDF2 key derivation (Password-Based Key Derivation Function 2)
         * 
         * @param password Şifre
         * @param passwordLen Şifre uzunluğu
         * @param salt Salt değeri
         * @param saltLen Salt uzunluğu
         * @param iterations İterasyon sayısı (önerilen: 10000+)
         * @param keyLen İstenen anahtar uzunluğu (byte)
         * @param output Türetilen anahtar çıktısı
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool pbkdf2(const char* password, size_t passwordLen,
                                     const uint8_t* salt, size_t saltLen,
                                     uint32_t iterations, size_t keyLen,
                                     uint8_t* output);

        /**
         * @brief Güvenli rastgele byte dizisi oluştur
         * 
         * @param output Çıktı buffer'ı
         * @param length İstenen uzunluk (byte)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool generateRandomBytes(uint8_t* output, size_t length);

        /**
         * @brief IV (Initialization Vector) oluştur (16 byte - AES block size)
         * 
         * @param iv IV çıktısı (16 byte)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool generateIV(uint8_t* iv);

        /**
         * @brief Constant-time string karşılaştırma (timing attack koruması)
         * 
         * @param a İlk string
         * @param b İkinci string
         * @param length Karşılaştırılacak uzunluk
         * @return true Eşit, false Farklı
         */
        TRAVELEXPENSE_API bool constantTimeCompare(const char* a, const char* b, size_t length);

        /**
         * @brief Dosyayı AES-256-CBC ile şifrele
         * 
         * @param inputFile Giriş dosya yolu
         * @param outputFile Çıkış dosya yolu
         * @param key Şifreleme anahtarı (32 byte)
         * @param iv IV (16 byte), nullptr ise otomatik oluşturulur
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool encryptFile(const char* inputFile, const char* outputFile,
                                          const uint8_t* key, const uint8_t* iv = nullptr);

        /**
         * @brief Şifrelenmiş dosyayı çöz
         * 
         * @param inputFile Şifrelenmiş dosya yolu
         * @param outputFile Çözülmüş dosya yolu
         * @param key Şifreleme anahtarı (32 byte)
         * @param iv IV (16 byte), nullptr ise dosyadan okunur
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool decryptFile(const char* inputFile, const char* outputFile,
                                          const uint8_t* key, const uint8_t* iv = nullptr);

        /**
         * @brief Whitebox DES ile veri şifreleme
         * 
         * Whitebox kriptografi, anahtarın kod içine gömülü olduğu ve reverse engineering'i
         * zorlaştıran bir tekniktir. Lookup table'lar kullanarak anahtar bilgisini maskeleyen
         * bir implementasyondur.
         * 
         * @param plaintext Şifrelenecek veri
         * @param plaintextLen Veri uzunluğu (8 byte'un katı olmalı)
         * @param ciphertext Şifrelenmiş veri çıktısı
         * @param ciphertextLen Şifrelenmiş veri uzunluğu (plaintextLen ile aynı)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool encryptWhiteboxDES(const void* plaintext, size_t plaintextLen,
                                                 void* ciphertext, size_t& ciphertextLen);

        /**
         * @brief Whitebox DES ile veri şifre çözme
         * 
         * @param ciphertext Şifrelenmiş veri
         * @param ciphertextLen Şifrelenmiş veri uzunluğu (8 byte'un katı olmalı)
         * @param plaintext Şifre çözülmüş veri çıktısı
         * @param plaintextLen Şifre çözülmüş veri uzunluğu (ciphertextLen ile aynı)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool decryptWhiteboxDES(const void* ciphertext, size_t ciphertextLen,
                                                 void* plaintext, size_t& plaintextLen);

        /**
         * @brief Dosyayı Whitebox DES ile şifrele
         * 
         * @param inputFile Giriş dosya yolu
         * @param outputFile Çıkış dosya yolu
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool encryptFileWhiteboxDES(const char* inputFile, const char* outputFile);

        /**
         * @brief Whitebox DES ile şifrelenmiş dosyayı çöz
         * 
         * @param inputFile Şifrelenmiş dosya yolu
         * @param outputFile Çözülmüş dosya yolu
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool decryptFileWhiteboxDES(const char* inputFile, const char* outputFile);

        // ============================================
        // WHITEBOX AES IMPLEMENTATION
        // ============================================

        /**
         * @brief Whitebox AES ile veri şifreleme
         * 
         * Whitebox kriptografi, anahtarın kod içine gömülü olduğu ve reverse engineering'i
         * zorlaştıran bir tekniktir. Lookup table'lar kullanarak anahtar bilgisini maskeleyen
         * bir implementasyondur. Bu implementasyon, embedded key ile çalışır ve key schedule
         * whitening teknikleri kullanır.
         * 
         * @param plaintext Şifrelenecek veri
         * @param plaintextLen Veri uzunluğu (16 byte'un katı olmalı)
         * @param ciphertext Şifrelenmiş veri çıktısı
         * @param ciphertextLen Şifrelenmiş veri uzunluğu (plaintextLen ile aynı)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool encryptWhiteboxAES(const void* plaintext, size_t plaintextLen,
                                                  void* ciphertext, size_t& ciphertextLen);

        /**
         * @brief Whitebox AES ile veri şifre çözme
         * 
         * @param ciphertext Şifrelenmiş veri
         * @param ciphertextLen Şifrelenmiş veri uzunluğu (16 byte'un katı olmalı)
         * @param plaintext Şifre çözülmüş veri çıktısı
         * @param plaintextLen Şifre çözülmüş veri uzunluğu (ciphertextLen ile aynı)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool decryptWhiteboxAES(const void* ciphertext, size_t ciphertextLen,
                                                  void* plaintext, size_t& plaintextLen);

        /**
         * @brief Dosyayı Whitebox AES ile şifrele
         * 
         * @param inputFile Giriş dosya yolu
         * @param outputFile Çıkış dosya yolu
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool encryptFileWhiteboxAES(const char* inputFile, const char* outputFile);

        /**
         * @brief Whitebox AES ile şifrelenmiş dosyayı çöz
         * 
         * @param inputFile Şifrelenmiş dosya yolu
         * @param outputFile Çözülmüş dosya yolu
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool decryptFileWhiteboxAES(const char* inputFile, const char* outputFile);
    }

} // namespace TravelExpense

#endif // ENCRYPTION_H

