/**
 * @file security.h
 * @brief Seyahat Gideri Takibi - Güvenlik Fonksiyonları
 * 
 * Bu dosya, güvenli bellek silme ve güvenlik yardımcı fonksiyonlarının
 * bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef SECURITY_H
#define SECURITY_H

#include <cstddef>
#include "export.h"

namespace TravelExpense { // LCOV_EXCL_LINE

    /**
     * @namespace Security
     * @brief Güvenlik fonksiyonları modülü
     * 
     * Bu modül, güvenli bellek silme, bellek temizleme ve güvenli
     * bellek işlemleri için yardımcı fonksiyonlar sağlar. Hassas
     * verilerin güvenli şekilde temizlenmesini sağlar.
     */
    namespace Security {
        /**
         * @brief Bellekteki hassas verileri güvenli şekilde sil
         * 
         * Bu fonksiyon, bellek içeriğini sıfırlar ve ardından rastgele verilerle
         * doldurarak verilerin kurtarılamamasını sağlar.
         * 
         * @param ptr Silinecek bellek alanının başlangıç adresi
         * @param size Silinecek bellek alanının boyutu (byte)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool secureMemoryCleanup(void* ptr, size_t size);

        /**
         * @brief Buffer içeriğini güvenli şekilde temizle (overload - char array için)
         * 
         * @param buffer Silinecek buffer
         * @param size Buffer boyutu
         */
        TRAVELEXPENSE_API void secureCleanup(char* buffer, size_t size);

        /**
         * @brief Bellek bölgesini sıfırla (memset güvenli versiyonu)
         * 
         * @param ptr Bellek adresi
         * @param size Boyut
         * @param value Doldurulacak değer (genellikle 0)
         */
        TRAVELEXPENSE_API void secureMemset(void* ptr, int value, size_t size);
    }

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // SECURITY_H

