/**
 * @file expenseManager.cpp
 * @brief Seyahat Gideri Takibi - Gider Yönetimi Implementation
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/expenseManager.h"
#include "../header/database.h"
#include "../header/safe_string.h"
#include <sqlite3.h>
#include <cstring>
#include <ctime>

namespace TravelExpense {

namespace ExpenseManager {

ErrorCode logExpense(const Expense &expense, int32_t &expenseId) {
  if (expense.tripId <= 0 || expense.amount <= 0) {
    return ErrorCode::InvalidInput;
  }

  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                INSERT INTO expenses (trip_id, category, amount, currency, date, 
                                     payment_method, description, created_at)
                VALUES (?, ?, ?, ?, ?, ?, ?, ?);
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  // Parametreleri bağla
  time_t now = time(nullptr);
  sqlite3_bind_int(stmt, 1, expense.tripId);
  sqlite3_bind_int(stmt, 2, static_cast<int>(expense.category));
  sqlite3_bind_double(stmt, 3, expense.amount);
  sqlite3_bind_text(stmt, 4, expense.currency, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, expense.date, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 6, expense.paymentMethod, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 7, expense.description, -1, SQLITE_STATIC);
  sqlite3_bind_int64(stmt, 8, static_cast<sqlite3_int64>(now));
  // Sorguyu çalıştır
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return ErrorCode::FileIO;
  }

  // Oluşturulan ID'yi al
  expenseId = static_cast<int32_t>(Database::getLastInsertRowId(db));
  sqlite3_finalize(stmt);
  return ErrorCode::Success;
}

ErrorCode getExpenses(int32_t tripId, std::vector<Expense> &expenses) {
  expenses.clear();
  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                SELECT expense_id, trip_id, category, amount, currency, date,
                       payment_method, description, created_at
                FROM expenses WHERE trip_id = ?;
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  // Parametreleri bağla
  sqlite3_bind_int(stmt, 1, tripId);

  // Sorguyu çalıştır ve sonuçları al
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    Expense expense;
    expense.expenseId = sqlite3_column_int(stmt, 0);
    expense.tripId = sqlite3_column_int(stmt, 1);
    expense.category = static_cast<ExpenseCategory>(sqlite3_column_int(stmt, 2));
    expense.amount = sqlite3_column_double(stmt, 3);
    expense.createdAt = sqlite3_column_int64(stmt, 8);
    const char *currency = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    const char *date = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
    const char *paymentMethod = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
    const char *description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
    SafeString::safeCopy(expense.currency, sizeof(expense.currency), currency ? currency : "TRY");
    SafeString::safeCopy(expense.date, sizeof(expense.date), date ? date : "");
    SafeString::safeCopy(expense.paymentMethod, sizeof(expense.paymentMethod), paymentMethod ? paymentMethod : "");
    SafeString::safeCopy(expense.description, sizeof(expense.description), description ? description : "");
    expenses.push_back(expense);
  }

  sqlite3_finalize(stmt);
  return (rc == SQLITE_DONE) ? ErrorCode::Success : ErrorCode::FileIO;
}

ErrorCode updateExpense(int32_t expenseId, const Expense &expense) {
  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                UPDATE expenses 
                SET trip_id = ?, category = ?, amount = ?, currency = ?, date = ?,
                    payment_method = ?, description = ?
                WHERE expense_id = ?;
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  // Parametreleri bağla
  sqlite3_bind_int(stmt, 1, expense.tripId);
  sqlite3_bind_int(stmt, 2, static_cast<int>(expense.category));
  sqlite3_bind_double(stmt, 3, expense.amount);
  sqlite3_bind_text(stmt, 4, expense.currency, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, expense.date, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 6, expense.paymentMethod, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 7, expense.description, -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 8, expenseId);
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

ErrorCode deleteExpense(int32_t expenseId) {
  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = "DELETE FROM expenses WHERE expense_id = ?;";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  // Parametreleri bağla
  sqlite3_bind_int(stmt, 1, expenseId);
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

ErrorCode getExpense(int32_t expenseId, Expense &expense) {
  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                SELECT expense_id, trip_id, category, amount, currency, date,
                       payment_method, description, created_at
                FROM expenses WHERE expense_id = ?;
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  // Parametreleri bağla
  sqlite3_bind_int(stmt, 1, expenseId);
  // Sorguyu çalıştır
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return ErrorCode::InvalidInput;
  }

  // Sonuçları al
  expense.expenseId = sqlite3_column_int(stmt, 0);
  expense.tripId = sqlite3_column_int(stmt, 1);
  expense.category = static_cast<ExpenseCategory>(sqlite3_column_int(stmt, 2));
  expense.amount = sqlite3_column_double(stmt, 3);
  expense.createdAt = sqlite3_column_int64(stmt, 8);
  const char *currency = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
  const char *date = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
  const char *paymentMethod = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
  const char *description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
  SafeString::safeCopy(expense.currency, sizeof(expense.currency), currency ? currency : "TRY");
  SafeString::safeCopy(expense.date, sizeof(expense.date), date ? date : "");
  SafeString::safeCopy(expense.paymentMethod, sizeof(expense.paymentMethod), paymentMethod ? paymentMethod : "");
  SafeString::safeCopy(expense.description, sizeof(expense.description), description ? description : "");
  sqlite3_finalize(stmt);
  return ErrorCode::Success;
}
}

} // namespace TravelExpense
