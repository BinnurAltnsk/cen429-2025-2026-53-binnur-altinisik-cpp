/**
 * @file fingerprinting.h
 * @brief Seyahat Gideri Takibi - Cihaz ve Uygulama Fingerprinting
 *
 * Bu dosya, Dinamik Varlıkların Korunması gereksinimlerini karşılar:
 * - Cihaz fingerprint oluşturma
 * - Uygulama fingerprint oluşturma
 * - Dinamik anahtar yönetimi
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef FINGERPRINTING_H
#define FINGERPRINTING_H

#include "commonTypes.h"
#include "export.h"
#include <cstdint>
#include <cstddef>

namespace TravelExpense { // LCOV_EXCL_LINE

/**
 * @namespace Fingerprinting
 * @brief Cihaz ve uygulama fingerprinting modülü
 *
 * Bu modül, cihaz ve uygulama fingerprint'lerinin oluşturulması,
 * doğrulanması ve dinamik anahtar yönetimi işlemlerini sağlar.
 * Dinamik varlıkların korunması gereksinimlerini karşılar.
 */
namespace Fingerprinting {

// ============================================
// CİHAZ FİNGERPRİNTİNG
// ============================================

/**
 * @brief Cihaz fingerprint'ini oluştur
 *
 * MAC adresi, sistem bilgileri ve donanım özelliklerini kullanarak
 * benzersiz bir cihaz fingerprint'i oluşturur.
 *
 * @param fingerprint Cihaz fingerprint çıktısı (64 karakter hex string)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode generateDeviceFingerprint(char *fingerprint);

/**
 * @brief Cihaz fingerprint'ini doğrula
 *
 * Beklenen fingerprint ile mevcut cihaz fingerprint'ini karşılaştırır.
 *
 * @param expectedFingerprint Beklenen cihaz fingerprint'i
 * @return ErrorCode Başarı durumu (SUCCESS = geçerli, ERROR = geçersiz)
 */
TRAVELEXPENSE_API ErrorCode verifyDeviceFingerprint(const char *expectedFingerprint);

// ============================================
// UYGULAMA FİNGERPRİNTİNG
// ============================================

/**
 * @brief Uygulama fingerprint'ini oluştur
 *
 * Executable dosyasının hash'ini, sürüm bilgisini, build zamanını
 * ve diğer uygulama özelliklerini kullanarak benzersiz bir uygulama
 * fingerprint'i oluşturur.
 *
 * @param fingerprint Uygulama fingerprint çıktısı (64 karakter hex string)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode generateApplicationFingerprint(char *fingerprint);

/**
 * @brief Uygulama fingerprint'ini doğrula
 *
 * Beklenen fingerprint ile mevcut uygulama fingerprint'ini karşılaştırır.
 *
 * @param expectedFingerprint Beklenen uygulama fingerprint'i
 * @return ErrorCode Başarı durumu (SUCCESS = geçerli, ERROR = geçersiz)
 */
TRAVELEXPENSE_API ErrorCode verifyApplicationFingerprint(const char *expectedFingerprint);

/**
 * @brief Birleşik fingerprint oluştur
 *
 * Cihaz ve uygulama fingerprint'lerini birleştirerek tek bir fingerprint oluşturur.
 * Bu, cihaz-uygulama bağlantısı için kullanılabilir.
 *
 * @param fingerprint Birleşik fingerprint çıktısı (64 karakter hex string)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode generateCombinedFingerprint(char *fingerprint);

// ============================================
// DİNAMİK ANAHTAR YÖNETİMİ
// ============================================

/**
 * @brief Dinamik anahtar oluştur
 *
 * Cihaz ve uygulama fingerprint'lerini kullanarak dinamik bir anahtar oluşturur.
 * Bu anahtar, cihaz ve uygulama bazlı şifreleme için kullanılabilir.
 *
 * @param fingerprint Cihaz veya uygulama fingerprint'i
 * @param key Dinamik anahtar çıktısı (32 byte)
 * @param keyLen Anahtar uzunluğu (32)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode generateDynamicKey(const char *fingerprint,
    uint8_t *key, size_t keyLen);

/**
 * @brief Cihaz bazlı anahtar oluştur
 *
 * Cihaz fingerprint'ini kullanarak dinamik bir anahtar oluşturur.
 *
 * @param key Dinamik anahtar çıktısı (32 byte)
 * @param keyLen Anahtar uzunluğu (32)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode generateDeviceBasedKey(uint8_t *key, size_t keyLen);

/**
 * @brief Uygulama bazlı anahtar oluştur
 *
 * Uygulama fingerprint'ini kullanarak dinamik bir anahtar oluşturur.
 *
 * @param key Dinamik anahtar çıktısı (32 byte)
 * @param keyLen Anahtar uzunluğu (32)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode generateApplicationBasedKey(uint8_t *key, size_t keyLen);

/**
 * @brief Birleşik anahtar oluştur
 *
 * Cihaz ve uygulama fingerprint'lerini birleştirerek dinamik bir anahtar oluşturur.
 * Bu anahtar, cihaz-uygulama bağlantısı için şifreleme anahtarı olarak kullanılabilir.
 *
 * @param key Birleşik dinamik anahtar çıktısı (32 byte)
 * @param keyLen Anahtar uzunluğu (32)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API ErrorCode generateCombinedKey(uint8_t *key, size_t keyLen);

} // namespace Fingerprinting

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // FINGERPRINTING_H
