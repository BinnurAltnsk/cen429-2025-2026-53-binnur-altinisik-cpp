/**
 * @file rasp.h
 * @brief Seyahat Gideri Takibi - RASP (Runtime Application Self-Protection) Fonksiyonları
 * 
 * Bu dosya, RASP tekniklerinin bildirimlerini içerir:
 * - Checksum doğrulama (Kod bloğu ve dosya integrity check)
 * - Anti-debug mekanizmaları (Debugger tespiti ve önleme)
 * - Tamper tespiti (Dosya değişiklik algılama ve müdahale tespiti)
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef RASP_H
#define RASP_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "export.h"

namespace TravelExpense {

    /**
     * @namespace RASP
     * @brief RASP (Runtime Application Self-Protection) teknikleri modülü
     */
    namespace RASP {

        // ============================================================================
        // CHECKSUM DOĞRULAMA - Checksum Verification
        // ============================================================================

        /**
         * @brief Kod bloğu checksum hesapla
         * 
         * Belirtilen bellek bölgesinin SHA-256 hash'ini hesaplar
         * 
         * @param data Kod bloğu başlangıç adresi
         * @param size Kod bloğu boyutu (byte)
         * @param checksum Çıktı checksum buffer'ı (en az 65 byte olmalı)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool calculateCodeChecksum(const void* data, size_t size, char* checksum);

        /**
         * @brief Kod bloğu checksum doğrula
         * 
         * Mevcut checksum ile beklenen checksum'u karşılaştırır
         * 
         * @param data Kod bloğu başlangıç adresi
         * @param size Kod bloğu boyutu (byte)
         * @param expectedChecksum Beklenen checksum (SHA-256 hex string)
         * @return true Checksum geçerli, false Checksum uyuşmuyor veya hata
         */
        TRAVELEXPENSE_API bool verifyCodeChecksum(const void* data, size_t size, const char* expectedChecksum);

        /**
         * @brief Dosya checksum hesapla
         * 
         * Dosyanın SHA-256 hash'ini hesaplar
         * 
         * @param filePath Dosya yolu
         * @param checksum Çıktı checksum buffer'ı (en az 65 byte olmalı)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool calculateFileChecksum(const char* filePath, char* checksum);

        /**
         * @brief Dosya checksum doğrula
         * 
         * Mevcut dosya checksum'u ile beklenen checksum'u karşılaştırır
         * 
         * @param filePath Dosya yolu
         * @param expectedChecksum Beklenen checksum (SHA-256 hex string)
         * @return true Checksum geçerli, false Checksum uyuşmuyor veya hata
         */
        TRAVELEXPENSE_API bool verifyFileChecksum(const char* filePath, const char* expectedChecksum);

        /**
         * @brief Kendi binary'sinin checksum'unu hesapla
         * 
         * Çalışan uygulamanın binary dosyasının checksum'unu hesaplar
         * 
         * @param checksum Çıktı checksum buffer'ı (en az 65 byte olmalı)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool calculateSelfChecksum(char* checksum);

        /**
         * @brief Kendi binary'sinin checksum'unu doğrula
         * 
         * Çalışan uygulamanın binary dosyasının checksum'unu kontrol eder
         * 
         * @param expectedChecksum Beklenen checksum (SHA-256 hex string)
         * @return true Checksum geçerli, false Checksum uyuşmuyor veya hata
         */
        TRAVELEXPENSE_API bool verifySelfChecksum(const char* expectedChecksum);

        /**
         * @brief Runtime checksum doğrulama callback fonksiyon tipi
         * 
         * @return true Checksum geçerli, false Checksum uyuşmuyor
         */
        typedef bool (*ChecksumCallback)();

        /**
         * @brief Periyodik checksum kontrolünü başlat
         * 
         * Belirli aralıklarla checksum kontrolü yapar
         * 
         * @param callback Checksum doğrulama callback fonksiyonu
         * @param intervalMs Kontrol aralığı (milisaniye)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool startPeriodicChecksum(ChecksumCallback callback, uint32_t intervalMs);

        /**
         * @brief Periyodik checksum kontrolünü durdur
         * 
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool stopPeriodicChecksum();

        // ============================================================================
        // ANTI-DEBUG - Debugger Tespiti ve Önleme
        // ============================================================================

        /**
         * @brief Debugger tespit edildi mi?
         * 
         * Windows: IsDebuggerPresent() ve CheckRemoteDebuggerPresent()
         * Linux: ptrace() kontrolü
         * 
         * @return true Debugger tespit edildi, false Debugger yok
         */
        TRAVELEXPENSE_API bool isDebuggerPresent();

        /**
         * @brief Remote debugger tespit edildi mi? (Sadece Windows)
         * 
         * @return true Remote debugger tespit edildi, false Remote debugger yok veya hata
         */
        TRAVELEXPENSE_API bool isRemoteDebuggerPresent();

        /**
         * @brief ptrace anti-debug kontrolü (Sadece Linux)
         * 
         * @return true ptrace başarılı (debugger yok), false ptrace başarısız (debugger var)
         */
        TRAVELEXPENSE_API bool checkPtrace();

        /**
         * @brief Debugger process listesini kontrol et
         * 
         * Sistemde bilinen debugger process'lerini tespit eder
         * 
         * @return true Debugger process tespit edildi, false Debugger process yok
         */
        TRAVELEXPENSE_API bool detectDebuggerProcesses();

        /**
         * @brief Tüm anti-debug kontrollerini çalıştır
         * 
         * Tüm anti-debug mekanizmalarını sırayla çalıştırır
         * 
         * @return true Debugger tespit edildi, false Debugger yok
         */
        TRAVELEXPENSE_API bool performAntiDebugCheck();

        /**
         * @brief Anti-debug tespiti durumunda tepki ver
         * 
         * Debugger tespit edildiğinde uygulamayı kapatır veya başka bir tepki verir
         * 
         * @param terminate Uygulamayı sonlandır mı? (true: sonlandır, false: sadece log)
         */
        TRAVELEXPENSE_API void handleDebuggerDetected(bool terminate);

        // ============================================================================
        // TAMPER DETECTION - Müdahale Tespiti
        // ============================================================================

        /**
         * @brief Dosya değişikliği tespit et
         * 
         * Dosyanın son değiştirilme zamanını kontrol eder
         * 
         * @param filePath Dosya yolu
         * @param expectedModTime Beklenen son değiştirilme zamanı (Unix timestamp)
         * @return true Dosya değiştirilmiş, false Dosya değiştirilmemiş veya hata
         */
        TRAVELEXPENSE_API bool detectFileModification(const char* filePath, int64_t expectedModTime);

        /**
         * @brief Dosya boyutu değişikliği tespit et
         * 
         * Dosyanın boyutunu kontrol eder
         * 
         * @param filePath Dosya yolu
         * @param expectedSize Beklenen dosya boyutu (byte)
         * @return true Dosya boyutu değişmiş, false Dosya boyutu aynı veya hata
         */
        TRAVELEXPENSE_API bool detectFileSizeChange(const char* filePath, int64_t expectedSize);

        /**
         * @brief Binary integrity kontrolü
         * 
         * Binary dosyasının checksum'unu kontrol eder
         * 
         * @param expectedChecksum Beklenen checksum (SHA-256 hex string)
         * @return true Binary değiştirilmemiş, false Binary değiştirilmiş veya hata
         */
        TRAVELEXPENSE_API bool checkBinaryIntegrity(const char* expectedChecksum);

        /**
         * @brief Tüm tamper detection kontrollerini çalıştır
         * 
         * Tüm tamper detection mekanizmalarını sırayla çalıştırır
         * 
         * @param expectedSelfChecksum Beklenen self checksum (SHA-256 hex string)
         * @return true Müdahale tespit edildi, false Müdahale yok
         */
        TRAVELEXPENSE_API bool performTamperCheck(const char* expectedSelfChecksum);

        /**
         * @brief Tamper tespiti durumunda tepki ver
         * 
         * Müdahale tespit edildiğinde uygulamayı kapatır veya başka bir tepki verir
         * 
         * @param terminate Uygulamayı sonlandır mı? (true: sonlandır, false: sadece log)
         */
        TRAVELEXPENSE_API void handleTamperDetected(bool terminate);

        // ============================================================================
        // RASP INITIALIZATION - RASP Başlatma ve Yönetimi
        // ============================================================================

        /**
         * @brief RASP sistemini başlat
         * 
         * Tüm RASP mekanizmalarını başlatır
         * 
         * @param expectedSelfChecksum Beklenen self checksum (SHA-256 hex string)
         * @param enablePeriodicCheck Periyodik checksum kontrolü aktif mi?
         * @param checkIntervalMs Periyodik kontrol aralığı (milisaniye)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool initializeRASP(const char* expectedSelfChecksum, bool enablePeriodicCheck, uint32_t checkIntervalMs);

        /**
         * @brief RASP sistemini kapat
         * 
         * Tüm RASP mekanizmalarını durdurur
         * 
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool shutdownRASP();

        /**
         * @brief RASP durumunu kontrol et
         * 
         * RASP sisteminin çalışıp çalışmadığını kontrol eder
         * 
         * @return true RASP aktif, false RASP pasif veya hata
         */
        TRAVELEXPENSE_API bool isRASPActive();

        /**
         * @brief RASP durumunu al
         * 
         * RASP sisteminin detaylı durumunu döndürür
         * 
         * @param debuggerDetected Debugger tespit edildi mi? (çıktı parametresi)
         * @param tamperDetected Müdahale tespit edildi mi? (çıktı parametresi)
         * @param checksumValid Checksum geçerli mi? (çıktı parametresi)
         * @return true Başarılı, false Hata
         */
        TRAVELEXPENSE_API bool getRASPStatus(bool* debuggerDetected, bool* tamperDetected, bool* checksumValid);

    } // namespace RASP

} // namespace TravelExpense

#endif // RASP_H

