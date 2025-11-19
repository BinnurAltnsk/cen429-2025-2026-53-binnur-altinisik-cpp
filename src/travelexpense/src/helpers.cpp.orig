/**
 * @file helpers.cpp
 * @brief Seyahat Gideri Takibi - Yardımcı Fonksiyonlar Implementation
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/helpers.h"
#include <cstring>

namespace TravelExpense {

const char *getCategoryString(ExpenseCategory category) {
  switch (category) {
    case ExpenseCategory::ACCOMMODATION:
      return "Konaklama";

    case ExpenseCategory::TRANSPORTATION:
      return "Ulaşım";

    case ExpenseCategory::FOOD:
      return "Yemek";

    case ExpenseCategory::ENTERTAINMENT:
      return "Eğlence";

    default:
      return "Bilinmeyen";
  }
}

ExpenseCategory getCategoryFromString(const char *categoryStr) {
  if (strcmp(categoryStr, "Konaklama") == 0 || strcmp(categoryStr, "ACCOMMODATION") == 0) {
    return ExpenseCategory::ACCOMMODATION;
  } else if (strcmp(categoryStr, "Ulaşım") == 0 || strcmp(categoryStr, "TRANSPORTATION") == 0) {
    return ExpenseCategory::TRANSPORTATION;
  } else if (strcmp(categoryStr, "Yemek") == 0 || strcmp(categoryStr, "FOOD") == 0) {
    return ExpenseCategory::FOOD;
  } else if (strcmp(categoryStr, "Eğlence") == 0 || strcmp(categoryStr, "ENTERTAINMENT") == 0) {
    return ExpenseCategory::ENTERTAINMENT;
  }

  return ExpenseCategory::ACCOMMODATION; // Varsayılan
}

} // namespace TravelExpense
