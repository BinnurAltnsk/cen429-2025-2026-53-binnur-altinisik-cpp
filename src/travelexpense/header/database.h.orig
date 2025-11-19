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

namespace TravelExpense { // LCOV_EXCL_LINE

    /**
     * @namespace Database
     * @brief SQLite veritabanı yönetimi modülü
     * 
     * Bu modül, SQLite veritabanı bağlantı yönetimi ve temel işlemlerini sağlar.
     * Singleton pattern kullanarak tek bir veritabanı bağlantısı yönetir.
     * Veritabanı tablolarının oluşturulması, sorgu çalıştırma ve bağlantı
     * yönetimi işlemlerini yönetir.
     */
    namespace Database {
        /**
         * @brief Veritabanı bağlantısını başlat
         * 
         * SQLite veritabanı dosyasına bağlantı açar. Dosya yoksa oluşturur.
         * Veritabanı dosyası, belirtilen yolda oluşturulur veya açılır.
         * 
         * @note Bu fonksiyon, veritabanı dosyasının bulunduğu dizini oluşturmaz.
         * Dizin önceden oluşturulmuş olmalıdır. Eğer dizin yoksa, hata döner.
         * 
         * @param dbPath Veritabanı dosya yolu (nullptr ise "data/travelexpense.db" kullanılır)
         *                Dosya yolu geçerli olmalı ve dizin mevcut olmalı.
         * @return sqlite3* Veritabanı handle'ı, nullptr ise hata (dosya açılamadı, izin hatası vb.)
         */
        TRAVELEXPENSE_API sqlite3* initializeDatabase(const char* dbPath = nullptr);

        /**
         * @brief Veritabanı bağlantısını kapat
         * 
         * Açık olan SQLite veritabanı bağlantısını kapatır ve kaynakları temizler.
         * Tüm prepared statement'lar ve transaction'lar kapatılır.
         * 
         * @note Bu fonksiyon, nullptr handle'ı geçerli kabul eder ve Success döner.
         * Bağlantı kapatıldıktan sonra, handle geçersiz hale gelir.
         * 
         * @param db Veritabanı handle'ı (nullptr ise hiçbir işlem yapılmaz)
         * @return ErrorCode Başarı durumu (Success, FileIO vb.)
         */
        TRAVELEXPENSE_API ErrorCode closeDatabase(sqlite3* db);

        /**
         * @brief Veritabanı tablolarını oluştur (migration)
         * 
         * Veritabanında gerekli tabloları oluşturur (users, trips, expenses, budgets).
         * Tablolar zaten varsa hata vermez (IF NOT EXISTS kullanılır).
         * 
         * @note Bu fonksiyon, veritabanı şemasını initialize eder.
         * Her tablo için CREATE TABLE IF NOT EXISTS komutu çalıştırılır.
         * 
         * @param db Veritabanı handle'ı (nullptr ise InvalidInput döner)
         * @return ErrorCode Başarı durumu (Success, InvalidInput, FileIO vb.)
         */
        TRAVELEXPENSE_API ErrorCode createTables(sqlite3* db);

        /**
         * @brief Veritabanı handle'ını al (singleton pattern)
         * 
         * Mevcut veritabanı bağlantısını döndürür. Eğer bağlantı yoksa
         * yeni bir bağlantı oluşturur (initializeDatabase() çağrılır).
         * 
         * @note Bu fonksiyon, singleton pattern kullanarak tek bir veritabanı
         * bağlantısı yönetir. İlk çağrıda bağlantı oluşturulur.
         * 
         * @return sqlite3* Veritabanı handle'ı (nullptr ise bağlantı oluşturulamadı)
         */
        TRAVELEXPENSE_API sqlite3* getDatabase();

        /**
         * @brief Veritabanı bağlantısını sıfırla (yeniden bağlan)
         * 
         * Mevcut bağlantıyı kapatır ve yeni bir bağlantı açar.
         * Singleton pattern'deki bağlantıyı yeniden başlatır.
         * 
         * @note Bu fonksiyon, veritabanı bağlantısını yeniden başlatmak için kullanılır.
         * Tüm açık transaction'lar ve prepared statement'lar kapatılır.
         * 
         * @return ErrorCode Başarı durumu (Success, FileIO, FileNotFound vb.)
         */
        TRAVELEXPENSE_API ErrorCode resetDatabase();

        /**
         * @brief SQL sorgusu çalıştır (basit)
         * 
         * Basit SQL sorgularını (INSERT, UPDATE, DELETE gibi) çalıştırır.
         * Sonuç döndürmeyen sorgular için kullanılır.
         * 
         * @note Bu fonksiyon, sqlite3_exec() kullanarak sorguyu çalıştırır.
         * SELECT sorguları için kullanılmaz (sonuç döndürmez).
         * Prepared statement kullanmak daha güvenlidir.
         * 
         * @param db Veritabanı handle'ı (nullptr ise InvalidInput döner)
         * @param sql SQL sorgusu (nullptr ise InvalidInput döner, null-terminated string olmalı)
         * @return ErrorCode Başarı durumu (Success, InvalidInput, FileIO vb.)
         */
        TRAVELEXPENSE_API ErrorCode executeQuery(sqlite3* db, const char* sql);

        /**
         * @brief Son eklenen satırın ID'sini al
         * 
         * Son INSERT işleminde eklenen satırın auto-increment ID'sini döndürür.
         * SQLite'in last_insert_rowid() fonksiyonunu kullanır.
         * 
         * @note Bu fonksiyon, son INSERT işleminden sonra çağrılmalıdır.
         * Eğer INSERT işlemi yapılmadıysa veya hata varsa, 0 döner.
         * 
         * @param db Veritabanı handle'ı (nullptr ise 0 döner)
         * @return int64_t Son eklenen satır ID'si, 0 ise hata veya satır eklenmemiş
         */
        TRAVELEXPENSE_API int64_t getLastInsertRowId(sqlite3* db);
    }

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // DATABASE_H

