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
#include "error_codes.h"

/**
 * @namespace TravelExpense
 * @brief Ana namespace - Seyahat Gideri Takibi uygulaması
 */
namespace TravelExpense {

    /**
     * @struct User
     * @brief Kullanıcı veri modeli
     */
    struct User {
        int32_t userId;                /**< @brief Kullanıcı kimliği */
        char username[50];             /**< @brief Kullanıcı adı */
        char passwordHash[64];         /**< @brief SHA-256 hash (32 bytes hex = 64 chars) */
        char salt[32];                 /**< @brief Salt değeri (16 bytes hex = 32 chars) */
        bool isGuest;                  /**< @brief Misafir kullanıcı mı? */
        time_t createdAt;              /**< @brief Oluşturulma zamanı */
        time_t lastLogin;              /**< @brief Son giriş zamanı */

        /**
         * @brief User constructor
         * 
         * Varsayılan değerlerle yeni bir User oluşturur.
         */
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
        int32_t tripId;                 /**< @brief Seyahat kimliği */
        int32_t userId;                 /**< @brief Kullanıcı kimliği */
        char destination[100];           /**< @brief Seyahat hedefi */
        char startDate[11];              /**< @brief Başlangıç tarihi (YYYY-MM-DD formatı) */
        char endDate[11];                /**< @brief Bitiş tarihi (YYYY-MM-DD formatı) */
        char accommodation[200];         /**< @brief Konaklama bilgisi */
        char transportation[100];         /**< @brief Ulaşım bilgisi */
        double budget;                   /**< @brief Bütçe */
        double totalExpenses;            /**< @brief Toplam gider */
        time_t createdAt;                /**< @brief Oluşturulma zamanı */
        time_t updatedAt;                /**< @brief Güncellenme zamanı */

        /**
         * @brief Trip constructor
         * 
         * Varsayılan değerlerle yeni bir Trip oluşturur.
         */
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
        ACCOMMODATION = 0,    /**< @brief Konaklama */
        TRANSPORTATION = 1,   /**< @brief Ulaşım */
        FOOD = 2,             /**< @brief Yemek */
        ENTERTAINMENT = 3     /**< @brief Eğlence */
    };

    /**
     * @struct Expense
     * @brief Gider veri modeli
     */
    struct Expense {
        int32_t expenseId;             /**< @brief Gider kimliği */
        int32_t tripId;                 /**< @brief Seyahat kimliği */
        ExpenseCategory category;      /**< @brief Gider kategorisi */
        double amount;                  /**< @brief Gider tutarı */
        char currency[4];               /**< @brief Para birimi (TRY, USD, EUR, vb.) */
        char date[11];                  /**< @brief Tarih (YYYY-MM-DD) */
        char paymentMethod[50];         /**< @brief Ödeme yöntemi (Nakit, Kredi Kartı, vb.) */
        char description[200];          /**< @brief Gider açıklaması */
        time_t createdAt;               /**< @brief Oluşturulma zamanı */

        /**
         * @brief Expense constructor
         * 
         * Varsayılan değerlerle yeni bir Expense oluşturur.
         */
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
        int32_t budgetId;               /**< @brief Bütçe kimliği */
        int32_t tripId;                 /**< @brief Seyahat kimliği */
        double totalBudget;             /**< @brief Toplam bütçe */
        double spentAmount;             /**< @brief Harcanan tutar */
        double categoryBudgets[4];     /**< @brief Kategori bütçeleri [Konaklama, Ulaşım, Yemek, Eğlence] */
        double categorySpent[4];       /**< @brief Kategori harcamaları [Konaklama, Ulaşım, Yemek, Eğlence] */
        time_t createdAt;              /**< @brief Oluşturulma zamanı */
        time_t updatedAt;               /**< @brief Güncellenme zamanı */

        /**
         * @brief Budget constructor
         * 
         * Varsayılan değerlerle yeni bir Budget oluşturur.
         */
        Budget() : budgetId(0), tripId(0), totalBudget(0.0), spentAmount(0.0),
                   createdAt(0), updatedAt(0) {
            memset(categoryBudgets, 0, sizeof(categoryBudgets));
            memset(categorySpent, 0, sizeof(categorySpent));
        }
    };

} // namespace TravelExpense

#endif // COMMON_TYPES_H

