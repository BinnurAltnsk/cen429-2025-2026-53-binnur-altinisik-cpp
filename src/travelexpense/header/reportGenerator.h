/**
 * @file reportGenerator.h
 * @brief Seyahat Gideri Takibi - Rapor Oluşturma Modülü
 * 
 * Bu dosya, rapor oluşturma işlemlerinin fonksiyon bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include "commonTypes.h"
#include <string>
#include <vector>

namespace TravelExpense {

    /**
     * @namespace ReportGenerator
     * @brief Rapor oluşturma modülü
     */
    namespace ReportGenerator {
        /**
         * @brief Seyahat için özet rapor oluştur
         * @param tripId Seyahat ID
         * @param report Oluşturulan rapor (string)
         * @return ErrorCode Başarı durumu
         */
        ErrorCode generateReport(int32_t tripId, std::string& report);

        /**
         * @brief Kullanıcıya ait raporları listele
         * @param userId Kullanıcı ID
         * @param reports Rapor listesi
         * @return ErrorCode Başarı durumu
         */
        ErrorCode getReports(int32_t userId, std::vector<std::string>& reports);

        /**
         * @brief Raporu dosyaya kaydet
         * @param tripId Seyahat ID
         * @param filepath Dosya yolu
         * @return ErrorCode Başarı durumu
         */
        ErrorCode saveReport(int32_t tripId, const char* filepath);
    }

} // namespace TravelExpense

#endif // REPORT_GENERATOR_H

