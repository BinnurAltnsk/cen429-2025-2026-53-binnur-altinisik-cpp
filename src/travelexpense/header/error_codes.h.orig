/**
 * @file error_codes.h
 * @brief Seyahat Gideri Takibi - Hata Kodları Enum'u
 *
 * Bu dosya, seyahat gideri takibi uygulamasının merkezi hata kodları enum'unu içerir.
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef ERROR_CODES_H
#define ERROR_CODES_H

/**
 * @namespace TravelExpense
 * @brief Ana namespace - Seyahat Gideri Takibi uygulaması
 */
namespace TravelExpense { // LCOV_EXCL_LINE

/**
 * @brief Hata kodları enum'u
 *
 * Uygulama genelinde kullanılan merkezi hata kodları enum'u.
 * Tüm modüller bu enum'u kullanarak hata durumlarını bildirir.
 */
enum class ErrorCode {
  Success = 0,              /**< @brief İşlem başarıyla tamamlandı */
  FileNotFound = 1,        /**< @brief Dosya bulunamadı */
  DecryptionFailed = 2,     /**< @brief Şifre çözme işlemi başarısız */
  ChecksumMismatch = 3,     /**< @brief Checksum uyuşmazlığı */
  InvalidUser = 4,         /**< @brief Geçersiz kullanıcı */
  BudgetExceeded = 5,      /**< @brief Bütçe aşıldı */
  InvalidInput = 6,        /**< @brief Geçersiz girdi */
  MemoryAllocation = 7,    /**< @brief Bellek tahsis hatası */
  FileIO = 8,              /**< @brief Dosya giriş/çıkış hatası */
  EncryptionFailed = 9,     /**< @brief Şifreleme işlemi başarısız */
  ConnectionFailed = 10,   /**< @brief Bağlantı hatası */
  SecurityFailed = 11,     /**< @brief Güvenlik kontrolü başarısız */

  Unknown = 99             /**< @brief Bilinmeyen hata */
};

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // ERROR_CODES_H
