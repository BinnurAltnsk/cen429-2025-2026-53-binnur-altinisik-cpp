/**
 * @file database.cpp
 * @brief Seyahat Gideri Takibi - SQLite Veritabanı Yönetimi Implementation
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/database.h"
#include "../header/fileIO.h"
#include <sqlite3.h>
#include <cstring>
#include <cstdio>

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
    #define MKDIR(dir) _mkdir(dir.c_str())
    #define ACCESS(path, mode) _access(path, mode)
#else
    #include <sys/stat.h>
    #include <unistd.h>
    #define MKDIR(dir) mkdir(dir.c_str(), 0755)
    #define ACCESS(path, mode) access(path, mode)
#endif

namespace TravelExpense {

    namespace Database {

        // Global veritabanı handle'ı (singleton)
        static sqlite3* g_database = nullptr;

        sqlite3* initializeDatabase(const char* dbPath) {
            // Veritabanı dizinini oluştur
            FileIO::ensureDataDirectory();

            std::string path;
            if (dbPath) {
                path = dbPath;
            } else {
                path = "data/travelexpense.db";
            }

            sqlite3* db = nullptr;
            int rc = sqlite3_open(path.c_str(), &db);

            if (rc != SQLITE_OK) {
                if (db) {
                    sqlite3_close(db);
                }
                return nullptr;
            }

            // Foreign keys'i etkinleştir
            sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

            return db;
        }

        ErrorCode closeDatabase(sqlite3* db) {
            if (!db) {
                return ErrorCode::InvalidInput;
            }

            int rc = sqlite3_close(db);
            if (rc != SQLITE_OK) {
                return ErrorCode::FileIO;
            }

            return ErrorCode::Success;
        }

        ErrorCode createTables(sqlite3* db) {
            if (!db) {
                return ErrorCode::InvalidInput;
            }

            char* errMsg = nullptr;
            int rc;

            // Users tablosu
            const char* createUsersTable = R"(
                CREATE TABLE IF NOT EXISTS users (
                    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    username TEXT NOT NULL UNIQUE,
                    password_hash TEXT NOT NULL,
                    salt TEXT NOT NULL,
                    is_guest INTEGER NOT NULL DEFAULT 0,
                    created_at INTEGER NOT NULL,
                    last_login INTEGER NOT NULL DEFAULT 0
                );
            )";

            rc = sqlite3_exec(db, createUsersTable, nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                if (errMsg) {
                    sqlite3_free(errMsg);
                }
                return ErrorCode::FileIO;
            }

            // Trips tablosu
            const char* createTripsTable = R"(
                CREATE TABLE IF NOT EXISTS trips (
                    trip_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    user_id INTEGER NOT NULL,
                    destination TEXT NOT NULL,
                    start_date TEXT NOT NULL,
                    end_date TEXT NOT NULL,
                    accommodation TEXT,
                    transportation TEXT,
                    budget REAL NOT NULL DEFAULT 0.0,
                    total_expenses REAL NOT NULL DEFAULT 0.0,
                    created_at INTEGER NOT NULL,
                    updated_at INTEGER NOT NULL,
                    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE
                );
            )";

            rc = sqlite3_exec(db, createTripsTable, nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                if (errMsg) {
                    sqlite3_free(errMsg);
                }
                return ErrorCode::FileIO;
            }

            // Expenses tablosu
            const char* createExpensesTable = R"(
                CREATE TABLE IF NOT EXISTS expenses (
                    expense_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    trip_id INTEGER NOT NULL,
                    category INTEGER NOT NULL,
                    amount REAL NOT NULL,
                    currency TEXT NOT NULL DEFAULT 'TRY',
                    date TEXT NOT NULL,
                    payment_method TEXT,
                    description TEXT,
                    created_at INTEGER NOT NULL,
                    FOREIGN KEY (trip_id) REFERENCES trips(trip_id) ON DELETE CASCADE
                );
            )";

            rc = sqlite3_exec(db, createExpensesTable, nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                if (errMsg) {
                    sqlite3_free(errMsg);
                }
                return ErrorCode::FileIO;
            }

            // Budgets tablosu
            const char* createBudgetsTable = R"(
                CREATE TABLE IF NOT EXISTS budgets (
                    budget_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    trip_id INTEGER NOT NULL UNIQUE,
                    total_budget REAL NOT NULL,
                    spent_amount REAL NOT NULL DEFAULT 0.0,
                    category_budget_accommodation REAL NOT NULL DEFAULT 0.0,
                    category_budget_transportation REAL NOT NULL DEFAULT 0.0,
                    category_budget_food REAL NOT NULL DEFAULT 0.0,
                    category_budget_entertainment REAL NOT NULL DEFAULT 0.0,
                    category_spent_accommodation REAL NOT NULL DEFAULT 0.0,
                    category_spent_transportation REAL NOT NULL DEFAULT 0.0,
                    category_spent_food REAL NOT NULL DEFAULT 0.0,
                    category_spent_entertainment REAL NOT NULL DEFAULT 0.0,
                    created_at INTEGER NOT NULL,
                    updated_at INTEGER NOT NULL,
                    FOREIGN KEY (trip_id) REFERENCES trips(trip_id) ON DELETE CASCADE
                );
            )";

            rc = sqlite3_exec(db, createBudgetsTable, nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                if (errMsg) {
                    sqlite3_free(errMsg);
                }
                return ErrorCode::FileIO;
            }

            // Assets tablosu (Varlık Yönetimi)
            const char* createAssetsTable = R"(
                CREATE TABLE IF NOT EXISTS assets (
                    asset_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    asset_type INTEGER NOT NULL,
                    name TEXT NOT NULL,
                    description TEXT,
                    location TEXT NOT NULL,
                    source TEXT,
                    size INTEGER NOT NULL DEFAULT 0,
                    created_at INTEGER NOT NULL,
                    deleted_at INTEGER NOT NULL DEFAULT 0,
                    default_value TEXT,
                    protection_scheme INTEGER NOT NULL DEFAULT 0,
                    is_encrypted INTEGER NOT NULL DEFAULT 0,
                    is_active INTEGER NOT NULL DEFAULT 1
                );
            )";

            rc = sqlite3_exec(db, createAssetsTable, nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                if (errMsg) {
                    sqlite3_free(errMsg);
                }
                return ErrorCode::FileIO;
            }

            // Index'ler oluştur
            const char* createIndexes = R"(
                CREATE INDEX IF NOT EXISTS idx_trips_user_id ON trips(user_id);
                CREATE INDEX IF NOT EXISTS idx_expenses_trip_id ON expenses(trip_id);
                CREATE INDEX IF NOT EXISTS idx_budgets_trip_id ON budgets(trip_id);
                CREATE INDEX IF NOT EXISTS idx_assets_type ON assets(asset_type);
                CREATE INDEX IF NOT EXISTS idx_assets_active ON assets(is_active, deleted_at);
            )";

            rc = sqlite3_exec(db, createIndexes, nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                if (errMsg) {
                    sqlite3_free(errMsg);
                }
                // Index hatası kritik değil
            }

            return ErrorCode::Success;
        }

        sqlite3* getDatabase() {
            if (!g_database) {
                g_database = initializeDatabase();
                if (g_database) {
                    createTables(g_database);
                }
            }
            return g_database;
        }

        ErrorCode resetDatabase() {
            if (g_database) {
                closeDatabase(g_database);
                g_database = nullptr;
            }

            g_database = initializeDatabase();
            if (!g_database) {
                return ErrorCode::FileIO;
            }

            return createTables(g_database);
        }

        ErrorCode executeQuery(sqlite3* db, const char* sql) {
            if (!db || !sql) {
                return ErrorCode::InvalidInput;
            }

            char* errMsg = nullptr;
            int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

            if (rc != SQLITE_OK) {
                if (errMsg) {
                    sqlite3_free(errMsg);
                }
                return ErrorCode::FileIO;
            }

            return ErrorCode::Success;
        }

        int64_t getLastInsertRowId(sqlite3* db) {
            if (!db) {
                return 0;
            }
            return sqlite3_last_insert_rowid(db);
        }

    } // namespace Database

} // namespace TravelExpense

