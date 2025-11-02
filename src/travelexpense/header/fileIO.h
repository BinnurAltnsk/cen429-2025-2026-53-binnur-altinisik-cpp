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

#include <cstdint>
#include <string>

namespace TravelExpense {

    /**
     * @namespace FileIO
     * @brief Dosya I/O yardımcı fonksiyonları
     */
    namespace FileIO {
        /**
         * @brief Veri dizinini oluştur (yoksa)
         * @return true Başarılı, false Hata
         */
        bool ensureDataDirectory();

        /**
         * @brief Son ID'yi dosyadan al
         * @param filename Entity adı (user, trip, expense, budget)
         * @return int32_t Son ID, 0 ise dosya yok
         */
        int32_t getLastId(const char* filename);

        /**
         * @brief Son ID'yi dosyaya kaydet
         * @param filename Entity adı
         * @param id Kaydedilecek ID
         * @return true Başarılı, false Hata
         */
        bool saveLastId(const char* filename, int32_t id);

        /**
         * @brief Yeni ID oluştur
         * @param entityName Entity adı (user, trip, expense, budget)
         * @return int32_t Yeni ID
         */
        int32_t generateId(const char* entityName);
    }

} // namespace TravelExpense

#endif // FILE_IO_H

