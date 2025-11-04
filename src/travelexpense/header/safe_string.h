/**
 * @file safe_string.h
 * @brief Güvenli string işlemleri için yardımcı fonksiyonlar
 * 
 * Bu dosya, MSVC C4996 uyarılarını önlemek için güvenli string işlemleri sağlar.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef SAFE_STRING_H
#define SAFE_STRING_H

#include <cstring>
#include <cstddef>

namespace TravelExpense {
    namespace SafeString {

        /**
         * @brief Güvenli string kopyalama
         * 
         * MSVC'de strncpy_s kullanır, diğer platformlarda strncpy + null terminator.
         * 
         * @param dest Hedef buffer
         * @param destSize Hedef buffer boyutu
         * @param src Kaynak string
         * @return true Başarılı, false Hata
         */
        inline bool safeCopy(char* dest, size_t destSize, const char* src) {
            if (!dest || destSize == 0) {
                return false;
            }

#ifdef _MSC_VER
            if (strncpy_s(dest, destSize, src ? src : "", _TRUNCATE) != 0) {
                return false;
            }
#else
            if (!src) {
                dest[0] = '\0';
                return true;
            }
            std::strncpy(dest, src, destSize - 1);
            dest[destSize - 1] = '\0';
#endif
            return true;
        }

        /**
         * @brief Güvenli string birleştirme
         * 
         * MSVC'de strncat_s kullanır, diğer platformlarda strncat.
         * 
         * @param dest Hedef buffer
         * @param destSize Hedef buffer boyutu
         * @param src Eklenecek string
         * @return true Başarılı, false Hata
         */
        inline bool safeConcat(char* dest, size_t destSize, const char* src) {
            if (!dest || destSize == 0) {
                return false;
            }

#ifdef _MSC_VER
            if (strncat_s(dest, destSize, src ? src : "", _TRUNCATE) != 0) {
                return false;
            }
#else
            if (!src) {
                return true;
            }
            size_t destLen = std::strlen(dest);
            if (destLen >= destSize - 1) {
                return false;
            }
            std::strncat(dest, src, destSize - destLen - 1);
            dest[destSize - 1] = '\0';
#endif
            return true;
        }

    } // namespace SafeString
} // namespace TravelExpense

#endif // SAFE_STRING_H

