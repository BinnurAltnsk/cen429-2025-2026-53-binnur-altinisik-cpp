/**
 * @file budgetManager.cpp
 * @brief Seyahat Gideri Takibi - Bütçe Yönetimi Implementation
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/budgetManager.h"
#include "../header/database.h"
#include <sqlite3.h>
#include <cstring>
#include <ctime>

namespace TravelExpense {

    namespace BudgetManager {

        ErrorCode setBudget(int32_t tripId, double totalBudget, 
                           const double* categoryBudgets, int32_t& budgetId) {
            if (tripId <= 0 || totalBudget <= 0 || !categoryBudgets) {
                return ErrorCode::InvalidInput;
            }

            // SQLite veritabanını al
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::FileNotFound;
            }

            // SQL sorgusu hazırla
            const char* sql = R"(
                INSERT INTO budgets (trip_id, total_budget, spent_amount,
                                   category_budget_accommodation, category_budget_transportation,
                                   category_budget_food, category_budget_entertainment,
                                   category_spent_accommodation, category_spent_transportation,
                                   category_spent_food, category_spent_entertainment,
                                   created_at, updated_at)
                VALUES (?, ?, 0.0, ?, ?, ?, ?, 0.0, 0.0, 0.0, 0.0, ?, ?);
            )";

            sqlite3_stmt* stmt = nullptr;
            int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                return ErrorCode::FileIO;
            }

            // Parametreleri bağla
            time_t now = time(nullptr);
            sqlite3_bind_int(stmt, 1, tripId);
            sqlite3_bind_double(stmt, 2, totalBudget);
            sqlite3_bind_double(stmt, 3, categoryBudgets[0]); // ACCOMMODATION
            sqlite3_bind_double(stmt, 4, categoryBudgets[1]); // TRANSPORTATION
            sqlite3_bind_double(stmt, 5, categoryBudgets[2]); // FOOD
            sqlite3_bind_double(stmt, 6, categoryBudgets[3]); // ENTERTAINMENT
            sqlite3_bind_int64(stmt, 7, static_cast<sqlite3_int64>(now));
            sqlite3_bind_int64(stmt, 8, static_cast<sqlite3_int64>(now));

            // Sorguyu çalıştır
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                sqlite3_finalize(stmt);
                return ErrorCode::FileIO;
            }

            // Oluşturulan ID'yi al
            budgetId = static_cast<int32_t>(Database::getLastInsertRowId(db));

            sqlite3_finalize(stmt);

            return ErrorCode::Success;
        }

        ErrorCode getBudget(int32_t tripId, Budget& budget) {
            // SQLite veritabanını al
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::FileNotFound;
            }

            // SQL sorgusu hazırla
            const char* sql = R"(
                SELECT budget_id, trip_id, total_budget, spent_amount,
                       category_budget_accommodation, category_budget_transportation,
                       category_budget_food, category_budget_entertainment,
                       category_spent_accommodation, category_spent_transportation,
                       category_spent_food, category_spent_entertainment,
                       created_at, updated_at
                FROM budgets WHERE trip_id = ?;
            )";

            sqlite3_stmt* stmt = nullptr;
            int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                return ErrorCode::FileIO;
            }

            // Parametreleri bağla
            sqlite3_bind_int(stmt, 1, tripId);

            // Sorguyu çalıştır
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_ROW) {
                sqlite3_finalize(stmt);
                return ErrorCode::InvalidInput;
            }

            // Sonuçları al
            budget.budgetId = sqlite3_column_int(stmt, 0);
            budget.tripId = sqlite3_column_int(stmt, 1);
            budget.totalBudget = sqlite3_column_double(stmt, 2);
            budget.spentAmount = sqlite3_column_double(stmt, 3);
            
            budget.categoryBudgets[0] = sqlite3_column_double(stmt, 4); // ACCOMMODATION
            budget.categoryBudgets[1] = sqlite3_column_double(stmt, 5); // TRANSPORTATION
            budget.categoryBudgets[2] = sqlite3_column_double(stmt, 6); // FOOD
            budget.categoryBudgets[3] = sqlite3_column_double(stmt, 7); // ENTERTAINMENT
            
            budget.categorySpent[0] = sqlite3_column_double(stmt, 8); // ACCOMMODATION
            budget.categorySpent[1] = sqlite3_column_double(stmt, 9); // TRANSPORTATION
            budget.categorySpent[2] = sqlite3_column_double(stmt, 10); // FOOD
            budget.categorySpent[3] = sqlite3_column_double(stmt, 11); // ENTERTAINMENT
            
            budget.createdAt = sqlite3_column_int64(stmt, 12);
            budget.updatedAt = sqlite3_column_int64(stmt, 13);

            sqlite3_finalize(stmt);

            return ErrorCode::Success;
        }

        ErrorCode updateBudget(int32_t budgetId, const Budget& budget) {
            // SQLite veritabanını al
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::FileNotFound;
            }

            // SQL sorgusu hazırla
            const char* sql = R"(
                UPDATE budgets 
                SET trip_id = ?, total_budget = ?, spent_amount = ?,
                    category_budget_accommodation = ?, category_budget_transportation = ?,
                    category_budget_food = ?, category_budget_entertainment = ?,
                    category_spent_accommodation = ?, category_spent_transportation = ?,
                    category_spent_food = ?, category_spent_entertainment = ?,
                    updated_at = ?
                WHERE budget_id = ?;
            )";

            sqlite3_stmt* stmt = nullptr;
            int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                return ErrorCode::FileIO;
            }

            // Parametreleri bağla
            time_t now = time(nullptr);
            sqlite3_bind_int(stmt, 1, budget.tripId);
            sqlite3_bind_double(stmt, 2, budget.totalBudget);
            sqlite3_bind_double(stmt, 3, budget.spentAmount);
            sqlite3_bind_double(stmt, 4, budget.categoryBudgets[0]);
            sqlite3_bind_double(stmt, 5, budget.categoryBudgets[1]);
            sqlite3_bind_double(stmt, 6, budget.categoryBudgets[2]);
            sqlite3_bind_double(stmt, 7, budget.categoryBudgets[3]);
            sqlite3_bind_double(stmt, 8, budget.categorySpent[0]);
            sqlite3_bind_double(stmt, 9, budget.categorySpent[1]);
            sqlite3_bind_double(stmt, 10, budget.categorySpent[2]);
            sqlite3_bind_double(stmt, 11, budget.categorySpent[3]);
            sqlite3_bind_int64(stmt, 12, static_cast<sqlite3_int64>(now));
            sqlite3_bind_int(stmt, 13, budgetId);

            // Sorguyu çalıştır
            rc = sqlite3_step(stmt);
            sqlite3_finalize(stmt);

            if (rc != SQLITE_DONE) {
                return ErrorCode::FileIO;
            }

            // Etkilenen satır sayısını kontrol et
            if (sqlite3_changes(db) == 0) {
                return ErrorCode::InvalidInput;
            }

            return ErrorCode::Success;
        }

        bool checkBudgetLimit(int32_t tripId, ExpenseCategory category, double amount) {
            Budget budget;
            if (getBudget(tripId, budget) != ErrorCode::Success) {
                return false; // Bütçe bulunamazsa izin verilmez
            }

            int categoryIndex = static_cast<int>(category);
            if (categoryIndex < 0 || categoryIndex >= 4) {
                return false;
            }

            return (budget.categorySpent[categoryIndex] + amount) <= budget.categoryBudgets[categoryIndex];
        }
    }

} // namespace TravelExpense

