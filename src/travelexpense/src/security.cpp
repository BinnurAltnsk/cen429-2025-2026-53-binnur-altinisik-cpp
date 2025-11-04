/**
 * @file security.cpp
 * @brief Seyahat Gideri Takibi - Güvenlik Fonksiyonları Implementation
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/security.h"
#include <cstring>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
    #include <winbase.h>
    // SecureZeroMemory zaten Windows'ta tanımlı
#else
    #include <string.h>
    // Linux için secure_memset benzeri fonksiyon
    static void* secure_memset(void* s, int c, size_t n) {
        volatile unsigned char* p = static_cast<volatile unsigned char*>(s);
        while (n--) {
            *p++ = static_cast<unsigned char>(c);
        }
        return s;
    }
    #define SecureZeroMemory(ptr, size) secure_memset(ptr, 0, size)
#endif

namespace TravelExpense {

    namespace Security {

        void secureMemset(void* ptr, int value, size_t size) {
            if (!ptr || size == 0) {
                return;
            }

            volatile unsigned char* p = static_cast<volatile unsigned char*>(ptr);
            volatile unsigned char v = static_cast<volatile unsigned char>(value);
            
            // Volatile pointer kullanarak optimizer'ın atlamasını engelle
            size_t i = 0;
            while (i < size) {
                p[i] = v;
                ++i;
            }
        }

        bool secureMemoryCleanup(void* ptr, size_t size) {
            if (!ptr || size == 0) {
                return false;
            }

            // Çoklu geçişli silme (3 geçiş)
            // Geçiş 1: Sıfırlama
            #ifdef _WIN32
                SecureZeroMemory(ptr, size);
            #else
                secure_memset(ptr, 0, size);
            #endif
            
            // Geçiş 2: Rastgele veri ile doldurma
            srand(static_cast<unsigned int>(time(nullptr)));
            unsigned char* p = static_cast<unsigned char*>(ptr);
            for (size_t i = 0; i < size; ++i) {
                p[i] = static_cast<unsigned char>(rand() % 256);
            }
            
            // Geçiş 3: Tekrar sıfırlama
            #ifdef _WIN32
                SecureZeroMemory(ptr, size);
            #else
                secure_memset(ptr, 0, size);
            #endif

            // Bellek alanını volatile pointer ile tekrar kontrol et
            volatile unsigned char* vp = static_cast<volatile unsigned char*>(ptr);
            for (size_t i = 0; i < size; ++i) {
                vp[i] = 0;
            }

            return true;
        }

        void secureCleanup(char* buffer, size_t size) {
            if (!buffer || size == 0) {
                return;
            }

            secureMemoryCleanup(buffer, size);
        }

    } // namespace Security

} // namespace TravelExpense

