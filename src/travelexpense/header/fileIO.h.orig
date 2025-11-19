/**
 * @file fileIO.h
 * @brief Seyahat Gideri Takibi - Dosya I/O Yardımcı Fonksiyonlar
 *
 * Bu dosya, dosya işlemleri için yardımcı fonksiyon bildirimlerini içerir.
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef FILE_IO_H
#define FILE_IO_H

#include "export.h"
#include <cstdint>
#include <string>

namespace TravelExpense { // LCOV_EXCL_LINE

/**
 * @namespace FileIO
 * @brief Dosya I/O yardımcı fonksiyonları
 *
 * Bu modül, dosya sistemi işlemleri ve veri dizini yönetimi için
 * yardımcı fonksiyonlar sağlar. ID yönetimi ve veri dizini
 * oluşturma işlemlerini yönetir.
 */
namespace FileIO {
/**
 * @brief Veri dizinini oluştur (yoksa)
 *
 * Uygulama veri dizinini (varsayılan: "data") oluşturur.
 * Eğer dizin zaten varsa, hiçbir işlem yapmaz.
 *
 * @note Bu fonksiyon, uygulama başlatıldığında çağrılmalıdır.
 * Dizin oluşturulamazsa (örneğin izin hatası), false döner.
 *
 * @return true Başarılı (dizin oluşturuldu veya zaten var), false Hata (izin hatası vb.)
 */
TRAVELEXPENSE_API bool ensureDataDirectory();

/**
 * @brief Son ID'yi dosyadan al
 *
 * Belirtilen entity için son kullanılan ID'yi dosyadan okur.
 * Dosya yoksa veya okuma hatası varsa 0 döner.
 *
 * @note Bu fonksiyon, entity ID'lerini takip etmek için kullanılır.
 * Dosya formatı: ASCII sayı olarak saklanır.
 *
 * @param filename Entity adı (user, trip, expense, budget)
 *                  Dosya yolu: "data/{filename}_id.txt" formatında
 *                  nullptr ise 0 döner.
 * @return int32_t Son ID, 0 ise dosya yok veya okuma hatası
 */
TRAVELEXPENSE_API int32_t getLastId(const char *filename);

/**
 * @brief Son ID'yi dosyaya kaydet
 *
 * Belirtilen entity için son kullanılan ID'yi dosyaya yazar.
 * Dosya yoksa oluşturulur, varsa güncellenir.
 *
 * @note Bu fonksiyon, ID tracking için kullanılır.
 * Dosya formatı: ASCII sayı olarak saklanır.
 *
 * @param filename Entity adı (user, trip, expense, budget)
 *                  Dosya yolu: "data/{filename}_id.txt" formatında
 *                  nullptr ise false döner.
 * @param id Kaydedilecek ID (0'dan büyük olmalı)
 * @return true Başarılı, false Hata (dosya yazma hatası, izin hatası vb.)
 */
TRAVELEXPENSE_API bool saveLastId(const char *filename, int32_t id);

/**
 * @brief Yeni ID oluştur
 *
 * Belirtilen entity için yeni bir benzersiz ID oluşturur.
 * Son ID'yi dosyadan okur, 1 artırır, dosyaya kaydeder ve döndürür.
 *
 * @note Bu fonksiyon, atomik ID oluşturma sağlar.
 * Eğer dosya yoksa, 1 ile başlar.
 *
 * @param entityName Entity adı (user, trip, expense, budget)
 *                    nullptr ise 0 döner.
 * @return int32_t Yeni ID (1'den başlar, her çağrıda 1 artar), 0 ise hata
 */
TRAVELEXPENSE_API int32_t generateId(const char *entityName);
}

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // FILE_IO_H
