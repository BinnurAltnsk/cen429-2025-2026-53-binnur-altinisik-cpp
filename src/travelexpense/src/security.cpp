/**
 * @file security.cpp
 * @brief Seyahat Gideri Takibi - Güvenlik Fonksiyonları Implementation
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/security.h"
#include <cstring>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <winbase.h>
// SecureZeroMemory zaten Windows'ta tanımlı
#else
#include <string.h>
/**
 * @brief Linux için güvenli bellek setleme fonksiyonu
 *
 * Bu fonksiyon, bellek alanını güvenli şekilde doldurur.
 * Volatile pointer kullanarak compiler optimizer'ın bu işlemi
 * atlamasını engeller. Hassas verilerin güvenli şekilde
 * temizlenmesi için kullanılır.
 *
 * @param s Bellek alanının başlangıç adresi
 * @param c Doldurulacak değer (unsigned char olarak yorumlanır)
 * @param n Doldurulacak byte sayısı
 * @return Bellek alanının başlangıç adresi (ptr)
 */
static void *secure_memset(void *s, int c, size_t n) {
  volatile unsigned char *p = static_cast<volatile unsigned char *>(s);

  while (n--) {
    *p++ = static_cast<unsigned char>(c);
  }

  return s;
}
#define SecureZeroMemory(ptr, size) secure_memset(ptr, 0, size)
#endif

namespace TravelExpense {

namespace Security {

/**
 * @brief Bellek bölgesini güvenli şekilde doldur (memset güvenli versiyonu)
 *
 * Bu fonksiyon, bellek alanını belirtilen değerle doldurur. Volatile pointer
 * kullanarak compiler optimizer'ın bu işlemi atlamasını engeller. Bu sayede
 * hassas verilerin güvenli şekilde temizlenmesi sağlanır.
 *
 * @note Standart memset() fonksiyonundan farklı olarak, bu fonksiyon
 * optimizer'ın silme işlemini atlamasını engellemek için volatile pointer
 * kullanır. Bu, hassas verilerin (şifreler, anahtarlar vb.) güvenli
 * şekilde temizlenmesi için kritiktir.
 *
 * @param ptr Bellek alanının başlangıç adresi (nullptr ise işlem yapılmaz)
 * @param value Doldurulacak değer (unsigned char olarak yorumlanır)
 * @param size Doldurulacak byte sayısı (0 ise işlem yapılmaz)
 */
void secureMemset(void *ptr, int value, size_t size) {
  if (!ptr || size == 0) {
    return;
  }

  volatile unsigned char *p = static_cast<volatile unsigned char *>(ptr);
  volatile unsigned char v = static_cast<volatile unsigned char>(value);
  // Volatile pointer kullanarak optimizer'ın atlamasını engelle
  size_t i = 0;

  while (i < size) {
    p[i] = v;
    ++i;
  }
}

/**
 * @brief Bellekteki hassas verileri güvenli şekilde sil (çoklu geçişli temizleme)
 *
 * Bu fonksiyon, bellek içeriğini üç geçişli bir algoritma ile güvenli şekilde siler:
 * 1. Geçiş: Bellek alanını sıfırlama (SecureZeroMemory veya secure_memset)
 * 2. Geçiş: Rastgele verilerle doldurma (veri kurtarmayı zorlaştırmak için)
 * 3. Geçiş: Tekrar sıfırlama ve volatile pointer ile kontrol
 *
 * Bu yöntem, hassas verilerin (şifreler, anahtarlar, token'lar vb.) güvenli
 * şekilde temizlenmesini sağlar. Çoklu geçişli silme, manyetik kalıntı (magnetic
 * remanence) tabanlı veri kurtarma saldırılarına karşı ek koruma sağlar.
 *
 * @note Bu fonksiyon, platform-specific güvenli bellek silme fonksiyonlarını
 * kullanır (Windows: SecureZeroMemory, Linux: secure_memset). Volatile pointer
 * kullanarak compiler optimizer'ın silme işlemini atlamasını engeller.
 *
 * @param ptr Silinecek bellek alanının başlangıç adresi (nullptr ise false döner)
 * @param size Silinecek bellek alanının boyutu (byte, 0 ise false döner)
 * @return true Başarılı, false Hata (null pointer veya sıfır uzunluk)
 */
bool secureMemoryCleanup(void *ptr, size_t size) {
  if (!ptr || size == 0) {
    return false;
  }

  // Çoklu geçişli silme (3 geçiş)
  // Geçiş 1: Sıfırlama
#ifdef _WIN32
  SecureZeroMemory(ptr, size);
#else
  secure_memset(ptr, 0, size);
#endif
  // Geçiş 2: Rastgele veri ile doldurma
  srand(static_cast<unsigned int>(time(nullptr)));
  unsigned char *p = static_cast<unsigned char *>(ptr);

  for (size_t i = 0; i < size; ++i) {
    p[i] = static_cast<unsigned char>(rand() % 256);
  }

  // Geçiş 3: Tekrar sıfırlama
#ifdef _WIN32
  SecureZeroMemory(ptr, size);
#else
  secure_memset(ptr, 0, size);
#endif
  // Bellek alanını volatile pointer ile tekrar kontrol et
  volatile unsigned char *vp = static_cast<volatile unsigned char *>(ptr);

  for (size_t i = 0; i < size; ++i) {
    vp[i] = 0;
  }

  return true;
}

/**
 * @brief Buffer içeriğini güvenli şekilde temizle (char array için overload)
 *
 * Bu fonksiyon, char buffer içeriğini güvenli şekilde temizler.
 * `secureMemoryCleanup()` fonksiyonunu çağırarak çoklu geçişli
 * temizleme algoritmasını kullanır.
 *
 * @note Bu fonksiyon, özellikle string buffer'ları (şifreler, token'lar vb.)
 * temizlemek için kullanılır. Null pointer veya sıfır uzunluk kontrolü yapılır.
 *
 * @param buffer Silinecek buffer (nullptr ise işlem yapılmaz)
 * @param size Buffer boyutu (byte, 0 ise işlem yapılmaz)
 */
void secureCleanup(char *buffer, size_t size) {
  if (!buffer || size == 0) {
    return;
  }

  secureMemoryCleanup(buffer, size);
}

} // namespace Security

} // namespace TravelExpense
