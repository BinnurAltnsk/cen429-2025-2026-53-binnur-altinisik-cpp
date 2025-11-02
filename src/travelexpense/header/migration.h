/**
 * @file migration.h
 * @brief Seyahat Gideri Takibi - Binary → SQLite Migration Fonksiyonları
 * 
 * Bu dosya, binary dosyalardan SQLite veritabanına veri migration işlemlerinin
 * bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef MIGRATION_H
#define MIGRATION_H

#include "commonTypes.h"
#include "export.h"
#include <cstdint>
#include <vector>

namespace TravelExpense {

    /**
     * @namespace Migration
     * @brief Binary → SQLite migration işlemleri modülü
     */
    namespace Migration {

        /**
         * @brief Migration sonuç yapısı
         */
        struct MigrationResult {
            int32_t usersMigrated;
            int32_t tripsMigrated;
            int32_t expensesMigrated;
            int32_t budgetsMigrated;
            int32_t errors;
            bool success;
        };

        /**
         * @brief Binary dosya formatını kontrol et
         * 
         * @param entityName Entity adı (users, trips, expenses, budgets)
         * @return true Binary dosya mevcut, false Mevcut değil
         */
        TRAVELEXPENSE_API bool checkBinaryFileExists(const char* entityName);

        /**
         * @brief Binary dosyadan User verilerini oku
         * 
         * @param filePath Binary dosya yolu
         * @param users Okunan User verileri
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool readUsersFromBinary(const char* filePath, std::vector<User>& users);

        /**
         * @brief Binary dosyadan Trip verilerini oku
         * 
         * @param filePath Binary dosya yolu
         * @param trips Okunan Trip verileri
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool readTripsFromBinary(const char* filePath, std::vector<Trip>& trips);

        /**
         * @brief Binary dosyadan Expense verilerini oku
         * 
         * @param filePath Binary dosya yolu
         * @param expenses Okunan Expense verileri
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool readExpensesFromBinary(const char* filePath, std::vector<Expense>& expenses);

        /**
         * @brief Binary dosyadan Budget verilerini oku
         * 
         * @param filePath Binary dosya yolu
         * @param budgets Okunan Budget verileri
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool readBudgetsFromBinary(const char* filePath, std::vector<Budget>& budgets);

        /**
         * @brief Users verilerini SQLite veritabanına migrate et
         * 
         * @param users Migrate edilecek User verileri
         * @param migratedCount Migrate edilen kayıt sayısı
         * @return ErrorCode İşlem sonucu
         */
        TRAVELEXPENSE_API ErrorCode migrateUsersToDatabase(const std::vector<User>& users, int32_t& migratedCount);

        /**
         * @brief Trips verilerini SQLite veritabanına migrate et
         * 
         * @param trips Migrate edilecek Trip verileri
         * @param migratedCount Migrate edilen kayıt sayısı
         * @return ErrorCode İşlem sonucu
         */
        TRAVELEXPENSE_API ErrorCode migrateTripsToDatabase(const std::vector<Trip>& trips, int32_t& migratedCount);

        /**
         * @brief Expenses verilerini SQLite veritabanına migrate et
         * 
         * @param expenses Migrate edilecek Expense verileri
         * @param migratedCount Migrate edilen kayıt sayısı
         * @return ErrorCode İşlem sonucu
         */
        TRAVELEXPENSE_API ErrorCode migrateExpensesToDatabase(const std::vector<Expense>& expenses, int32_t& migratedCount);

        /**
         * @brief Budgets verilerini SQLite veritabanına migrate et
         * 
         * @param budgets Migrate edilecek Budget verileri
         * @param migratedCount Migrate edilen kayıt sayısı
         * @return ErrorCode İşlem sonucu
         */
        TRAVELEXPENSE_API ErrorCode migrateBudgetsToDatabase(const std::vector<Budget>& budgets, int32_t& migratedCount);

        /**
         * @brief ID dosyalarını migrate et
         * 
         * ID dosyalarından (user.id, trip.id, vb.) maksimum ID değerlerini alır
         * ve veritabanındaki AUTOINCREMENT'i ayarlar.
         * 
         * @return ErrorCode İşlem sonucu
         */
        TRAVELEXPENSE_API ErrorCode migrateIdFiles();

        /**
         * @brief Tüm binary dosyaları SQLite veritabanına migrate et
         * 
         * @param dataDir Binary dosyaların bulunduğu dizin (varsayılan: "data")
         * @param result Migration sonuçları
         * @return ErrorCode İşlem sonucu
         */
        TRAVELEXPENSE_API ErrorCode migrateAllFromBinary(const char* dataDir, MigrationResult& result);

        /**
         * @brief Migration durumunu kontrol et
         * 
         * @param dataDir Binary dosyaların bulunduğu dizin
         * @return true Migration gerekli (binary dosyalar var), false Gerekli değil
         */
        TRAVELEXPENSE_API bool isMigrationNeeded(const char* dataDir);

    } // namespace Migration

} // namespace TravelExpense

#endif // MIGRATION_H

