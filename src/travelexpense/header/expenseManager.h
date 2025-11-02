/**
 * @file expenseManager.h
 * @brief Seyahat Gideri Takibi - Gider Yönetimi Modülü
 * 
 * Bu dosya, gider yönetimi işlemlerinin fonksiyon bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef EXPENSE_MANAGER_H
#define EXPENSE_MANAGER_H

#include "commonTypes.h"
#include <vector>

namespace TravelExpense {

    /**
     * @namespace ExpenseManager
     * @brief Gider yönetimi modülü
     */
    namespace ExpenseManager {
        /**
         * @brief Yeni gider kaydet
         * @param expense Gider bilgileri
         * @param expenseId Oluşturulan giderin ID'si
         * @return ErrorCode Başarı durumu
         */
        ErrorCode logExpense(const Expense& expense, int32_t& expenseId);

        /**
         * @brief Seyahate ait giderleri listele
         * @param tripId Seyahat ID
         * @param expenses Gider listesi
         * @return ErrorCode Başarı durumu
         */
        ErrorCode getExpenses(int32_t tripId, std::vector<Expense>& expenses);

        /**
         * @brief Gider bilgisini güncelle
         * @param expenseId Gider ID
         * @param expense Güncellenmiş gider bilgileri
         * @return ErrorCode Başarı durumu
         */
        ErrorCode updateExpense(int32_t expenseId, const Expense& expense);

        /**
         * @brief Gideri sil
         * @param expenseId Gider ID
         * @return ErrorCode Başarı durumu
         */
        ErrorCode deleteExpense(int32_t expenseId);

        /**
         * @brief Gider detayını al
         * @param expenseId Gider ID
         * @param expense Gider bilgisi
         * @return ErrorCode Başarı durumu
         */
        ErrorCode getExpense(int32_t expenseId, Expense& expense);
    }

} // namespace TravelExpense

#endif // EXPENSE_MANAGER_H

