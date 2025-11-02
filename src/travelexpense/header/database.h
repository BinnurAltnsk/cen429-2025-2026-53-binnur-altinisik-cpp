/**
 * @file database.h
 * @brief Seyahat Gideri Takibi - SQLite Veritabanı Yönetimi
 * 
 * Bu dosya, SQLite veritabanı bağlantı ve yönetim fonksiyonlarının
 * bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef DATABASE_H
#define DATABASE_H

#include "commonTypes.h"
#include "export.h"
#include <sqlite3.h>
#include <string>

namespace TravelExpense {

    /**
     * @namespace Database
     * @brief SQLite veritabanı yönetimi modülü
     */
    namespace Database {
        /**
         * @brief Veritabanı bağlantısını başlat
         * 
         * @param dbPath Veritabanı dosya yolu (nullptr ise "data/travelexpense.db" kullanılır)
         * @return sqlite3* Veritabanı handle'ı, nullptr ise hata
         */
        TRAVELEXPENSE_API sqlite3* initializeDatabase(const char* dbPath = nullptr);

        /**
         * @brief Veritabanı bağlantısını kapat
         * 
         * @param db Veritabanı handle'ı
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode closeDatabase(sqlite3* db);

        /**
         * @brief Veritabanı tablolarını oluştur (migration)
         * 
         * @param db Veritabanı handle'ı
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode createTables(sqlite3* db);

        /**
         * @brief Veritabanı handle'ını al (singleton pattern)
         * 
         * @return sqlite3* Veritabanı handle'ı
         */
        TRAVELEXPENSE_API sqlite3* getDatabase();

        /**
         * @brief Veritabanı bağlantısını sıfırla (yeniden bağlan)
         * 
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode resetDatabase();

        /**
         * @brief SQL sorgusu çalıştır (basit)
         * 
         * @param db Veritabanı handle'ı
         * @param sql SQL sorgusu
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode executeQuery(sqlite3* db, const char* sql);

        /**
         * @brief Son eklenen satırın ID'sini al
         * 
         * @param db Veritabanı handle'ı
         * @return int64_t Son eklenen satır ID'si
         */
        TRAVELEXPENSE_API int64_t getLastInsertRowId(sqlite3* db);
    }

} // namespace TravelExpense

#endif // DATABASE_H

