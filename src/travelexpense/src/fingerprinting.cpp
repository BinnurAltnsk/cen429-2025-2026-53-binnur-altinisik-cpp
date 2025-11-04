/**
 * @file fingerprinting.cpp
 * @brief Seyahat Gideri Takibi - Cihaz ve Uygulama Fingerprinting Implementation
 * 
 * Bu dosya, Dinamik Varlıkların Korunması gereksinimlerinin implementasyonlarını içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/fingerprinting.h"
#include "../header/encryption.h"
#include "../header/rasp.h"
#include "../header/sessionManager.h"
#include "../header/safe_string.h"
#include <cstring>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "iphlpapi.lib")
    #include <sysinfoapi.h>
#else
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <net/if.h>
    #include <ifaddrs.h>
    #include <sys/utsname.h>
#endif

namespace TravelExpense {

    namespace Fingerprinting {

        // ============================================
        // CİHAZ FİNGERPRİNTİNG
        // ============================================

        ErrorCode generateDeviceFingerprint(char* fingerprint) {
            if (!fingerprint) {
                return ErrorCode::InvalidInput;
            }

            std::ostringstream oss;
            
#ifdef _WIN32
            // Windows: MAC adresi kullan
            IP_ADAPTER_INFO adapterInfo[16];
            DWORD dwBufLen = sizeof(adapterInfo);
            DWORD dwStatus = GetAdaptersInfo(adapterInfo, &dwBufLen);
            
            if (dwStatus == ERROR_SUCCESS) {
                PIP_ADAPTER_INFO pAdapterInfo = adapterInfo;
                if (pAdapterInfo) {
                    // İlk MAC adresini kullan
                    for (int i = 0; i < pAdapterInfo->AddressLength; ++i) {
                        oss << std::hex << std::setw(2) << std::setfill('0')
                            << static_cast<int>(pAdapterInfo->Address[i]);
                    }
                }
            }

            // Sistem bilgileri ekle
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            oss << sysInfo.dwProcessorType;
            oss << sysInfo.dwNumberOfProcessors;

            // Bilgisayar adı ekle
            char computerName[MAX_COMPUTERNAME_LENGTH + 1];
            DWORD size = sizeof(computerName);
            if (GetComputerNameA(computerName, &size)) {
                oss << computerName;
            }
#else
            // Linux: MAC adresi kullan
            struct ifaddrs* ifaddr = nullptr;
            if (getifaddrs(&ifaddr) == 0) {
                for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
                    if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_PACKET) {
                        struct sockaddr_ll* s = reinterpret_cast<struct sockaddr_ll*>(ifa->ifa_addr);
                        for (int i = 0; i < 6; ++i) {
                            oss << std::hex << std::setw(2) << std::setfill('0')
                                << static_cast<int>(s->sll_addr[i]);
                        }
                        break; // İlk MAC adresini kullan
                    }
                }
                freeifaddrs(ifaddr);
            }

            // Sistem bilgileri ekle
            struct utsname unameInfo;
            if (uname(&unameInfo) == 0) {
                oss << unameInfo.machine;
                oss << unameInfo.nodename;
            }

            // Hostname ekle
            char hostname[256];
            if (gethostname(hostname, sizeof(hostname)) == 0) {
                oss << hostname;
            }
#endif

            std::string fp = oss.str();
            if (fp.empty()) {
                // Fallback: timestamp + process ID
                fp = std::to_string(time(nullptr));
#ifdef _WIN32
                fp += std::to_string(GetCurrentProcessId());
#else
                fp += std::to_string(getpid());
#endif
            }

            // SHA-256 hash ile fingerprint oluştur
            char hash[65];
            if (!Encryption::sha256Hash(fp.c_str(), fp.length(), hash)) {
                return ErrorCode::EncryptionFailed;
            }

            SafeString::safeCopy(fingerprint, 65, hash);

            return ErrorCode::Success;
        }

        ErrorCode verifyDeviceFingerprint(const char* expectedFingerprint) {
            if (!expectedFingerprint) {
                return ErrorCode::InvalidInput;
            }

            // Mevcut cihaz fingerprint'ini al
            char currentFingerprint[65];
            if (generateDeviceFingerprint(currentFingerprint) != ErrorCode::Success) {
                return ErrorCode::Unknown;
            }

            // Fingerprint'leri karşılaştır (case-insensitive)
#ifdef _WIN32
            if (_strnicmp(currentFingerprint, expectedFingerprint, 64) != 0) {
#else
            if (strncasecmp(currentFingerprint, expectedFingerprint, 64) != 0) {
#endif
                return ErrorCode::InvalidInput; // Geçersiz cihaz
            }

            return ErrorCode::Success;
        }

        // ============================================
        // UYGULAMA FİNGERPRİNTİNG
        // ============================================

        ErrorCode generateApplicationFingerprint(char* fingerprint) {
            if (!fingerprint) {
                return ErrorCode::InvalidInput;
            }

            std::ostringstream oss;

            // Executable dosyasının checksum'unu al
            char selfChecksum[65] = {0};
            if (RASP::calculateSelfChecksum(selfChecksum)) {
                oss << selfChecksum;
            }

            // Uygulama sürümünü al
            char appVersion[64] = {0};
            if (SessionManager::getApplicationVersion(appVersion, sizeof(appVersion)) == ErrorCode::Success) {
                oss << appVersion;
            }

            // Build zamanı (compile-time bilgisi - sabit değer)
            // Not: Gerçek uygulamada __DATE__ ve __TIME__ makroları kullanılabilir
            oss << "BUILD_2025";

            // Uygulama adı (sabit değer)
            oss << "TravelExpense";

            std::string fp = oss.str();
            if (fp.empty()) {
                // Fallback: timestamp
                fp = std::to_string(time(nullptr));
            }

            // SHA-256 hash ile fingerprint oluştur
            char hash[65];
            if (!Encryption::sha256Hash(fp.c_str(), fp.length(), hash)) {
                return ErrorCode::EncryptionFailed;
            }

            SafeString::safeCopy(fingerprint, 65, hash);

            return ErrorCode::Success;
        }

        ErrorCode verifyApplicationFingerprint(const char* expectedFingerprint) {
            if (!expectedFingerprint) {
                return ErrorCode::InvalidInput;
            }

            // Mevcut uygulama fingerprint'ini al
            char currentFingerprint[65];
            if (generateApplicationFingerprint(currentFingerprint) != ErrorCode::Success) {
                return ErrorCode::Unknown;
            }

            // Fingerprint'leri karşılaştır (case-insensitive)
#ifdef _WIN32
            if (_strnicmp(currentFingerprint, expectedFingerprint, 64) != 0) {
#else
            if (strncasecmp(currentFingerprint, expectedFingerprint, 64) != 0) {
#endif
                return ErrorCode::InvalidInput; // Geçersiz uygulama
            }

            return ErrorCode::Success;
        }

        ErrorCode generateCombinedFingerprint(char* fingerprint) {
            if (!fingerprint) {
                return ErrorCode::InvalidInput;
            }

            // Cihaz fingerprint'ini al
            char deviceFp[65] = {0};
            if (generateDeviceFingerprint(deviceFp) != ErrorCode::Success) {
                return ErrorCode::Unknown;
            }

            // Uygulama fingerprint'ini al
            char appFp[65] = {0};
            if (generateApplicationFingerprint(appFp) != ErrorCode::Success) {
                return ErrorCode::Unknown;
            }

            // Birleştir
            std::ostringstream oss;
            oss << deviceFp;
            oss << appFp;

            std::string combined = oss.str();

            // SHA-256 hash ile birleşik fingerprint oluştur
            char hash[65];
            if (!Encryption::sha256Hash(combined.c_str(), combined.length(), hash)) {
                return ErrorCode::EncryptionFailed;
            }

            SafeString::safeCopy(fingerprint, 65, hash);

            return ErrorCode::Success;
        }

        // ============================================
        // DİNAMİK ANAHTAR YÖNETİMİ
        // ============================================

        ErrorCode generateDynamicKey(const char* fingerprint, uint8_t* key, size_t keyLen) {
            if (!fingerprint || !key || keyLen != 32) {
                return ErrorCode::InvalidInput;
            }

            // Fingerprint'i kullanarak PBKDF2 ile anahtar türet
            // Salt olarak fingerprint'in ilk 16 byte'ını kullan
            uint8_t salt[16];
            for (int i = 0; i < 16; ++i) {
                // Hex karakterlerini byte'a çevir
                char hexByte[3] = {fingerprint[i * 2], fingerprint[i * 2 + 1], '\0'};
                salt[i] = static_cast<uint8_t>(strtol(hexByte, nullptr, 16));
            }

            // PBKDF2 ile anahtar türet (10000 iterasyon)
            const char* password = fingerprint; // Fingerprint'i password olarak kullan
            if (!Encryption::pbkdf2(password, strlen(password), salt, sizeof(salt),
                                     10000, keyLen, key)) {
                return ErrorCode::EncryptionFailed;
            }

            return ErrorCode::Success;
        }

        ErrorCode generateDeviceBasedKey(uint8_t* key, size_t keyLen) {
            if (!key || keyLen != 32) {
                return ErrorCode::InvalidInput;
            }

            // Cihaz fingerprint'ini al
            char deviceFp[65];
            if (generateDeviceFingerprint(deviceFp) != ErrorCode::Success) {
                return ErrorCode::Unknown;
            }

            // Dinamik anahtar oluştur
            return generateDynamicKey(deviceFp, key, keyLen);
        }

        ErrorCode generateApplicationBasedKey(uint8_t* key, size_t keyLen) {
            if (!key || keyLen != 32) {
                return ErrorCode::InvalidInput;
            }

            // Uygulama fingerprint'ini al
            char appFp[65];
            if (generateApplicationFingerprint(appFp) != ErrorCode::Success) {
                return ErrorCode::Unknown;
            }

            // Dinamik anahtar oluştur
            return generateDynamicKey(appFp, key, keyLen);
        }

        ErrorCode generateCombinedKey(uint8_t* key, size_t keyLen) {
            if (!key || keyLen != 32) {
                return ErrorCode::InvalidInput;
            }

            // Birleşik fingerprint'i al
            char combinedFp[65];
            if (generateCombinedFingerprint(combinedFp) != ErrorCode::Success) {
                return ErrorCode::Unknown;
            }

            // Dinamik anahtar oluştur
            return generateDynamicKey(combinedFp, key, keyLen);
        }

    } // namespace Fingerprinting

} // namespace TravelExpense

