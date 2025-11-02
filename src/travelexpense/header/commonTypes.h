/**
 * @file commonTypes.h
 * @brief Seyahat Gideri Takibi - Ortak Veri Tipleri ve Modelleri
 * 
 * Bu dosya, seyahat gideri takibi uygulamasının ortak veri modellerini,
 * enum'ları ve temel tipleri içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <cstdint>
#include <ctime>
#include <cstring>

/**
 * @namespace TravelExpense
 * @brief Ana namespace - Seyahat Gideri Takibi uygulaması
 */
namespace TravelExpense {

    /**
     * @brief Hata kodları enum'u
     */
    enum class ErrorCode {
        SUCCESS = 0,
        ERROR_FILE_NOT_FOUND = 1,
        ERROR_DECRYPTION_FAILED = 2,
        ERROR_CHECKSUM_MISMATCH = 3,
        ERROR_INVALID_USER = 4,
        ERROR_BUDGET_EXCEEDED = 5,
        ERROR_INVALID_INPUT = 6,
        ERROR_MEMORY_ALLOCATION = 7,
        ERROR_FILE_IO = 8,
        ERROR_ENCRYPTION_FAILED = 9,
        ERROR_UNKNOWN = 99
    };

    /**
     * @struct User
     * @brief Kullanıcı veri modeli
     */
    struct User {
        int32_t userId;
        char username[50];
        char passwordHash[64];        // SHA-256 hash (32 bytes hex = 64 chars)
        char salt[32];                // Salt değeri (16 bytes hex = 32 chars)
        bool isGuest;
        time_t createdAt;
        time_t lastLogin;

        User() : userId(0), isGuest(false), createdAt(0), lastLogin(0) {
            memset(username, 0, sizeof(username));
            memset(passwordHash, 0, sizeof(passwordHash));
            memset(salt, 0, sizeof(salt));
        }
    };

    /**
     * @struct Trip
     * @brief Seyahat veri modeli
     */
    struct Trip {
        int32_t tripId;
        int32_t userId;
        char destination[100];
        char startDate[11];           // YYYY-MM-DD formatı
        char endDate[11];
        char accommodation[200];
        char transportation[100];
        double budget;
        double totalExpenses;
        time_t createdAt;
        time_t updatedAt;

        Trip() : tripId(0), userId(0), budget(0.0), totalExpenses(0.0),
                 createdAt(0), updatedAt(0) {
            memset(destination, 0, sizeof(destination));
            memset(startDate, 0, sizeof(startDate));
            memset(endDate, 0, sizeof(endDate));
            memset(accommodation, 0, sizeof(accommodation));
            memset(transportation, 0, sizeof(transportation));
        }
    };

    /**
     * @enum ExpenseCategory
     * @brief Gider kategorileri
     */
    enum class ExpenseCategory {
        ACCOMMODATION = 0,    // Konaklama
        TRANSPORTATION = 1,   // Ulaşım
        FOOD = 2,             // Yemek
        ENTERTAINMENT = 3     // Eğlence
    };

    /**
     * @struct Expense
     * @brief Gider veri modeli
     */
    struct Expense {
        int32_t expenseId;
        int32_t tripId;
        ExpenseCategory category;
        double amount;
        char currency[4];              // TRY, USD, EUR, vb.
        char date[11];                 // YYYY-MM-DD
        char paymentMethod[50];        // Nakit, Kredi Kartı, vb.
        char description[200];
        time_t createdAt;

        Expense() : expenseId(0), tripId(0), category(ExpenseCategory::ACCOMMODATION),
                    amount(0.0), createdAt(0) {
            memset(currency, 0, sizeof(currency));
            memset(date, 0, sizeof(date));
            memset(paymentMethod, 0, sizeof(paymentMethod));
            memset(description, 0, sizeof(description));
        }
    };

    /**
     * @struct Budget
     * @brief Bütçe veri modeli
     */
    struct Budget {
        int32_t budgetId;
        int32_t tripId;
        double totalBudget;
        double spentAmount;
        double categoryBudgets[4];      // [Konaklama, Ulaşım, Yemek, Eğlence]
        double categorySpent[4];
        time_t createdAt;
        time_t updatedAt;

        Budget() : budgetId(0), tripId(0), totalBudget(0.0), spentAmount(0.0),
                   createdAt(0), updatedAt(0) {
            memset(categoryBudgets, 0, sizeof(categoryBudgets));
            memset(categorySpent, 0, sizeof(categorySpent));
        }
    };

} // namespace TravelExpense

#endif // COMMON_TYPES_H

