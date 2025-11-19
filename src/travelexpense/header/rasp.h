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

namespace TravelExpense { // LCOV_EXCL_LINE

/**
 * @namespace RASP
 * @brief RASP (Runtime Application Self-Protection) teknikleri modülü
 *
 * Bu modül, runtime uygulama kendi kendini koruma tekniklerini sağlar:
 * checksum doğrulama, anti-debug mekanizmaları, tamper detection ve
 * periyodik kontrol sistemi. Uygulamanın çalışma zamanında güvenliğini sağlar.
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
TRAVELEXPENSE_API bool calculateCodeChecksum(const void *data, size_t size, char *checksum);

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
TRAVELEXPENSE_API bool verifyCodeChecksum(const void *data, size_t size, const char *expectedChecksum);

/**
 * @brief Dosya checksum hesapla
 *
 * Dosyanın SHA-256 hash'ini hesaplar
 *
 * @param filePath Dosya yolu
 * @param checksum Çıktı checksum buffer'ı (en az 65 byte olmalı)
 * @return true Başarılı, false Hata
 */
TRAVELEXPENSE_API bool calculateFileChecksum(const char *filePath, char *checksum);

/**
 * @brief Dosya checksum doğrula
 *
 * Mevcut dosya checksum'u ile beklenen checksum'u karşılaştırır
 *
 * @param filePath Dosya yolu
 * @param expectedChecksum Beklenen checksum (SHA-256 hex string)
 * @return true Checksum geçerli, false Checksum uyuşmuyor veya hata
 */
TRAVELEXPENSE_API bool verifyFileChecksum(const char *filePath, const char *expectedChecksum);

/**
 * @brief Kendi binary'sinin checksum'unu hesapla
 *
 * Çalışan uygulamanın binary dosyasının checksum'unu hesaplar
 *
 * @param checksum Çıktı checksum buffer'ı (en az 65 byte olmalı)
 * @return true Başarılı, false Hata
 */
TRAVELEXPENSE_API bool calculateSelfChecksum(char *checksum);

/**
 * @brief Kendi binary'sinin checksum'unu doğrula
 *
 * Çalışan uygulamanın binary dosyasının checksum'unu kontrol eder
 *
 * @param expectedChecksum Beklenen checksum (SHA-256 hex string)
 * @return true Checksum geçerli, false Checksum uyuşmuyor veya hata
 */
TRAVELEXPENSE_API bool verifySelfChecksum(const char *expectedChecksum);

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
TRAVELEXPENSE_API bool detectFileModification(const char *filePath, int64_t expectedModTime);

/**
 * @brief Dosya boyutu değişikliği tespit et
 *
 * Dosyanın boyutunu kontrol eder
 *
 * @param filePath Dosya yolu
 * @param expectedSize Beklenen dosya boyutu (byte)
 * @return true Dosya boyutu değişmiş, false Dosya boyutu aynı veya hata
 */
TRAVELEXPENSE_API bool detectFileSizeChange(const char *filePath, int64_t expectedSize);

/**
 * @brief Binary integrity kontrolü
 *
 * Binary dosyasının checksum'unu kontrol eder
 *
 * @param expectedChecksum Beklenen checksum (SHA-256 hex string)
 * @return true Binary değiştirilmemiş, false Binary değiştirilmiş veya hata
 */
TRAVELEXPENSE_API bool checkBinaryIntegrity(const char *expectedChecksum);

/**
 * @brief Tüm tamper detection kontrollerini çalıştır
 *
 * Tüm tamper detection mekanizmalarını sırayla çalıştırır
 *
 * @param expectedSelfChecksum Beklenen self checksum (SHA-256 hex string)
 * @return true Müdahale tespit edildi, false Müdahale yok
 */
TRAVELEXPENSE_API bool performTamperCheck(const char *expectedSelfChecksum);

/**
 * @brief Tamper tespiti durumunda tepki ver
 *
 * Müdahale tespit edildiğinde uygulamayı kapatır veya başka bir tepki verir
 *
 * @param terminate Uygulamayı sonlandır mı? (true: sonlandır, false: sadece log)
 */
TRAVELEXPENSE_API void handleTamperDetected(bool terminate);

// ============================================================================
// HOOK DETECTION - Hook Saldırı Tespiti
// ============================================================================

/**
 * @brief Hook saldırısı tespit et
 *
 * Sistem fonksiyonlarının hook'lanıp hook'lanmadığını kontrol eder.
 * Windows ve Linux'ta farklı teknikler kullanır.
 *
 * @return true Hook tespit edildi, false Hook yok
 */
TRAVELEXPENSE_API bool detectHookAttack();

/**
 * @brief Belirli bir fonksiyonun hook'lanıp hook'lanmadığını kontrol et
 *
 * @param functionAddress Fonksiyon adresi
 * @param functionName Fonksiyon adı (debug için)
 * @return true Hook tespit edildi, false Hook yok
 */
// rasp.h
TRAVELEXPENSE_API bool detectFunctionHook(const void *functionAddress, const char *functionName);


/**
 * @brief Kritik fonksiyonların hook kontrolünü yap
 *
 * Uygulamanın kritik fonksiyonlarının hook'lanıp hook'lanmadığını kontrol eder
 *
 * @return true Hook tespit edildi, false Hook yok
 */
TRAVELEXPENSE_API bool checkCriticalFunctionHooks();

/**
 * @brief Hook tespiti durumunda tepki ver
 *
 * Hook tespit edildiğinde uygulamayı kapatır veya başka bir tepki verir
 *
 * @param terminate Uygulamayı sonlandır mı? (true: sonlandır, false: sadece log)
 */
TRAVELEXPENSE_API void handleHookDetected(bool terminate);

// ============================================================================
// CONTROL FLOW COUNTER - Kontrol Akışı Sayacı Kontrolü
// ============================================================================

/**
 * @brief Kontrol akışı sayacını başlat
 *
 * Kontrol akışı bütünlüğünü sağlamak için sayaç başlatır
 *
 * @param expectedValue Beklenen sayaç değeri
 * @return true Başarılı, false Hata
 */
TRAVELEXPENSE_API bool initializeControlFlowCounter(uint32_t expectedValue);

/**
 * @brief Kontrol akışı sayacını artır
 *
 * Her kritik kontrol akışı noktasında çağrılmalıdır
 *
 * @return true Başarılı, false Hata
 */
TRAVELEXPENSE_API bool incrementControlFlowCounter();

/**
 * @brief Kontrol akışı sayacını doğrula
 *
 * Mevcut sayaç değerinin beklenen değerle uyumlu olup olmadığını kontrol eder
 *
 * @param expectedValue Beklenen sayaç değeri
 * @return true Sayaç geçerli, false Sayaç geçersiz (müdahale tespit edildi)
 */
TRAVELEXPENSE_API bool verifyControlFlowCounter(uint32_t expectedValue);

/**
 * @brief Kontrol akışı sayacını al
 *
 * @return Mevcut sayaç değeri
 */
TRAVELEXPENSE_API uint32_t getControlFlowCounter();

/**
 * @brief Kontrol akışı sayacını sıfırla
 *
 * @return true Başarılı, false Hata
 */
TRAVELEXPENSE_API bool resetControlFlowCounter();

/**
 * @brief Kontrol akışı bütünlük kontrolü yap
 *
 * Tüm kontrol akışı mekanizmalarını kontrol eder
 *
 * @param expectedValue Beklenen sayaç değeri
 * @return true Kontrol akışı geçerli, false Müdahale tespit edildi
 */
TRAVELEXPENSE_API bool performControlFlowCheck(uint32_t expectedValue);

// ============================================================================
// UNSAFE DEVICE DETECTION - Güvensiz Cihaz Tespiti
// ============================================================================

/**
 * @brief Emulator tespit et
 *
 * Sistem özelliklerini kontrol ederek emulator kullanımını tespit eder
 *
 * @return true Emulator tespit edildi, false Emulator yok
 */
TRAVELEXPENSE_API bool detectEmulator();

/**
 * @brief Güvensiz cihaz tespit et
 *
 * Emulator, root/jailbreak ve diğer güvensiz cihaz durumlarını kontrol eder
 *
 * @return true Güvensiz cihaz tespit edildi, false Cihaz güvenli
 */
TRAVELEXPENSE_API bool detectUnsafeDevice();

/**
 * @brief Root/Jailbreak tespit et
 *
 * Windows: Administrator privileges kontrolü
 * Linux: Root access kontrolü
 *
 * @return true Root/Jailbreak tespit edildi, false Normal kullanıcı
 */
TRAVELEXPENSE_API bool detectRootJailbreak();

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
TRAVELEXPENSE_API bool initializeRASP(const char *expectedSelfChecksum, bool enablePeriodicCheck, uint32_t checkIntervalMs);

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
TRAVELEXPENSE_API bool getRASPStatus(bool *debuggerDetected, bool *tamperDetected, bool *checksumValid);

} // namespace RASP

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // RASP_H
