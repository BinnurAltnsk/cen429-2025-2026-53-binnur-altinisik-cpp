/**
 * @file codehardening.cpp
 * @brief Seyahat Gideri Takibi - Kod Sertleştirme Fonksiyonları
 *
 * Bu dosya, kod sertleştirme tekniklerinin implementasyonunu içerir:
 * - Opaque loops (Kontrol akışı gizleme)
 * - String obfuscation (String gizleme)
 * - Fonksiyon gizleme
 * - Opaque boolean değişkenleri
 * - Dead branches (Sahte ölüm dallar)
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#include "codehardening.h"
#include "security.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>

// Platform-specific includes
#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #define NOMINMAX
  #include <windows.h>
#else
  #include <unistd.h>
#endif

/**
 * @namespace TravelExpense
 * @brief Seyahat Gideri Takibi uygulaması ana namespace'i
 */
namespace TravelExpense {
/**
 * @namespace CodeHardening
 * @brief Kod sertleştirme teknikleri modülü implementasyonu
 */
namespace CodeHardening {

// ============================================================================
// OPAQUE LOOPS - Kontrol Akışı Gizleme
// ============================================================================

/**
 * @var OPAQUE_MAGIC_1
 * @brief Opaque loop obfuscation için magic constant 1
 *
 * Bu magic constant, opaque loop'ların kontrol akışını gizlemek için kullanılır.
 * XOR işlemleri ile counter değerlerini obfuscate eder.
 * Değer: 0x5A5A5A5A
 *
 * @note Compile-time randomization önerilir (her build'de farklı değer).
 * @see createOpaqueCounter Opaque counter oluşturma (içeride kullanılır)
 * @see OPAQUE_MAGIC_2 Magic constant 2
 * @see OPAQUE_MAGIC_3 Magic constant 3
 * @see OPAQUE_MAGIC_4 Magic constant 4
 */
static const uint32_t OPAQUE_MAGIC_1 = 0x5A5A5A5A;

/**
 * @var OPAQUE_MAGIC_2
 * @brief Opaque loop obfuscation için magic constant 2
 *
 * Bu magic constant, opaque loop'ların kontrol akışını gizlemek için kullanılır.
 * XOR işlemleri ile counter değerlerini obfuscate eder.
 * Değer: 0xA5A5A5A5 (OPAQUE_MAGIC_1'in bitwise complement'i)
 *
 * @note Compile-time randomization önerilir (her build'de farklı değer).
 * @see createOpaqueCounter Opaque counter oluşturma (içeride kullanılır)
 * @see OPAQUE_MAGIC_1 Magic constant 1
 * @see OPAQUE_MAGIC_3 Magic constant 3
 * @see OPAQUE_MAGIC_4 Magic constant 4
 */
static const uint32_t OPAQUE_MAGIC_2 = 0xA5A5A5A5;

/**
 * @var OPAQUE_MAGIC_3
 * @brief Opaque loop obfuscation için magic constant 3
 *
 * Bu magic constant, opaque loop'ların kontrol akışını gizlemek için kullanılır.
 * Addition işlemleri ile counter değerlerini obfuscate eder.
 * Değer: 0x12345678
 *
 * @note Compile-time randomization önerilir (her build'de farklı değer).
 * @see createOpaqueCounter Opaque counter oluşturma (içeride kullanılır)
 * @see OPAQUE_MAGIC_1 Magic constant 1
 * @see OPAQUE_MAGIC_2 Magic constant 2
 * @see OPAQUE_MAGIC_4 Magic constant 4
 */
static const uint32_t OPAQUE_MAGIC_3 = 0x12345678;

/**
 * @var OPAQUE_MAGIC_4
 * @brief Opaque loop obfuscation için magic constant 4
 *
 * Bu magic constant, opaque loop'ların kontrol akışını gizlemek için kullanılır.
 * Addition işlemleri ile counter değerlerini obfuscate eder.
 * Değer: 0x87654321 (OPAQUE_MAGIC_3'ün bitwise reverse'i)
 *
 * @note Compile-time randomization önerilir (her build'de farklı değer).
 * @see createOpaqueCounter Opaque counter oluşturma (içeride kullanılır)
 * @see OPAQUE_MAGIC_1 Magic constant 1
 * @see OPAQUE_MAGIC_2 Magic constant 2
 * @see OPAQUE_MAGIC_3 Magic constant 3
 */
static const uint32_t OPAQUE_MAGIC_4 = 0x87654321;

uint32_t createOpaqueCounter(uint32_t base, uint32_t offset) {
  // Base ve offset'i karmaşık şekilde birleştir
  uint32_t counter = base;
  counter ^= OPAQUE_MAGIC_1;
  counter += offset;
  counter ^= OPAQUE_MAGIC_2;
  counter = (counter << 1) | (counter >> 31); // Rotate left
  counter += OPAQUE_MAGIC_3;
  return counter;
}

bool checkOpaqueLoopCondition(uint32_t current, uint32_t limit) {
  // Karmaşık koşul kontrolü
  uint32_t obfuscatedCurrent = current;
  obfuscatedCurrent ^= OPAQUE_MAGIC_2;
  obfuscatedCurrent = (obfuscatedCurrent << 3) | (obfuscatedCurrent >> 29); // Rotate left 3
  obfuscatedCurrent -= OPAQUE_MAGIC_1;
  uint32_t obfuscatedLimit = limit;
  obfuscatedLimit ^= OPAQUE_MAGIC_3;
  obfuscatedLimit = (obfuscatedLimit << 5) | (obfuscatedLimit >> 27); // Rotate left 5
  obfuscatedLimit += OPAQUE_MAGIC_4;
  // Orijinal karşılaştırmayı gizle
  // current < limit kontrolü
  uint32_t diff = obfuscatedCurrent - obfuscatedLimit;
  diff ^= OPAQUE_MAGIC_1;
  diff = (diff << 7) | (diff >> 25); // Rotate left 7
  // Sign bit kontrolü (carry flag simulation)
  bool result = (diff & 0x80000000) == 0;
  result ^= (obfuscatedCurrent < obfuscatedLimit);
  // Orijinal karşılaştırma - current < limit kontrolü
  // Obfuscation işlemleri sadece görünürlüğü gizlemek için,
  // gerçek karşılaştırma current < limit olmalı
  return current < limit;
}

uint32_t opaqueIncrement(uint32_t value, uint32_t increment) {
  // Karmaşık increment işlemi
  uint32_t result = value;
  result ^= OPAQUE_MAGIC_1;
  result += increment;
  result ^= OPAQUE_MAGIC_2;
  result = (result << 2) | (result >> 30); // Rotate left 2
  result -= OPAQUE_MAGIC_3;
  // Orijinal increment
  uint32_t original = value;
  original ^= OPAQUE_MAGIC_2;
  original = (original >> 2) | (original << 30); // Rotate right 2 (inverse)
  original += OPAQUE_MAGIC_3;
  return original + increment;
}

uint32_t opaqueDecrement(uint32_t value, uint32_t decrement) {
  // Karmaşık decrement işlemi
  uint32_t result = value;
  result ^= OPAQUE_MAGIC_3;
  result -= decrement;
  result ^= OPAQUE_MAGIC_4;
  result = (result << 4) | (result >> 28); // Rotate left 4
  result += OPAQUE_MAGIC_1;
  // Orijinal decrement
  uint32_t original = value;
  original ^= OPAQUE_MAGIC_4;
  original = (original >> 4) | (original << 28); // Rotate right 4 (inverse)
  original -= OPAQUE_MAGIC_1;
  return original - decrement;
}

// ============================================================================
// STRING OBFUSCATION - String Gizleme
// ============================================================================

uint8_t *obfuscateString(const char *input, size_t inputLen, uint8_t key, size_t &outputLen) {
  if (!input || inputLen == 0) {
    outputLen = 0;
    return nullptr;
  }

  // XOR tabanlı obfuscation
  uint8_t *obfuscated = static_cast<uint8_t *>(malloc(inputLen));

  if (!obfuscated) {
    outputLen = 0;
    return nullptr;
  }

  // XOR ile şifrele
  for (size_t i = 0; i < inputLen; ++i) {
    obfuscated[i] = static_cast<uint8_t>(input[i]) ^ key;
    // Key'i değiştir (rolling key)
    key = static_cast<uint8_t>((key << 1) | (key >> 7)); // Rotate left
    key ^= static_cast<uint8_t>(i); // Position-dependent key
  }

  outputLen = inputLen;
  return obfuscated;
}

char *deobfuscateString(const uint8_t *obfuscated, size_t obfuscatedLen, uint8_t key, size_t &outputLen) {
  if (!obfuscated || obfuscatedLen == 0) {
    outputLen = 0;
    return nullptr;
  }

  // XOR tabanlı deobfuscation
  char *deobfuscated = static_cast<char *>(malloc(obfuscatedLen + 1)); // +1 for null terminator

  if (!deobfuscated) {
    outputLen = 0;
    return nullptr;
  }

  uint8_t currentKey = key;

  // XOR ile şifre çöz
  for (size_t i = 0; i < obfuscatedLen; ++i) {
    deobfuscated[i] = static_cast<char>(obfuscated[i] ^ currentKey);
    // Key'i değiştir (rolling key - aynı şekilde)
    currentKey = static_cast<uint8_t>((currentKey << 1) | (currentKey >> 7)); // Rotate left
    currentKey ^= static_cast<uint8_t>(i); // Position-dependent key
  }

  deobfuscated[obfuscatedLen] = '\0'; // Null terminator
  outputLen = obfuscatedLen;
  return deobfuscated;
}

bool getObfuscatedString(const uint8_t *obfuscated, size_t obfuscatedLen, uint8_t key, char *output, size_t outputSize) {
  if (!obfuscated || obfuscatedLen == 0 || !output || outputSize == 0) {
    return false;
  }

  if (outputSize < obfuscatedLen + 1) { // +1 for null terminator
    return false;
  }

  // XOR ile deobfuscate et
  uint8_t currentKey = key;

  for (size_t i = 0; i < obfuscatedLen; ++i) {
    output[i] = static_cast<char>(obfuscated[i] ^ currentKey);
    // Key'i değiştir (rolling key)
    currentKey = static_cast<uint8_t>((currentKey << 1) | (currentKey >> 7)); // Rotate left
    currentKey ^= static_cast<uint8_t>(i); // Position-dependent key
  }

  output[obfuscatedLen] = '\0'; // Null terminator
  return true;
}

void secureClearObfuscatedString(uint8_t *obfuscated, size_t obfuscatedLen) {
  if (!obfuscated || obfuscatedLen == 0) {
    return;
  }

  // Güvenli temizlik
  TravelExpense::Security::secureMemoryCleanup(obfuscated, obfuscatedLen);
  free(obfuscated);
}

// ============================================================================
// FUNCTION HIDING - Fonksiyon Gizleme
// ============================================================================

// Function pointer obfuscation key
static const uintptr_t FUNC_PTR_KEY = 0xDEADBEEFCAFEBABEULL;

void *obfuscateFunctionPointer(void *funcPtr) {
  if (!funcPtr) {
    return nullptr;
  }

  // Pointer'ı integer'a dönüştür ve XOR ile şifrele
  uintptr_t ptrValue = reinterpret_cast<uintptr_t>(funcPtr);
  ptrValue ^= FUNC_PTR_KEY;
  ptrValue = (ptrValue << 13) | (ptrValue >> (sizeof(uintptr_t) * 8 - 13)); // Rotate left 13
  return reinterpret_cast<void *>(ptrValue);
}

void *deobfuscateFunctionPointer(void *obfuscated) {
  if (!obfuscated) {
    return nullptr;
  }

  // Pointer'ı integer'a dönüştür ve XOR ile şifre çöz
  uintptr_t ptrValue = reinterpret_cast<uintptr_t>(obfuscated);
  ptrValue = (ptrValue >> 13) | (ptrValue << (sizeof(uintptr_t) * 8 - 13)); // Rotate right 13 (inverse)
  ptrValue ^= FUNC_PTR_KEY;
  return reinterpret_cast<void *>(ptrValue);
}

// ============================================================================
// OPAQUE BOOLEAN - Opaque Boolean Değişkenleri
// ============================================================================

// Opaque boolean magic values
static const uint32_t OPAQUE_TRUE = 0xFACE1234;
static const uint32_t OPAQUE_FALSE = 0xBEEF5678;

uint32_t createOpaqueBoolean(bool value) {
  // Boolean değerini karmaşık sayıya dönüştür
  if (value) {
    uint32_t result = OPAQUE_TRUE;
    result ^= OPAQUE_MAGIC_1;
    result = (result << 7) | (result >> 25); // Rotate left 7
    result += OPAQUE_MAGIC_2;
    return result;
  } else {
    uint32_t result = OPAQUE_FALSE;
    result ^= OPAQUE_MAGIC_3;
    result = (result << 11) | (result >> 21); // Rotate left 11
    result += OPAQUE_MAGIC_4;
    return result;
  }
}

bool getOpaqueBoolean(uint32_t opaqueFlag) {
  // Opaque flag'i boolean'a dönüştür
  uint32_t processed = opaqueFlag;
  processed -= OPAQUE_MAGIC_2;
  processed = (processed >> 7) | (processed << 25); // Rotate right 7 (inverse)
  processed ^= OPAQUE_MAGIC_1;

  // True kontrolü
  if (processed == OPAQUE_TRUE) {
    return true;
  }

  // False kontrolü
  processed = opaqueFlag;
  processed -= OPAQUE_MAGIC_4;
  processed = (processed >> 11) | (processed << 21); // Rotate right 11 (inverse)
  processed ^= OPAQUE_MAGIC_3;
  return processed == OPAQUE_FALSE ? false : true; // Default true for unknown values
}

bool compareOpaqueFlags(uint32_t flag1, uint32_t flag2) {
  // İki opaque flag'i karşılaştır
  uint32_t processed1 = flag1;
  processed1 ^= OPAQUE_MAGIC_1;
  processed1 = (processed1 << 5) | (processed1 >> 27); // Rotate left 5
  processed1 += OPAQUE_MAGIC_2;
  uint32_t processed2 = flag2;
  processed2 ^= OPAQUE_MAGIC_3;
  processed2 = (processed2 << 9) | (processed2 >> 23); // Rotate left 9
  processed2 += OPAQUE_MAGIC_4;
  // Karmaşık karşılaştırma
  uint32_t diff = processed1 - processed2;
  diff ^= OPAQUE_MAGIC_1;
  diff = (diff << 3) | (diff >> 29); // Rotate left 3
  // Orijinal karşılaştırma
  return flag1 == flag2;
}

uint32_t combineOpaqueFlags(const bool *values, size_t count) {
  if (!values || count == 0 || count > 32) {
    return 0;
  }

  // Boolean değerlerini bit flag'lerine dönüştür
  uint32_t combined = 0;

  for (size_t i = 0; i < count; ++i) {
    if (values[i]) {
      combined |= (1U << i);
    }
  }

  // Obfuscate et
  combined ^= OPAQUE_MAGIC_1;
  combined = (combined << 17) | (combined >> 15); // Rotate left 17
  combined += OPAQUE_MAGIC_2;
  combined ^= OPAQUE_MAGIC_3;
  return combined;
}

bool getOpaqueFlagAt(uint32_t combinedFlag, size_t index) {
  if (index >= 32) {
    return false;
  }

  // Deobfuscate et
  uint32_t deobfuscated = combinedFlag;
  deobfuscated ^= OPAQUE_MAGIC_3;
  deobfuscated -= OPAQUE_MAGIC_2;
  deobfuscated = (deobfuscated >> 17) | (deobfuscated << 15); // Rotate right 17 (inverse)
  deobfuscated ^= OPAQUE_MAGIC_1;
  // Bit kontrolü
  return (deobfuscated & (1U << index)) != 0;
}

// ============================================================================
// HELPER FUNCTIONS - Yardımcı Fonksiyonlar
// ============================================================================

uint32_t opaqueXOR(uint32_t a, uint32_t b) {
  // XOR işlemini gizle
  uint32_t obfA = a;
  obfA ^= OPAQUE_MAGIC_1;
  obfA = (obfA << 3) | (obfA >> 29); // Rotate left 3
  uint32_t obfB = b;
  obfB ^= OPAQUE_MAGIC_2;
  obfB = (obfB << 5) | (obfB >> 27); // Rotate left 5
  uint32_t result = obfA ^ obfB;
  result ^= OPAQUE_MAGIC_3;
  result = (result << 7) | (result >> 25); // Rotate left 7
  // Orijinal XOR - obfuscation sonrası orijinal sonucu döndür
  return a ^ b;
}

uint32_t opaqueAND(uint32_t a, uint32_t b) {
  // AND işlemini gizle
  uint32_t obfA = a;
  obfA ^= OPAQUE_MAGIC_2;
  obfA = (obfA << 11) | (obfA >> 21); // Rotate left 11
  uint32_t obfB = b;
  obfB ^= OPAQUE_MAGIC_4;
  obfB = (obfB << 13) | (obfB >> 19); // Rotate left 13
  uint32_t result = obfA & obfB;
  result ^= OPAQUE_MAGIC_1;
  result = (result << 9) | (result >> 23); // Rotate left 9
  // Orijinal AND
  return a & b;
}

uint32_t opaqueOR(uint32_t a, uint32_t b) {
  // OR işlemini gizle
  uint32_t obfA = a;
  obfA ^= OPAQUE_MAGIC_3;
  obfA = (obfA << 15) | (obfA >> 17); // Rotate left 15
  uint32_t obfB = b;
  obfB ^= OPAQUE_MAGIC_1;
  obfB = (obfB << 17) | (obfB >> 15); // Rotate left 17
  uint32_t result = obfA | obfB;
  result ^= OPAQUE_MAGIC_2;
  result = (result << 11) | (result >> 21); // Rotate left 11
  // Orijinal OR
  return a | b;
}

uint32_t opaqueNOT(uint32_t a) {
  // NOT işlemini gizle
  uint32_t obfA = a;
  obfA ^= OPAQUE_MAGIC_4;
  obfA = (obfA << 19) | (obfA >> 13); // Rotate left 19
  uint32_t result = ~obfA;
  result ^= OPAQUE_MAGIC_3;
  result = (result << 7) | (result >> 25); // Rotate left 7
  // Orijinal NOT
  return ~a;
}

// ============================================================================
// DEAD BRANCHES - Sahte Ölüm Dallar
// ============================================================================

/**
 * @var DEAD_BRANCH_MAGIC_1
 * @brief Dead branch obfuscation için magic constant 1
 */
static const uint32_t DEAD_BRANCH_MAGIC_1 = 0xCAFEBABE;

/**
 * @var DEAD_BRANCH_MAGIC_2
 * @brief Dead branch obfuscation için magic constant 2
 */
static const uint32_t DEAD_BRANCH_MAGIC_2 = 0xDEADBEEF;

bool createOpaquePredicateFalse(uint32_t value) {
  // Karmaşık hesaplama - her zaman false döner
  // Ancak compiler bunu optimize edemez çünkü karmaşık işlemler içerir
  // Obfuscated value
  uint32_t obfValue = value;
  obfValue ^= OPAQUE_MAGIC_1;
  obfValue = (obfValue << 7) | (obfValue >> 25); // Rotate left 7
  obfValue += OPAQUE_MAGIC_2;
  obfValue ^= OPAQUE_MAGIC_3;
  // Karmaşık karşılaştırma - her zaman false
  // x^2 + 1 == 0 (gerçek sayılarda imkansız)
  uint64_t square = static_cast<uint64_t>(obfValue) * static_cast<uint64_t>(obfValue);
  square += 1;
  // Obfuscated comparison
  uint32_t obfSquare = static_cast<uint32_t>(square & 0xFFFFFFFF);
  obfSquare ^= DEAD_BRANCH_MAGIC_1;
  obfSquare = (obfSquare << 11) | (obfSquare >> 21); // Rotate left 11
  obfSquare += DEAD_BRANCH_MAGIC_2;
  // Her zaman false döner (square + 1 hiçbir zaman 0 olamaz)
  // Ancak compiler bunu anlayamaz çünkü obfuscation var
  return (obfSquare == 0) && (square == 0);
}

bool createOpaquePredicateTrue(uint32_t value) {
  // Karmaşık hesaplama - her zaman true döner
  // Ancak compiler bunu optimize edemez
  // Obfuscated value
  uint32_t obfValue = value;
  obfValue ^= OPAQUE_MAGIC_2;
  obfValue = (obfValue << 13) | (obfValue >> 19); // Rotate left 13
  obfValue += OPAQUE_MAGIC_4;
  obfValue ^= OPAQUE_MAGIC_1;
  // Karmaşık karşılaştırma - her zaman true
  // x^2 >= 0 (her zaman doğru)
  uint64_t square = static_cast<uint64_t>(obfValue) * static_cast<uint64_t>(obfValue);
  // Obfuscated comparison
  uint32_t obfSquare = static_cast<uint32_t>(square & 0xFFFFFFFF);
  obfSquare ^= DEAD_BRANCH_MAGIC_2;
  obfSquare = (obfSquare << 17) | (obfSquare >> 15); // Rotate left 17
  obfSquare += DEAD_BRANCH_MAGIC_1;
  // Her zaman true döner (square >= 0 her zaman doğru)
  // Ancak compiler bunu anlayamaz
  return (obfSquare >= 0) || (square >= 0);
}

bool opaquePredicateImpossible(uint32_t x) {
  // Matematiksel olarak imkansız koşul: x^2 + 1 == 0
  // Gerçek sayılarda bu hiçbir zaman doğru olamaz
  // Obfuscated calculation
  uint32_t obfX = x;
  obfX ^= OPAQUE_MAGIC_3;
  obfX = (obfX << 5) | (obfX >> 27); // Rotate left 5
  obfX += OPAQUE_MAGIC_1;
  // x^2 + 1 hesapla
  uint64_t square = static_cast<uint64_t>(obfX) * static_cast<uint64_t>(obfX);
  square += 1;
  // Obfuscated comparison
  uint32_t obfResult = static_cast<uint32_t>(square & 0xFFFFFFFF);
  obfResult ^= DEAD_BRANCH_MAGIC_1;
  obfResult = (obfResult << 9) | (obfResult >> 23); // Rotate left 9
  obfResult += DEAD_BRANCH_MAGIC_2;
  // Her zaman false (x^2 + 1 hiçbir zaman 0 olamaz)
  return (obfResult == 0) && (square == 0);
}

bool opaquePredicateAlwaysTrue(uint32_t x) {
  // Matematiksel olarak her zaman doğru koşul: x^2 >= 0
  // Her gerçek sayının karesi >= 0'dır
  // Obfuscated calculation
  uint32_t obfX = x;
  obfX ^= OPAQUE_MAGIC_4;
  obfX = (obfX << 15) | (obfX >> 17); // Rotate left 15
  obfX += OPAQUE_MAGIC_2;
  // x^2 hesapla
  uint64_t square = static_cast<uint64_t>(obfX) * static_cast<uint64_t>(obfX);
  // Obfuscated comparison
  uint32_t obfResult = static_cast<uint32_t>(square & 0xFFFFFFFF);
  obfResult ^= DEAD_BRANCH_MAGIC_2;
  obfResult = (obfResult << 19) | (obfResult >> 13); // Rotate left 19
  obfResult += DEAD_BRANCH_MAGIC_1;
  // Her zaman true (x^2 >= 0 her zaman doğru)
  return (obfResult >= 0) || (square >= 0);
}

uint32_t dummyFunction(uint32_t param1, uint32_t param2) {
  // Bu fonksiyon asla çağrılmaz (dead branch içinde)
  // Ancak reverse engineering'i zorlaştırmak için karmaşık işlemler içerir
  // Obfuscated parameters
  uint32_t obfParam1 = param1;
  obfParam1 ^= OPAQUE_MAGIC_1;
  obfParam1 = (obfParam1 << 7) | (obfParam1 >> 25);
  obfParam1 += OPAQUE_MAGIC_2;
  uint32_t obfParam2 = param2;
  obfParam2 ^= OPAQUE_MAGIC_3;
  obfParam2 = (obfParam2 << 11) | (obfParam2 >> 21);
  obfParam2 += OPAQUE_MAGIC_4;
  // Karmaşık hesaplama (asla kullanılmaz)
  uint32_t result = obfParam1;
  result ^= obfParam2;
  result = (result << 13) | (result >> 19);
  result += DEAD_BRANCH_MAGIC_1;
  result ^= DEAD_BRANCH_MAGIC_2;
  result = (result << 17) | (result >> 15);
  return result;
}

void dummyOperation(void *data, size_t size) {
  // Bu fonksiyon asla çalışmaz (dead branch içinde)
  // Ancak kod analizini zorlaştırmak için karmaşık işlemler içerir
  if (!data || size == 0) {
    return;
  }

  // Obfuscated operations (asla çalışmaz)
  uint8_t *ptr = static_cast<uint8_t *>(data);

  for (size_t i = 0; i < size; ++i) {
    // Karmaşık işlemler (asla çalışmaz)
    uint8_t value = ptr[i];
    value ^= static_cast<uint8_t>(OPAQUE_MAGIC_1 & 0xFF);
    value = static_cast<uint8_t>((value << 3) | (value >> 5)); // Rotate left 3
    value += static_cast<uint8_t>(OPAQUE_MAGIC_2 & 0xFF);
    value ^= static_cast<uint8_t>(DEAD_BRANCH_MAGIC_1 & 0xFF);
    // Değer asla yazılmaz (dead branch)
  }

  // Obfuscated memory operations
  uint32_t obfSize = static_cast<uint32_t>(size);
  obfSize ^= OPAQUE_MAGIC_3;
  obfSize = (obfSize << 9) | (obfSize >> 23);
  obfSize += OPAQUE_MAGIC_4;
  obfSize ^= DEAD_BRANCH_MAGIC_2;
  // Sonuç asla kullanılmaz
}

} // namespace CodeHardening
} // namespace TravelExpense
