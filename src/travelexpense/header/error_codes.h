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
namespace TravelExpense {

    /**
     * @brief Hata kodları enum'u
     */
    enum class ErrorCode {
        Success = 0,
        FileNotFound = 1,
        DecryptionFailed = 2,
        ChecksumMismatch = 3,
        InvalidUser = 4,
        BudgetExceeded = 5,
        InvalidInput = 6,
        MemoryAllocation = 7,
        FileIO = 8,
        EncryptionFailed = 9,
        ConnectionFailed = 10,
        SecurityFailed = 11,

        Unknown = 99
    };

} // namespace TravelExpense

#endif // ERROR_CODES_H

