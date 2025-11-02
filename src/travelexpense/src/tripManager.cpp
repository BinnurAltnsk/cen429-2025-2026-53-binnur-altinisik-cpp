/**
 * @file tripManager.cpp
 * @brief Seyahat Gideri Takibi - Seyahat Yönetimi Implementation
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/tripManager.h"
#include "../header/database.h"
#include <sqlite3.h>
#include <cstring>
#include <ctime>

namespace TravelExpense {

    namespace TripManager {

        ErrorCode createTrip(const Trip& trip, int32_t& tripId) {
            if (trip.userId <= 0) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            // SQLite veritabanını al
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            // SQL sorgusu hazırla
            const char* sql = R"(
                INSERT INTO trips (user_id, destination, start_date, end_date, 
                                 accommodation, transportation, budget, total_expenses, 
                                 created_at, updated_at)
                VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
            )";

            sqlite3_stmt* stmt = nullptr;
            int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                return ErrorCode::ERROR_FILE_IO;
            }

            // Parametreleri bağla
            time_t now = time(nullptr);
            sqlite3_bind_int(stmt, 1, trip.userId);
            sqlite3_bind_text(stmt, 2, trip.destination, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, trip.startDate, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, trip.endDate, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 5, trip.accommodation, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 6, trip.transportation, -1, SQLITE_STATIC);
            sqlite3_bind_double(stmt, 7, trip.budget);
            sqlite3_bind_double(stmt, 8, trip.totalExpenses);
            sqlite3_bind_int64(stmt, 9, static_cast<sqlite3_int64>(now));
            sqlite3_bind_int64(stmt, 10, static_cast<sqlite3_int64>(now));

            // Sorguyu çalıştır
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                sqlite3_finalize(stmt);
                return ErrorCode::ERROR_FILE_IO;
            }

            // Oluşturulan ID'yi al
            tripId = static_cast<int32_t>(Database::getLastInsertRowId(db));

            sqlite3_finalize(stmt);

            return ErrorCode::SUCCESS;
        }

        ErrorCode getTrips(int32_t userId, std::vector<Trip>& trips) {
            trips.clear();
            
            // SQLite veritabanını al
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            // SQL sorgusu hazırla
            const char* sql = R"(
                SELECT trip_id, user_id, destination, start_date, end_date,
                       accommodation, transportation, budget, total_expenses,
                       created_at, updated_at
                FROM trips WHERE user_id = ?;
            )";

            sqlite3_stmt* stmt = nullptr;
            int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                return ErrorCode::ERROR_FILE_IO;
            }

            // Parametreleri bağla
            sqlite3_bind_int(stmt, 1, userId);

            // Sorguyu çalıştır ve sonuçları al
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                Trip trip;
                trip.tripId = sqlite3_column_int(stmt, 0);
                trip.userId = sqlite3_column_int(stmt, 1);
                
                const char* dest = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                const char* startDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                const char* endDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                const char* accommodation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                const char* transportation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
                
                trip.budget = sqlite3_column_double(stmt, 7);
                trip.totalExpenses = sqlite3_column_double(stmt, 8);
                trip.createdAt = sqlite3_column_int64(stmt, 9);
                trip.updatedAt = sqlite3_column_int64(stmt, 10);

                strncpy(trip.destination, dest ? dest : "", sizeof(trip.destination) - 1);
                trip.destination[sizeof(trip.destination) - 1] = '\0';
                strncpy(trip.startDate, startDate ? startDate : "", sizeof(trip.startDate) - 1);
                trip.startDate[sizeof(trip.startDate) - 1] = '\0';
                strncpy(trip.endDate, endDate ? endDate : "", sizeof(trip.endDate) - 1);
                trip.endDate[sizeof(trip.endDate) - 1] = '\0';
                strncpy(trip.accommodation, accommodation ? accommodation : "", sizeof(trip.accommodation) - 1);
                trip.accommodation[sizeof(trip.accommodation) - 1] = '\0';
                strncpy(trip.transportation, transportation ? transportation : "", sizeof(trip.transportation) - 1);
                trip.transportation[sizeof(trip.transportation) - 1] = '\0';

                trips.push_back(trip);
            }

            sqlite3_finalize(stmt);

            return (rc == SQLITE_DONE) ? ErrorCode::SUCCESS : ErrorCode::ERROR_FILE_IO;
        }

        ErrorCode updateTrip(int32_t tripId, const Trip& trip) {
            // SQLite veritabanını al
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            // SQL sorgusu hazırla
            const char* sql = R"(
                UPDATE trips 
                SET user_id = ?, destination = ?, start_date = ?, end_date = ?,
                    accommodation = ?, transportation = ?, budget = ?, 
                    total_expenses = ?, updated_at = ?
                WHERE trip_id = ?;
            )";

            sqlite3_stmt* stmt = nullptr;
            int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                return ErrorCode::ERROR_FILE_IO;
            }

            // Parametreleri bağla
            time_t now = time(nullptr);
            sqlite3_bind_int(stmt, 1, trip.userId);
            sqlite3_bind_text(stmt, 2, trip.destination, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, trip.startDate, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, trip.endDate, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 5, trip.accommodation, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 6, trip.transportation, -1, SQLITE_STATIC);
            sqlite3_bind_double(stmt, 7, trip.budget);
            sqlite3_bind_double(stmt, 8, trip.totalExpenses);
            sqlite3_bind_int64(stmt, 9, static_cast<sqlite3_int64>(now));
            sqlite3_bind_int(stmt, 10, tripId);

            // Sorguyu çalıştır
            rc = sqlite3_step(stmt);
            sqlite3_finalize(stmt);

            if (rc != SQLITE_DONE) {
                return ErrorCode::ERROR_FILE_IO;
            }

            // Etkilenen satır sayısını kontrol et
            if (sqlite3_changes(db) == 0) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            return ErrorCode::SUCCESS;
        }

        ErrorCode deleteTrip(int32_t tripId) {
            // SQLite veritabanını al
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            // SQL sorgusu hazırla (CASCADE ile bağlı kayıtlar da silinir)
            const char* sql = "DELETE FROM trips WHERE trip_id = ?;";

            sqlite3_stmt* stmt = nullptr;
            int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                return ErrorCode::ERROR_FILE_IO;
            }

            // Parametreleri bağla
            sqlite3_bind_int(stmt, 1, tripId);

            // Sorguyu çalıştır
            rc = sqlite3_step(stmt);
            sqlite3_finalize(stmt);

            if (rc != SQLITE_DONE) {
                return ErrorCode::ERROR_FILE_IO;
            }

            // Etkilenen satır sayısını kontrol et
            if (sqlite3_changes(db) == 0) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            return ErrorCode::SUCCESS;
        }

        ErrorCode getTrip(int32_t tripId, Trip& trip) {
            // SQLite veritabanını al
            sqlite3* db = Database::getDatabase();
            if (!db) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            // SQL sorgusu hazırla
            const char* sql = R"(
                SELECT trip_id, user_id, destination, start_date, end_date,
                       accommodation, transportation, budget, total_expenses,
                       created_at, updated_at
                FROM trips WHERE trip_id = ?;
            )";

            sqlite3_stmt* stmt = nullptr;
            int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                return ErrorCode::ERROR_FILE_IO;
            }

            // Parametreleri bağla
            sqlite3_bind_int(stmt, 1, tripId);

            // Sorguyu çalıştır
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_ROW) {
                sqlite3_finalize(stmt);
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            // Sonuçları al
            trip.tripId = sqlite3_column_int(stmt, 0);
            trip.userId = sqlite3_column_int(stmt, 1);
            
            const char* dest = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            const char* startDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            const char* endDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            const char* accommodation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            const char* transportation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
            
            trip.budget = sqlite3_column_double(stmt, 7);
            trip.totalExpenses = sqlite3_column_double(stmt, 8);
            trip.createdAt = sqlite3_column_int64(stmt, 9);
            trip.updatedAt = sqlite3_column_int64(stmt, 10);

            strncpy(trip.destination, dest ? dest : "", sizeof(trip.destination) - 1);
            trip.destination[sizeof(trip.destination) - 1] = '\0';
            strncpy(trip.startDate, startDate ? startDate : "", sizeof(trip.startDate) - 1);
            trip.startDate[sizeof(trip.startDate) - 1] = '\0';
            strncpy(trip.endDate, endDate ? endDate : "", sizeof(trip.endDate) - 1);
            trip.endDate[sizeof(trip.endDate) - 1] = '\0';
            strncpy(trip.accommodation, accommodation ? accommodation : "", sizeof(trip.accommodation) - 1);
            trip.accommodation[sizeof(trip.accommodation) - 1] = '\0';
            strncpy(trip.transportation, transportation ? transportation : "", sizeof(trip.transportation) - 1);
            trip.transportation[sizeof(trip.transportation) - 1] = '\0';

            sqlite3_finalize(stmt);

            return ErrorCode::SUCCESS;
        }
    }

} // namespace TravelExpense

