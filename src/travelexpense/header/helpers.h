/**
 * @file helpers.h
 * @brief Seyahat Gideri Takibi - Yardımcı Fonksiyonlar
 * 
 * Bu dosya, kategori string dönüşümleri gibi yardımcı fonksiyon bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef HELPERS_H
#define HELPERS_H

#include "commonTypes.h"

namespace TravelExpense {

    /**
     * @brief Kategori string'ini al
     * @param category Gider kategorisi
     * @return const char* Kategori adı
     */
    const char* getCategoryString(ExpenseCategory category);

    /**
     * @brief String'i kategori enum'una çevir
     * @param categoryStr Kategori string'i
     * @return ExpenseCategory Kategori enum'u
     */
    ExpenseCategory getCategoryFromString(const char* categoryStr);

} // namespace TravelExpense

#endif // HELPERS_H

