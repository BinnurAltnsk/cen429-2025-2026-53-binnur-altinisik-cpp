/**
 * @file budgetManager.h
 * @brief Seyahat Gideri Takibi - Bütçe Yönetimi Modülü
 * 
 * Bu dosya, bütçe yönetimi işlemlerinin fonksiyon bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef BUDGET_MANAGER_H
#define BUDGET_MANAGER_H

#include "commonTypes.h"

namespace TravelExpense {

    /**
     * @namespace BudgetManager
     * @brief Bütçe yönetimi modülü
     */
    namespace BudgetManager {
        /**
         * @brief Seyahat için bütçe belirle
         * @param tripId Seyahat ID
         * @param totalBudget Toplam bütçe
         * @param categoryBudgets Kategori bazlı bütçeler (4 eleman)
         * @param budgetId Oluşturulan bütçenin ID'si
         * @return ErrorCode Başarı durumu
         */
        ErrorCode setBudget(int32_t tripId, double totalBudget, 
                           const double* categoryBudgets, int32_t& budgetId);

        /**
         * @brief Seyahat bütçesini görüntüle
         * @param tripId Seyahat ID
         * @param budget Bütçe bilgisi
         * @return ErrorCode Başarı durumu
         */
        ErrorCode getBudget(int32_t tripId, Budget& budget);

        /**
         * @brief Bütçeyi güncelle
         * @param budgetId Bütçe ID
         * @param budget Güncellenmiş bütçe bilgileri
         * @return ErrorCode Başarı durumu
         */
        ErrorCode updateBudget(int32_t budgetId, const Budget& budget);

        /**
         * @brief Bütçe limiti kontrolü
         * @param tripId Seyahat ID
         * @param category Gider kategorisi
         * @param amount Harcama miktarı
         * @return true Bütçe limiti içinde, false Limit aşıldı
         */
        bool checkBudgetLimit(int32_t tripId, ExpenseCategory category, double amount);
    }

} // namespace TravelExpense

#endif // BUDGET_MANAGER_H

