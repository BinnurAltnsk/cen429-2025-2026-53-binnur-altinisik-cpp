/**
 * @file migration.cpp
 * @brief Seyahat Gideri Takibi - Binary → SQLite Migration Implementation
 * 
 * Bu dosya, binary dosyalardan SQLite veritabanına veri migration işlemlerinin
 * implementasyonunu içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/migration.h"
#include "../header/database.h"
#include "../header/fileIO.h"
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <sqlite3.h>

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
    #define ACCESS(path, mode) _access(path, mode)
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #define ACCESS(path, mode) access(path, mode)
#endif

namespace TravelExpense {
    namespace Migration {

        bool checkBinaryFileExists(const char* entityName) {
            if (!entityName) {
                return false;
            }
            std::string filePath = std::string("data/") + entityName + ".dat";
            return (ACCESS(filePath.c_str(), 0) == 0);
        }

        bool readUsersFromBinary(const char* filePath, std::vector<User>& users) {
            if (!filePath) {
                return false;
            }

            users.clear();
            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open()) {
                return false;
            }

            // Dosya boyutunu al
            file.seekg(0, std::ios::end);
            std::streamsize fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            if (fileSize < static_cast<std::streamsize>(sizeof(User))) {
                file.close();
                return false;
            }

            // Binary dosyadan User struct'larını oku
            // Format: Her User struct'ı ardışık olarak kaydedilmiş
            size_t count = static_cast<size_t>(fileSize) / sizeof(User);
            
            for (size_t i = 0; i < count; ++i) {
                User user;
                file.read(reinterpret_cast<char*>(&user), sizeof(User));
                if (file.gcount() == sizeof(User)) {
                    // Geçerli kullanıcı kontrolü (userId > 0 veya username dolu)
                    if (user.userId > 0 || strlen(user.username) > 0) {
                        users.push_back(user);
                    }
                }
            }

            file.close();
            return true;
        }

        bool readTripsFromBinary(const char* filePath, std::vector<Trip>& trips) {
            if (!filePath) {
                return false;
            }

            trips.clear();
            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open()) {
                return false;
            }

            // Dosya boyutunu al
            file.seekg(0, std::ios::end);
            std::streamsize fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            if (fileSize < static_cast<std::streamsize>(sizeof(Trip))) {
                file.close();
                return false;
            }

            // Binary dosyadan Trip struct'larını oku
            size_t count = static_cast<size_t>(fileSize) / sizeof(Trip);
            
            for (size_t i = 0; i < count; ++i) {
                Trip trip;
                file.read(reinterpret_cast<char*>(&trip), sizeof(Trip));
                if (file.gcount() == sizeof(Trip)) {
                    // Geçerli trip kontrolü (tripId > 0 veya userId > 0)
                    if (trip.tripId > 0 || trip.userId > 0) {
                        trips.push_back(trip);
                    }
                }
            }

            file.close();
            return true;
        }

        bool readExpensesFromBinary(const char* filePath, std::vector<Expense>& expenses) {
            if (!filePath) {
                return false;
            }

            expenses.clear();
            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open()) {
                return false;
            }

            // Dosya boyutunu al
            file.seekg(0, std::ios::end);
            std::streamsize fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            if (fileSize < static_cast<std::streamsize>(sizeof(Expense))) {
                file.close();
                return false;
            }

            // Binary dosyadan Expense struct'larını oku
            size_t count = static_cast<size_t>(fileSize) / sizeof(Expense);
            
            for (size_t i = 0; i < count; ++i) {
                Expense expense;
                file.read(reinterpret_cast<char*>(&expense), sizeof(Expense));
                if (file.gcount() == sizeof(Expense)) {
                    // Geçerli expense kontrolü (expenseId > 0 veya tripId > 0)
                    if (expense.expenseId > 0 || expense.tripId > 0) {
                        expenses.push_back(expense);
                    }
                }
            }

            file.close();
            return true;
        }

        bool readBudgetsFromBinary(const char* filePath, std::vector<Budget>& budgets) {
            if (!filePath) {
                return false;
            }

            budgets.clear();
            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open()) {
                return false;
            }

            // Dosya boyutunu al
            file.seekg(0, std::ios::end);
            std::streamsize fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            if (fileSize < static_cast<std::streamsize>(sizeof(Budget))) {
                file.close();
                return false;
            }

            // Binary dosyadan Budget struct'larını oku
            size_t count = static_cast<size_t>(fileSize) / sizeof(Budget);
            
            for (size_t i = 0; i < count; ++i) {
                Budget budget;
                file.read(reinterpret_cast<char*>(&budget), sizeof(Budget));
                if (file.gcount() == sizeof(Budget)) {
                    // Geçerli budget kontrolü (budgetId > 0 veya tripId > 0)
                    if (budget.budgetId > 0 || budget.tripId > 0) {
                        budgets.push_back(budget);
                    }
                }
            }

            file.close();
            return true;
        }

        ErrorCode migrateUsersToDatabase(const std::vector<User>& users, int32_t& migratedCount) {
            migratedCount = 0;
            
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::FileNotFound;
            }

            // Transaction başlat (performans için)
            sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

            for (const auto& user : users) {
                // Kullanıcı zaten var mı kontrol et
                const char* checkSql = "SELECT user_id FROM users WHERE username = ?;";
                sqlite3_stmt* checkStmt = nullptr;
                if (sqlite3_prepare_v2(db, checkSql, -1, &checkStmt, nullptr) == SQLITE_OK) {
                    sqlite3_bind_text(checkStmt, 1, user.username, -1, SQLITE_STATIC);
                    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
                        // Kullanıcı zaten var, atla
                        sqlite3_finalize(checkStmt);
                        continue;
                    }
                    sqlite3_finalize(checkStmt);
                }

                // Kullanıcıyı ekle
                // Not: user_id AUTOINCREMENT olduğu için, NULL veya 0 bırakırsak SQLite otomatik ID üretir
                // Ancak binary dosyalardaki ID'leri korumak için manuel ID insert ediyoruz
                // SQLite AUTOINCREMENT ile manuel ID insert destekler, duplicate kontrolü yaptık
                const char* sql = R"(
                    INSERT INTO users (user_id, username, password_hash, salt, is_guest, created_at, last_login)
                    VALUES (?, ?, ?, ?, ?, ?, ?);
                )";

                sqlite3_stmt* stmt = nullptr;
                if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
                    // ID > 0 ise koru, 0 ise NULL bırak (AUTOINCREMENT kullan)
                    if (user.userId > 0) {
                        sqlite3_bind_int(stmt, 1, user.userId);
                    } else {
                        sqlite3_bind_null(stmt, 1);
                    }
                    sqlite3_bind_text(stmt, 2, user.username, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 3, user.passwordHash, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 4, user.salt, -1, SQLITE_STATIC);
                    sqlite3_bind_int(stmt, 5, user.isGuest ? 1 : 0);
                    sqlite3_bind_int64(stmt, 6, static_cast<sqlite3_int64>(user.createdAt));
                    sqlite3_bind_int64(stmt, 7, static_cast<sqlite3_int64>(user.lastLogin));

                    if (sqlite3_step(stmt) == SQLITE_DONE) {
                        migratedCount++;
                    }
                    sqlite3_finalize(stmt);
                }
            }

            // Transaction commit et
            sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

            return ErrorCode::Success;
        }

        ErrorCode migrateTripsToDatabase(const std::vector<Trip>& trips, int32_t& migratedCount) {
            migratedCount = 0;
            
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::FileNotFound;
            }

            // Transaction başlat
            sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

            for (const auto& trip : trips) {
                // Trip zaten var mı kontrol et
                const char* checkSql = "SELECT trip_id FROM trips WHERE trip_id = ?;";
                sqlite3_stmt* checkStmt = nullptr;
                if (sqlite3_prepare_v2(db, checkSql, -1, &checkStmt, nullptr) == SQLITE_OK) {
                    sqlite3_bind_int(checkStmt, 1, trip.tripId);
                    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
                        // Trip zaten var, atla
                        sqlite3_finalize(checkStmt);
                        continue;
                    }
                    sqlite3_finalize(checkStmt);
                }

                // Trip'i ekle
                const char* sql = R"(
                    INSERT INTO trips (trip_id, user_id, destination, start_date, end_date,
                                     accommodation, transportation, budget, total_expenses,
                                     created_at, updated_at)
                    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
                )";

                sqlite3_stmt* stmt = nullptr;
                if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
                    // ID > 0 ise koru, 0 ise NULL bırak (AUTOINCREMENT kullan)
                    if (trip.tripId > 0) {
                        sqlite3_bind_int(stmt, 1, trip.tripId);
                    } else {
                        sqlite3_bind_null(stmt, 1);
                    }
                    sqlite3_bind_int(stmt, 2, trip.userId);
                    sqlite3_bind_text(stmt, 3, trip.destination, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 4, trip.startDate, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 5, trip.endDate, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 6, trip.accommodation, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 7, trip.transportation, -1, SQLITE_STATIC);
                    sqlite3_bind_double(stmt, 8, trip.budget);
                    sqlite3_bind_double(stmt, 9, trip.totalExpenses);
                    sqlite3_bind_int64(stmt, 10, static_cast<sqlite3_int64>(trip.createdAt));
                    sqlite3_bind_int64(stmt, 11, static_cast<sqlite3_int64>(trip.updatedAt));

                    if (sqlite3_step(stmt) == SQLITE_DONE) {
                        migratedCount++;
                    }
                    sqlite3_finalize(stmt);
                }
            }

            // Transaction commit et
            sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

            return ErrorCode::Success;
        }

        ErrorCode migrateExpensesToDatabase(const std::vector<Expense>& expenses, int32_t& migratedCount) {
            migratedCount = 0;
            
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::FileNotFound;
            }

            // Transaction başlat
            sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

            for (const auto& expense : expenses) {
                // Expense zaten var mı kontrol et
                const char* checkSql = "SELECT expense_id FROM expenses WHERE expense_id = ?;";
                sqlite3_stmt* checkStmt = nullptr;
                if (sqlite3_prepare_v2(db, checkSql, -1, &checkStmt, nullptr) == SQLITE_OK) {
                    sqlite3_bind_int(checkStmt, 1, expense.expenseId);
                    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
                        // Expense zaten var, atla
                        sqlite3_finalize(checkStmt);
                        continue;
                    }
                    sqlite3_finalize(checkStmt);
                }

                // Expense'i ekle
                const char* sql = R"(
                    INSERT INTO expenses (expense_id, trip_id, category, amount, currency, date,
                                         payment_method, description, created_at)
                    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);
                )";

                sqlite3_stmt* stmt = nullptr;
                if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
                    // ID > 0 ise koru, 0 ise NULL bırak (AUTOINCREMENT kullan)
                    if (expense.expenseId > 0) {
                        sqlite3_bind_int(stmt, 1, expense.expenseId);
                    } else {
                        sqlite3_bind_null(stmt, 1);
                    }
                    sqlite3_bind_int(stmt, 2, expense.tripId);
                    sqlite3_bind_int(stmt, 3, static_cast<int>(expense.category));
                    sqlite3_bind_double(stmt, 4, expense.amount);
                    sqlite3_bind_text(stmt, 5, expense.currency, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 6, expense.date, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 7, expense.paymentMethod, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 8, expense.description, -1, SQLITE_STATIC);
                    sqlite3_bind_int64(stmt, 9, static_cast<sqlite3_int64>(expense.createdAt));

                    if (sqlite3_step(stmt) == SQLITE_DONE) {
                        migratedCount++;
                    }
                    sqlite3_finalize(stmt);
                }
            }

            // Transaction commit et
            sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

            return ErrorCode::Success;
        }

        ErrorCode migrateBudgetsToDatabase(const std::vector<Budget>& budgets, int32_t& migratedCount) {
            migratedCount = 0;
            
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::FileNotFound;
            }

            // Transaction başlat
            sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

            for (const auto& budget : budgets) {
                // Budget zaten var mı kontrol et
                const char* checkSql = "SELECT budget_id FROM budgets WHERE budget_id = ?;";
                sqlite3_stmt* checkStmt = nullptr;
                if (sqlite3_prepare_v2(db, checkSql, -1, &checkStmt, nullptr) == SQLITE_OK) {
                    sqlite3_bind_int(checkStmt, 1, budget.budgetId);
                    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
                        // Budget zaten var, atla
                        sqlite3_finalize(checkStmt);
                        continue;
                    }
                    sqlite3_finalize(checkStmt);
                }

                // Budget'i ekle
                const char* sql = R"(
                    INSERT INTO budgets (budget_id, trip_id, total_budget, spent_amount,
                                       category_budget_accommodation, category_budget_transportation,
                                       category_budget_food, category_budget_entertainment,
                                       category_spent_accommodation, category_spent_transportation,
                                       category_spent_food, category_spent_entertainment,
                                       created_at, updated_at)
                    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
                )";

                sqlite3_stmt* stmt = nullptr;
                if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
                    // ID > 0 ise koru, 0 ise NULL bırak (AUTOINCREMENT kullan)
                    if (budget.budgetId > 0) {
                        sqlite3_bind_int(stmt, 1, budget.budgetId);
                    } else {
                        sqlite3_bind_null(stmt, 1);
                    }
                    sqlite3_bind_int(stmt, 2, budget.tripId);
                    sqlite3_bind_double(stmt, 3, budget.totalBudget);
                    sqlite3_bind_double(stmt, 4, budget.spentAmount);
                    sqlite3_bind_double(stmt, 5, budget.categoryBudgets[0]); // ACCOMMODATION
                    sqlite3_bind_double(stmt, 6, budget.categoryBudgets[1]); // TRANSPORTATION
                    sqlite3_bind_double(stmt, 7, budget.categoryBudgets[2]); // FOOD
                    sqlite3_bind_double(stmt, 8, budget.categoryBudgets[3]); // ENTERTAINMENT
                    sqlite3_bind_double(stmt, 9, budget.categorySpent[0]);
                    sqlite3_bind_double(stmt, 10, budget.categorySpent[1]);
                    sqlite3_bind_double(stmt, 11, budget.categorySpent[2]);
                    sqlite3_bind_double(stmt, 12, budget.categorySpent[3]);
                    sqlite3_bind_int64(stmt, 13, static_cast<sqlite3_int64>(budget.createdAt));
                    sqlite3_bind_int64(stmt, 14, static_cast<sqlite3_int64>(budget.updatedAt));

                    if (sqlite3_step(stmt) == SQLITE_DONE) {
                        migratedCount++;
                    }
                    sqlite3_finalize(stmt);
                }
            }

            // Transaction commit et
            sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

            return ErrorCode::Success;
        }

        ErrorCode migrateIdFiles() {
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::FileNotFound;
            }

            // ID dosyalarından maksimum ID'leri al ve veritabanındaki AUTOINCREMENT'i ayarla
            // Not: SQLite'da AUTOINCREMENT'i manuel olarak ayarlayamayız,
            // ama ID dosyalarındaki son ID'ler geçerli kalır
            
            const char* entities[] = {"user", "trip", "expense", "budget", nullptr};
            
            for (int i = 0; entities[i] != nullptr; ++i) {
                int32_t lastId = FileIO::getLastId(entities[i]);
                if (lastId > 0) {
                    // ID dosyasındaki son ID'yi kontrol et
                    // SQLite AUTOINCREMENT zaten doğru çalışır, bu sadece bilgilendirme amaçlı
                    // Eğer gerekirse, manuel olarak ID'leri güncelleyebiliriz
                }
            }

            return ErrorCode::Success;
        }

        bool isMigrationNeeded(const char* dataDir) {
            if (!dataDir) {
                dataDir = "data";
            }

            // Binary dosyaların varlığını kontrol et
            std::string baseDir = std::string(dataDir) + "/";
            const char* binaryFiles[] = {
                "users.dat",
                "trips.dat",
                "expenses.dat",
                "budgets.dat",
                nullptr
            };

            for (int i = 0; binaryFiles[i] != nullptr; ++i) {
                std::string filePath = baseDir + binaryFiles[i];
                if (ACCESS(filePath.c_str(), 0) == 0) {
                    return true; // En az bir binary dosya var
                }
            }

            return false; // Hiç binary dosya yok
        }

        ErrorCode migrateAllFromBinary(const char* dataDir, MigrationResult& result) {
            // Sonuç yapısını sıfırla
            result.usersMigrated = 0;
            result.tripsMigrated = 0;
            result.expensesMigrated = 0;
            result.budgetsMigrated = 0;
            result.errors = 0;
            result.success = true;

            if (!dataDir) {
                dataDir = "data";
            }

            // Veritabanı bağlantısını kontrol et
            sqlite3* db = Database::getDatabase();
            if (!db) {
                // Veritabanını başlat
                db = Database::initializeDatabase();
                if (!db) {
                    result.success = false;
                    result.errors++;
                    return ErrorCode::FileNotFound;
                }
            }

            // Tabloları oluştur (yoksa)
            ErrorCode rc = Database::createTables(db);
            if (rc != ErrorCode::Success) {
                result.success = false;
                result.errors++;
            }

            std::string baseDir = std::string(dataDir) + "/";

            // 1. Users migration
            std::string usersFile = baseDir + "users.dat";
            if (ACCESS(usersFile.c_str(), 0) == 0) {
                std::vector<User> users;
                if (readUsersFromBinary(usersFile.c_str(), users)) {
                    int32_t migrated = 0;
                    rc = migrateUsersToDatabase(users, migrated);
                    if (rc == ErrorCode::Success) {
                        result.usersMigrated = migrated;
                    } else {
                        result.errors++;
                    }
                } else {
                    result.errors++;
                }
            }

            // 2. Trips migration
            std::string tripsFile = baseDir + "trips.dat";
            if (ACCESS(tripsFile.c_str(), 0) == 0) {
                std::vector<Trip> trips;
                if (readTripsFromBinary(tripsFile.c_str(), trips)) {
                    int32_t migrated = 0;
                    rc = migrateTripsToDatabase(trips, migrated);
                    if (rc == ErrorCode::Success) {
                        result.tripsMigrated = migrated;
                    } else {
                        result.errors++;
                    }
                } else {
                    result.errors++;
                }
            }

            // 3. Expenses migration
            std::string expensesFile = baseDir + "expenses.dat";
            if (ACCESS(expensesFile.c_str(), 0) == 0) {
                std::vector<Expense> expenses;
                if (readExpensesFromBinary(expensesFile.c_str(), expenses)) {
                    int32_t migrated = 0;
                    rc = migrateExpensesToDatabase(expenses, migrated);
                    if (rc == ErrorCode::Success) {
                        result.expensesMigrated = migrated;
                    } else {
                        result.errors++;
                    }
                } else {
                    result.errors++;
                }
            }

            // 4. Budgets migration
            std::string budgetsFile = baseDir + "budgets.dat";
            if (ACCESS(budgetsFile.c_str(), 0) == 0) {
                std::vector<Budget> budgets;
                if (readBudgetsFromBinary(budgetsFile.c_str(), budgets)) {
                    int32_t migrated = 0;
                    rc = migrateBudgetsToDatabase(budgets, migrated);
                    if (rc == ErrorCode::Success) {
                        result.budgetsMigrated = migrated;
                    } else {
                        result.errors++;
                    }
                } else {
                    result.errors++;
                }
            }

            // 5. ID dosyalarını migrate et
            migrateIdFiles();

            // Genel başarı durumu
            if (result.errors > 0) {
                result.success = false;
            }

            return (result.success) ? ErrorCode::Success : ErrorCode::FileIO;
        }

    } // namespace Migration

} // namespace TravelExpense

