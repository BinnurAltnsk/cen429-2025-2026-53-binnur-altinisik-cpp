/**
 * @file tls.h
 * @brief Seyahat Gideri Takibi - SSL/TLS ve Certificate Pinning
 *
 * Bu dosya, SSL/TLS ve Certificate Pinning gereksinimlerini karşılar:
 * - SSL/TLS bağlantısı kurma
 * - Certificate pinning implementasyonu
 * - Mutual authentication
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef TLS_H
#define TLS_H

#include "commonTypes.h"
#include "export.h"
#include <cstdint>
#include <cstddef>

namespace TravelExpense { // LCOV_EXCL_LINE

/**
 * @namespace TLS
 * @brief SSL/TLS ve Certificate Pinning modülü
 */
namespace TLS {

// ============================================
// SSL/TLS BAĞLANTI YÖNETİMİ
// ============================================

/**
 * @brief TLS bağlamı (context) yapısı
 *
 * Bu yapı, TLS bağlantı bilgilerini saklar.
 * Gerçek implementasyonda OpenSSL SSL_CTX veya benzeri kullanılabilir.
 *
 * @note Bu yapı, TLS bağlantısının yaşam döngüsü boyunca geçerli kalır.
 * Bağlantı kapatıldığında cleanupTLSContext() ile temizlenmelidir.
 */
struct TLSContext {
  /** @brief OpenSSL SSL_CTX* veya benzeri bağlam işaretçisi */
  void *sslContext;
  /** @brief OpenSSL SSL* veya benzeri bağlantı işaretçisi */
  void *sslConnection;
  /** @brief Bağlam başlatıldı mı? (true = başlatıldı, false = başlatılmadı) */
  bool isInitialized;
  /** @brief Bağlantı kuruldu mu? (true = bağlı, false = bağlı değil) */
  bool isConnected;
  /** @brief Sunucu hostname (max 255 karakter) */
  char serverHostname[256];
  /** @brief Sunucu port numarası (1-65535) */
  uint16_t serverPort;
};

/**
 * @brief TLS bağlamını başlat
 *
 * @param ctx TLS bağlamı (çıktı)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode initializeTLSContext(TLSContext *ctx);

/**
 * @brief TLS bağlamını temizle
 *
 * @param ctx TLS bağlamı
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode cleanupTLSContext(TLSContext *ctx);

/**
 * @brief TLS bağlantısı kur
 *
 * @param ctx TLS bağlamı
 * @param hostname Sunucu hostname
 * @param port Sunucu port
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode connectTLS(TLSContext *ctx, const char *hostname, uint16_t port);

/**
 * @brief TLS bağlantısını kapat
 *
 * @param ctx TLS bağlamı
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode disconnectTLS(TLSContext *ctx);

/**
 * @brief TLS üzerinden veri gönder
 *
 * @param ctx TLS bağlamı
 * @param data Gönderilecek veri
 * @param dataLen Veri uzunluğu
 * @param bytesSent Gönderilen byte sayısı (çıktı)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode sendTLS(TLSContext *ctx, const void *data, size_t dataLen, size_t &bytesSent);

/**
 * @brief TLS üzerinden veri al
 *
 * @param ctx TLS bağlamı
 * @param buffer Alınacak veri buffer'ı
 * @param bufferLen Buffer uzunluğu
 * @param bytesReceived Alınan byte sayısı (çıktı)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode receiveTLS(TLSContext *ctx, void *buffer, size_t bufferLen, size_t &bytesReceived);

// ============================================
// CERTIFICATE PINNING
// ============================================

/**
 * @brief Sertifika pin bilgisi
 *
 * Bu yapı, pin'lenecek sertifika bilgilerini saklar.
 * Certificate pinning, sunucu sertifikasının beklenen değerlerle eşleşmesini zorunlu kılar.
 *
 * @note Hem fingerprint hem de public key hash pin'lenebilir.
 * En güvenli yaklaşım, her ikisini de pin'lemektir.
 */
struct CertificatePin {
  /** @brief Hostname (max 255 karakter) */
  char hostname[256];
  /** @brief SHA-256 fingerprint (64 karakter hex string + null terminator) */
  char fingerprint[65];
  /** @brief Public key hash (64 karakter hex string + null terminator) */
  char publicKeyHash[65];
  /** @brief Sertifika pin'i etkin mi? (true = pin aktif, false = pin pasif) */
  bool pinCertificate;
  /** @brief Public key pin'i etkin mi? (true = pin aktif, false = pin pasif) */
  bool pinPublicKey;
};

/**
 * @brief Certificate pin kaydet
 *
 * @param pin Certificate pin bilgisi
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode registerCertificatePin(const CertificatePin *pin);

/**
 * @brief Certificate pin doğrula
 *
 * Mevcut sertifika ile kayıtlı pin'i karşılaştırır.
 *
 * @param ctx TLS bağlamı
 * @param hostname Sunucu hostname
 * @return ErrorCode Başarı durumu (SUCCESS = geçerli, ERROR = geçersiz)
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode verifyCertificatePin(TLSContext *ctx, const char *hostname);

/**
 * @brief Sunucu sertifikasının fingerprint'ini al
 *
 * @param ctx TLS bağlamı
 * @param fingerprint Sertifika fingerprint çıktısı (64 karakter hex)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode getCertificateFingerprint(TLSContext *ctx, char *fingerprint);

/**
 * @brief Sunucu sertifikasının public key hash'ini al
 *
 * @param ctx TLS bağlamı
 * @param publicKeyHash Public key hash çıktısı (64 karakter hex)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode getCertificatePublicKeyHash(TLSContext *ctx, char *publicKeyHash);

// ============================================
// MUTUAL AUTHENTICATION
// ============================================

/**
 * @brief İstemci sertifikası yükle
 *
 * Mutual authentication için istemci sertifikası ve private key'i yükler.
 *
 * @param ctx TLS bağlamı
 * @param certPath İstemci sertifika dosyası yolu (PEM format)
 * @param keyPath İstemci private key dosyası yolu (PEM format)
 * @param keyPassword Private key şifresi (nullptr ise şifre yok)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode loadClientCertificate(TLSContext *ctx, const char *certPath,
    const char *keyPath, const char *keyPassword);

/**
 * @brief CA sertifika yolu ayarla
 *
 * Sunucu sertifikasını doğrulamak için CA sertifika dizinini ayarlar.
 *
 * @param ctx TLS bağlamı
 * @param caPath CA sertifika dosyası yolu veya dizin yolu
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode setCAPath(TLSContext *ctx, const char *caPath);

/**
 * @brief Sunucu sertifikası doğrula
 *
 * Sunucu sertifikasının geçerliliğini kontrol eder.
 *
 * @param ctx TLS bağlamı
 * @return ErrorCode Başarı durumu (SUCCESS = geçerli, ERROR = geçersiz)
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode verifyServerCertificate(TLSContext *ctx);

// ============================================
// YARDIMCI FONKSİYONLAR
// ============================================

/**
 * @brief Sertifika fingerprint hesapla
 *
 * Sertifika dosyasından SHA-256 fingerprint hesaplar.
 *
 * @param certPath Sertifika dosyası yolu (PEM veya DER format)
 * @param fingerprint Fingerprint çıktısı (64 karakter hex)
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode calculateCertificateFingerprint(const char *certPath, char *fingerprint);

/**
 * @brief TLS hata mesajı al
 *
 * @param errorCode Hata kodu
 * @param message Hata mesajı buffer'ı
 * @param messageLen Buffer uzunluğu
 * @return ErrorCode Başarı durumu
 */
TRAVELEXPENSE_API TravelExpense::ErrorCode getTLSErrorMessage(ErrorCode errorCode, char *message, size_t messageLen);

} // namespace TLS

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // TLS_H
