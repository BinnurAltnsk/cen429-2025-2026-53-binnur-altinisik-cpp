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
#include "export.h"
#include <string>
#include <vector>

namespace TravelExpense { // LCOV_EXCL_LINE

    /**
     * @namespace ReportGenerator
     * @brief Rapor oluşturma modülü
     * 
     * Bu modül, seyahat giderleri için özet raporlar oluşturur,
     * raporları listeler ve dosyaya kaydeder. Kullanıcı ve seyahat
     * bazlı raporlama işlemlerini yönetir.
     */
    namespace ReportGenerator {
        /**
         * @brief Seyahat için özet rapor oluştur
         * 
         * Belirtilen seyahat ID'sine ait giderler ve bütçe bilgilerini kullanarak
         * özet rapor oluşturur. Rapor, toplam harcama, kategori bazlı harcamalar,
         * bütçe durumu ve gider listesi gibi bilgileri içerir.
         * 
         * @note Bu fonksiyon, raporu string formatında oluşturur.
         * Rapor formatı: JSON, XML veya plain text olabilir.
         * 
         * @param tripId Seyahat ID (geçerli bir tripId olmalı, 0'dan büyük)
         * @param report Oluşturulan rapor (çıktı parametresi, std::string)
         *                Rapor içeriği: seyahat bilgileri, gider özeti, bütçe durumu vb.
         * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
         */
        TRAVELEXPENSE_API ErrorCode generateReport(int32_t tripId, std::string& report);

        /**
         * @brief Kullanıcıya ait raporları listele
         * 
         * Belirtilen kullanıcı ID'sine ait tüm seyahat raporlarını listeler.
         * Raporlar, seyahat ID'sine göre sıralanabilir.
         * 
         * @note Bu fonksiyon, reports vektörünü temizler ve yeni raporlarla doldurur.
         * Eğer kullanıcı için rapor bulunamazsa, reports vektörü boş kalır.
         * 
         * @param userId Kullanıcı ID (geçerli bir userId olmalı, 0'dan büyük)
         * @param reports Rapor listesi (çıktı parametresi, std::vector<std::string>)
         *                 Her öğe bir seyahat raporunu temsil eder.
         *                 Fonksiyon çağrıldığında temizlenir ve yeni raporlarla doldurulur.
         * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
         */
        TRAVELEXPENSE_API ErrorCode getReports(int32_t userId, std::vector<std::string>& reports);

        /**
         * @brief Raporu dosyaya kaydet
         * 
         * Belirtilen seyahat için oluşturulan raporu dosyaya kaydeder.
         * Dosya yoksa oluşturulur, varsa üzerine yazılır.
         * 
         * @note Bu fonksiyon, generateReport() ile oluşturulan raporu dosyaya yazar.
         * Dosya formatı: .txt, .json veya .xml olabilir.
         * 
         * @param tripId Seyahat ID (geçerli bir tripId olmalı, 0'dan büyük)
         * @param filepath Dosya yolu (nullptr ise InvalidInput döner)
         *                 Dosya yolu geçerli ve yazılabilir olmalı.
         * @return ErrorCode Başarı durumu (Success, FileIO, InvalidInput vb.)
         */
        TRAVELEXPENSE_API ErrorCode saveReport(int32_t tripId, const char* filepath);
    }

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // REPORT_GENERATOR_H

