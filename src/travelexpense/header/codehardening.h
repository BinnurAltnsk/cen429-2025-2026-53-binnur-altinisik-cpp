/**
 * @file codehardening.h
 * @brief Seyahat Gideri Takibi - Kod Sertleştirme Fonksiyonları
 *
 * Bu dosya, kod sertleştirme tekniklerinin bildirimlerini içerir:
 * - Opaque loops (Kontrol akışı gizleme)
 * - String obfuscation (String gizleme)
 * - Fonksiyon gizleme
 * - Opaque boolean değişkenleri
 * - Dead branches (Sahte ölüm dallar)
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef CODEHARDENING_H
#define CODEHARDENING_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include "export.h"

namespace TravelExpense { // LCOV_EXCL_LINE

/**
 * @namespace CodeHardening
 * @brief Kod sertleştirme teknikleri modülü
 *
 * Bu modül, kod sertleştirme tekniklerini sağlar: opaque loops (kontrol akışı gizleme),
 * string obfuscation (string gizleme), fonksiyon gizleme ve opaque boolean değişkenleri.
 * Reverse engineering ve code analysis'a karşı koruma sağlar.
 */
namespace CodeHardening {

// ============================================================================
// OPAQUE LOOPS - Kontrol Akışı Gizleme
// ============================================================================

/**
 * @brief Opaque loop counter oluştur
 *
 * Döngü sayısını gizlemek için kullanılan opaque counter
 *
 * @param base Base değer
 * @param offset Offset değeri
 * @return Obfuscated counter değeri
 */
TRAVELEXPENSE_API uint32_t createOpaqueCounter(uint32_t base, uint32_t offset);

/**
 * @brief Opaque loop condition kontrolü
 *
 * Döngü koşulunu gizlemek için kullanılan karmaşık kontrol fonksiyonu
 *
 * @param current Mevcut değer
 * @param limit Limit değeri
 * @return true Devam et, false Durdur
 */
TRAVELEXPENSE_API bool checkOpaqueLoopCondition(uint32_t current, uint32_t limit);

/**
 * @brief Opaque loop increment
 *
 * Döngü değişkenini artırmak için kullanılan opaque fonksiyon
 *
 * @param value Mevcut değer
 * @param increment Artış miktarı (genellikle gizli)
 * @return Artırılmış değer
 */
TRAVELEXPENSE_API uint32_t opaqueIncrement(uint32_t value, uint32_t increment);

/**
 * @brief Opaque loop decrement
 *
 * Döngü değişkenini azaltmak için kullanılan opaque fonksiyon
 *
 * @param value Mevcut değer
 * @param decrement Azalış miktarı (genellikle gizli)
 * @return Azaltılmış değer
 */
TRAVELEXPENSE_API uint32_t opaqueDecrement(uint32_t value, uint32_t decrement);

// ============================================================================
// STRING OBFUSCATION - String Gizleme
// ============================================================================

/**
 * @brief String'i obfuscate et (şifrele)
 *
 * XOR tabanlı string obfuscation
 *
 * @param input Orijinal string
 * @param inputLen String uzunluğu
 * @param key Obfuscation anahtarı
 * @param outputLen Çıktı uzunluğu (key ile XOR sonrası aynı kalır)
 * @return Obfuscated string (heap'te, kullanıcı free etmeli)
 */
TRAVELEXPENSE_API uint8_t *obfuscateString(const char *input, size_t inputLen, uint8_t key, size_t &outputLen);

/**
 * @brief Obfuscated string'i deobfuscate et (şifre çöz)
 *
 * XOR tabanlı string deobfuscation
 *
 * @param obfuscated Obfuscated string
 * @param obfuscatedLen Obfuscated string uzunluğu
 * @param key Obfuscation anahtarı
 * @param outputLen Çıktı uzunluğu
 * @return Deobfuscated string (heap'te, kullanıcı free etmeli)
 */
TRAVELEXPENSE_API char *deobfuscateString(const uint8_t *obfuscated, size_t obfuscatedLen, uint8_t key, size_t &outputLen);

/**
 * @brief String'i runtime'da deobfuscate et ve kullan
 *
 * Obfuscated string'i decode edip geçici buffer'da saklar
 *
 * @param obfuscated Obfuscated string
 * @param obfuscatedLen Obfuscated string uzunluğu
 * @param key Obfuscation anahtarı
 * @param output Buffer (en az obfuscatedLen + 1 byte olmalı)
 * @param outputSize Buffer boyutu
 * @return true Başarılı, false Hata
 */
TRAVELEXPENSE_API bool getObfuscatedString(const uint8_t *obfuscated, size_t obfuscatedLen, uint8_t key, char *output, size_t outputSize);

/**
 * @brief Obfuscated string için güvenli temizlik
 *
 * Obfuscated string'i güvenli şekilde siler
 *
 * @param obfuscated Obfuscated string
 * @param obfuscatedLen String uzunluğu
 */
TRAVELEXPENSE_API void secureClearObfuscatedString(uint8_t *obfuscated, size_t obfuscatedLen);

// ============================================================================
// FUNCTION HIDING - Fonksiyon Gizleme
// ============================================================================

/**
 * @brief Fonksiyon pointer'ını obfuscate et
 *
 * @param funcPtr Fonksiyon pointer'ı
 * @return Obfuscated pointer
 */
TRAVELEXPENSE_API void *obfuscateFunctionPointer(void *funcPtr);

/**
 * @brief Obfuscated fonksiyon pointer'ını deobfuscate et
 *
 * @param obfuscated Obfuscated pointer
 * @return Orijinal fonksiyon pointer'ı
 */
TRAVELEXPENSE_API void *deobfuscateFunctionPointer(void *obfuscated);

// ============================================================================
// OPAQUE BOOLEAN - Opaque Boolean Değişkenleri
// ============================================================================

/**
 * @brief Boolean değerini opaque flag'e dönüştür
 *
 * @param value Boolean değer
 * @return Opaque flag (true için karmaşık değer, false için başka)
 */
TRAVELEXPENSE_API uint32_t createOpaqueBoolean(bool value);

/**
 * @brief Opaque flag'i boolean'a dönüştür
 *
 * @param opaqueFlag Opaque flag
 * @return Boolean değer
 */
TRAVELEXPENSE_API bool getOpaqueBoolean(uint32_t opaqueFlag);

/**
 * @brief İki opaque flag'i karşılaştır
 *
 * @param flag1 İlk flag
 * @param flag2 İkinci flag
 * @return true Eşit, false Farklı
 */
TRAVELEXPENSE_API bool compareOpaqueFlags(uint32_t flag1, uint32_t flag2);

/**
 * @brief Opaque flag kombinasyonu oluştur
 *
 * Birden fazla boolean değerini tek opaque flag'te birleştirir
 *
 * @param values Boolean değer dizisi
 * @param count Değer sayısı (maks 32)
 * @return Opaque flag kombinasyonu
 */
TRAVELEXPENSE_API uint32_t combineOpaqueFlags(const bool *values, size_t count);

/**
 * @brief Opaque flag kombinasyonundan boolean değeri al
 *
 * @param combinedFlag Kombine flag
 * @param index İndeks (0-31)
 * @return Boolean değer
 */
TRAVELEXPENSE_API bool getOpaqueFlagAt(uint32_t combinedFlag, size_t index);

// ============================================================================
// HELPER FUNCTIONS - Yardımcı Fonksiyonlar
// ============================================================================

/**
 * @brief Opaque XOR işlemi
 *
 * XOR işlemini gizlemek için kullanılan karmaşık fonksiyon
 *
 * @param a İlk değer
 * @param b İkinci değer
 * @return XOR sonucu
 */
TRAVELEXPENSE_API uint32_t opaqueXOR(uint32_t a, uint32_t b);

/**
 * @brief Opaque AND işlemi
 *
 * @param a İlk değer
 * @param b İkinci değer
 * @return AND sonucu
 */
TRAVELEXPENSE_API uint32_t opaqueAND(uint32_t a, uint32_t b);

/**
 * @brief Opaque OR işlemi
 *
 * @param a İlk değer
 * @param b İkinci değer
 * @return OR sonucu
 */
TRAVELEXPENSE_API uint32_t opaqueOR(uint32_t a, uint32_t b);

/**
 * @brief Opaque NOT işlemi
 *
 * @param a Değer
 * @return NOT sonucu
 */
TRAVELEXPENSE_API uint32_t opaqueNOT(uint32_t a);

// ============================================================================
// DEAD BRANCHES - Sahte Ölüm Dallar
// ============================================================================

/**
 * @brief Opaque predicate - Her zaman false döner (compile-time'da anlaşılamaz)
 *
 * Bu fonksiyon, dead branch oluşturmak için kullanılır. Runtime'da
 * her zaman false döner, ancak compiler bunu optimize edemez.
 * Reverse engineering'i zorlaştırmak için karmaşık hesaplamalar içerir.
 *
 * @param value Giriş değeri (karmaşık hesaplama için kullanılır)
 * @return Her zaman false (ancak compile-time'da anlaşılamaz)
 */
TRAVELEXPENSE_API bool createOpaquePredicateFalse(uint32_t value);

/**
 * @brief Opaque predicate - Her zaman true döner (compile-time'da anlaşılamaz)
 *
 * Bu fonksiyon, dead branch oluşturmak için kullanılır. Runtime'da
 * her zaman true döner, ancak compiler bunu optimize edemez.
 *
 * @param value Giriş değeri (karmaşık hesaplama için kullanılır)
 * @return Her zaman true (ancak compile-time'da anlaşılamaz)
 */
TRAVELEXPENSE_API bool createOpaquePredicateTrue(uint32_t value);

/**
 * @brief Opaque predicate - Karmaşık hesaplama ile false döner
 *
 * Matematiksel olarak imkansız bir koşulu kontrol eder.
 * Örnek: x^2 + 1 == 0 (gerçek sayılarda imkansız)
 *
 * @param x Giriş değeri
 * @return Her zaman false
 */
TRAVELEXPENSE_API bool opaquePredicateImpossible(uint32_t x);

/**
 * @brief Opaque predicate - Karmaşık hesaplama ile true döner
 *
 * Matematiksel olarak her zaman doğru olan bir koşulu kontrol eder.
 * Örnek: x^2 >= 0 (her zaman doğru)
 *
 * @param x Giriş değeri
 * @return Her zaman true
 */
TRAVELEXPENSE_API bool opaquePredicateAlwaysTrue(uint32_t x);

/**
 * @brief Sahte fonksiyon - Dead branch içinde çağrılır
 *
 * Bu fonksiyon asla çağrılmaz, ancak dead branch içinde
 * bulunur ve reverse engineering'i zorlaştırır.
 *
 * @param param1 İlk parametre
 * @param param2 İkinci parametre
 * @return Sahte değer
 */
TRAVELEXPENSE_API uint32_t dummyFunction(uint32_t param1, uint32_t param2);

/**
 * @brief Sahte işlem - Dead branch içinde çalıştırılır
 *
 * Bu fonksiyon asla çalışmaz, ancak dead branch içinde
 * bulunur ve kod analizini zorlaştırır.
 *
 * @param data İşlenecek veri
 * @param size Veri boyutu
 */
TRAVELEXPENSE_API void dummyOperation(void *data, size_t size);

} // namespace CodeHardening

} // namespace TravelExpense // LCOV_EXCL_LINE

// ============================================================================
// MACROS - Fonksiyon Gizleme Makroları
// ============================================================================

/**
 * @brief Fonksiyon ismini gizlemek için makro
 *
 * Kullanım:
 * OBFUSCATE_FUNC_NAME(functionName) -> obfuscated_functionName
 */
#define OBFUSCATE_FUNC_NAME(name) \
  TravelExpense::CodeHardening::obfuscateFunctionPointer(reinterpret_cast<void*>(name))

/**
 * @brief Obfuscated fonksiyonu çağırmak için makro
 *
 * Kullanım:
 * CALL_OBFUSCATED_FUNC(returnType, obfuscatedPtr, args...)
 */
#define CALL_OBFUSCATED_FUNC(retType, obfPtr, ...) \
  (reinterpret_cast<retType(*)>(TravelExpense::CodeHardening::deobfuscateFunctionPointer(obfPtr)))(__VA_ARGS__)

/**
 * @brief Opaque loop için yardımcı makro
 *
 * Kullanım:
 * OPAQUE_LOOP(var, limit, increment) { ... }
 */
#define OPAQUE_LOOP(var, limit, increment) \
  for (uint32_t var = TravelExpense::CodeHardening::createOpaqueCounter(0, increment); \
       TravelExpense::CodeHardening::checkOpaqueLoopCondition(var, limit); \
       var = TravelExpense::CodeHardening::opaqueIncrement(var, increment))

/**
 * @brief Opaque string için yardımcı makro
 *
 * Obfuscated string'i runtime'da decode edip kullanır
 *
 * Kullanım:
 * char buffer[256];
 * OBFUSCATED_STRING(obfuscated_data, obfuscated_len, key, buffer, sizeof(buffer))
 */
#define OBFUSCATED_STRING(obf, obfLen, key, buf, bufSize) \
  TravelExpense::CodeHardening::getObfuscatedString(obf, obfLen, key, buf, bufSize)

/**
 * @brief Dead branch oluşturmak için yardımcı makro
 *
 * Opaque predicate ile asla erişilemeyen kod bloğu oluşturur.
 * Reverse engineering'i zorlaştırmak için kullanılır.
 *
 * Kullanım:
 * DEAD_BRANCH(value) {
 *   // Bu kod asla çalışmaz
 *   dummyFunction(1, 2);
 * }
 */
#define DEAD_BRANCH(value) \
  if (TravelExpense::CodeHardening::createOpaquePredicateFalse(value))

/**
 * @brief Always-true dead branch oluşturmak için yardımcı makro
 *
 * Opaque predicate ile her zaman true dönen ama karmaşık kod bloğu oluşturur.
 *
 * Kullanım:
 * ALWAYS_TRUE_BRANCH(value) {
 *   // Bu kod her zaman çalışır, ancak karmaşık hesaplamalar içerir
 *   dummyOperation(data, size);
 * }
 */
#define ALWAYS_TRUE_BRANCH(value) \
  if (TravelExpense::CodeHardening::createOpaquePredicateTrue(value))

#endif // CODEHARDENING_H
