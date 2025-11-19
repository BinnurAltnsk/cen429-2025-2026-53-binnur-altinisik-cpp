/**
 * @file helpers.h
 * @brief Seyahat Gideri Takibi - Yard─▒mc─▒ Fonksiyonlar
 *
 * Bu dosya, kategori string d├Ân├╝┼ş├╝mleri gibi yard─▒mc─▒ fonksiyon bildirimlerini i├ğerir.
 *
 * @author Binnur Alt─▒n─▒┼ş─▒k
 * @date 2025
 */

#ifndef HELPERS_H
#define HELPERS_H

#include "commonTypes.h"

namespace TravelExpense { // LCOV_EXCL_LINE

/**
 * @brief Kategori string'ini al
 * @param category Gider kategorisi
 * @return const char* Kategori ad─▒
 */
const char *getCategoryString(ExpenseCategory category);

/**
 * @brief String'i kategori enum'una ├ğevir
 * @param categoryStr Kategori string'i
 * @return ExpenseCategory Kategori enum'u
 */
ExpenseCategory getCategoryFromString(const char *categoryStr);

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // HELPERS_H
