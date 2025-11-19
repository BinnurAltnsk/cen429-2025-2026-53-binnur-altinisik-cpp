/**
 * @file sessionManager.cpp
 * @brief Seyahat Gideri Takibi - Oturum Yönetimi ve İletim Güvenliği Implementation
 *
 * Bu dosya, İletim Sırasındaki Veri Güvenliği gereksinimlerinin implementasyonlarını içerir.
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/sessionManager.h"
#include "../header/encryption.h"
#include "../header/security.h"
#include "../header/safe_string.h"
#include <cstring>
#include <ctime>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #define NOMINMAX
  #include <windows.h>
  #include <iphlpapi.h>
  #pragma comment(lib, "iphlpapi.lib")
#else
  #include <unistd.h>
  #include <sys/ioctl.h>
  #include <net/if.h>
  #include <ifaddrs.h>
#endif

namespace TravelExpense {

namespace SessionManager {

// ============================================
// OTURUM ANAHTARI YÖNETİMİ
// ============================================

ErrorCode generateSessionKey(uint8_t *sessionKey, size_t sessionKeyLen) {
  if (!sessionKey || sessionKeyLen != 32) {
    return ErrorCode::InvalidInput;
  }

  // 32 byte (256-bit) güvenli rastgele oturum anahtarı oluştur
  if (!Encryption::generateRandomBytes(sessionKey, 32)) {
    return ErrorCode::EncryptionFailed;
  }

  return ErrorCode::Success;
}

ErrorCode encryptSessionKey(const uint8_t *plainSessionKey,
                            uint8_t *encryptedSessionKey,
                            size_t &encryptedLen) {
  if (!plainSessionKey || !encryptedSessionKey) {
    return ErrorCode::InvalidInput;
  }

  // Master key (uygulama içine gömülü - gerçek uygulamada daha güvenli saklanmalı)
  static const uint8_t MASTER_KEY[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
  };
  // IV oluştur
  uint8_t iv[16];

  if (!Encryption::generateIV(iv)) {
    return ErrorCode::EncryptionFailed;
  }

  // Oturum anahtarını AES-256-CBC ile şifrele
  size_t ciphertextLen = 48; // 32 byte session key + 16 byte IV
  uint8_t *ciphertext = new uint8_t[ciphertextLen];
  size_t encryptedSize = ciphertextLen;

  if (!Encryption::encryptAES256(plainSessionKey, 32, MASTER_KEY, iv,
                                 ciphertext, encryptedSize)) {
    delete[] ciphertext;
    return ErrorCode::EncryptionFailed;
  }

  // IV'yi öne ekle (IV + şifrelenmiş anahtar)
  std::memcpy(encryptedSessionKey, iv, 16);
  std::memcpy(encryptedSessionKey + 16, ciphertext, 32);
  encryptedLen = 48;
  delete[] ciphertext;
  return ErrorCode::Success;
}

ErrorCode decryptSessionKey(const uint8_t *encryptedSessionKey,
                            size_t encryptedLen,
                            uint8_t *plainSessionKey) {
  if (!encryptedSessionKey || encryptedLen != 48 || !plainSessionKey) {
    return ErrorCode::InvalidInput;
  }

  // Master key (uygulama içine gömülü)
  static const uint8_t MASTER_KEY[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
  };
  // IV'yi çıkar
  uint8_t iv[16];
  std::memcpy(iv, encryptedSessionKey, 16);
  // Şifrelenmiş anahtarı çöz
  size_t plaintextLen = 32;

  if (!Encryption::decryptAES256(encryptedSessionKey + 16, 32, MASTER_KEY, iv,
                                 plainSessionKey, plaintextLen)) {
    return ErrorCode::DecryptionFailed;
  }

  return ErrorCode::Success;
}

// ============================================
// CİHAZ BAĞLANTISI VE SÜRÜM KONTROLÜ
// ============================================

ErrorCode getDeviceFingerprint(char *fingerprint) {
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

#else
  // Linux: MAC adresi kullan
  struct ifaddrs *ifaddr = nullptr;

  if (getifaddrs(&ifaddr) == 0) {
    for (struct ifaddrs *ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
      if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_PACKET) {
        struct sockaddr_ll *s = reinterpret_cast<struct sockaddr_ll *>(ifa->ifa_addr);

        for (int i = 0; i < 6; ++i) {
          oss << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(s->sll_addr[i]);
        }

        break; // İlk MAC adresini kullan
      }
    }

    freeifaddrs(ifaddr);
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

ErrorCode getApplicationVersion(char *version, size_t versionLen) {
  if (!version || versionLen == 0) {
    return ErrorCode::InvalidInput;
  }

  // Uygulama sürümü
  const char *appVersion = "1.0.0";
  SafeString::safeCopy(version, versionLen, appVersion);
  version[versionLen - 1] = '\0';
  return ErrorCode::Success;
}

ErrorCode validateDeviceAndVersion(const char *deviceFingerprint,
                                   const char *appVersion) {
  if (!deviceFingerprint || !appVersion) {
    return ErrorCode::InvalidInput;
  }

  // Mevcut cihaz fingerprint'ini al
  char currentFingerprint[65];

  if (getDeviceFingerprint(currentFingerprint) != ErrorCode::Success) {
    return ErrorCode::Unknown;
  }

  // Cihaz fingerprint kontrolü
  if (std::strcmp(deviceFingerprint, currentFingerprint) != 0) {
    return ErrorCode::InvalidInput; // Geçersiz cihaz
  }

  // Uygulama sürümü kontrolü
  char currentVersion[64];

  if (getApplicationVersion(currentVersion, sizeof(currentVersion)) != ErrorCode::Success) {
    return ErrorCode::Unknown;
  }

  if (std::strcmp(appVersion, currentVersion) != 0) {
    return ErrorCode::InvalidInput; // Geçersiz sürüm
  }

  return ErrorCode::Success;
}

// ============================================
// GİZLİ VERİ (CONFIDENTIAL PAYLOAD)
// ============================================

ErrorCode encryptPayload(const void *plaintext, size_t plaintextLen,
                         const uint8_t *sessionKey,
                         void *ciphertext, size_t &ciphertextLen) {
  if (!plaintext || plaintextLen == 0 || !sessionKey || !ciphertext) {
    return ErrorCode::InvalidInput;
  }

  // IV oluştur
  uint8_t iv[16];

  if (!Encryption::generateIV(iv)) {
    return ErrorCode::EncryptionFailed;
  }

  // AES-256-CBC ile şifrele
  size_t encryptedSize = ((plaintextLen + 15) / 16) * 16 + 16; // Padding + IV
  uint8_t *tempCiphertext = new uint8_t[encryptedSize];
  size_t actualSize = encryptedSize;

  if (!Encryption::encryptAES256(plaintext, plaintextLen, sessionKey, iv,
                                 tempCiphertext, actualSize)) {
    delete[] tempCiphertext;
    return ErrorCode::EncryptionFailed;
  }

  // IV'yi öne ekle
  std::memcpy(ciphertext, iv, 16);
  std::memcpy(static_cast<uint8_t *>(ciphertext) + 16, tempCiphertext, actualSize);
  ciphertextLen = 16 + actualSize;
  delete[] tempCiphertext;
  return ErrorCode::Success;
}

ErrorCode decryptPayload(const void *ciphertext, size_t ciphertextLen,
                         const uint8_t *sessionKey,
                         void *plaintext, size_t &plaintextLen) {
  if (!ciphertext || ciphertextLen < 16 || !sessionKey || !plaintext) {
    return ErrorCode::InvalidInput;
  }

  // IV'yi çıkar
  uint8_t iv[16];
  std::memcpy(iv, ciphertext, 16);
  // Şifreyi çöz
  size_t plaintextSize = ciphertextLen - 16;

  if (!Encryption::decryptAES256(static_cast<const uint8_t *>(ciphertext) + 16,
                                 plaintextSize, sessionKey, iv,
                                 plaintext, plaintextLen)) {
    return ErrorCode::DecryptionFailed;
  }

  return ErrorCode::Success;
}

// ============================================
// BÜTÜNLÜK KONTROLÜ VE KİMLİK DOĞRULAMA
// ============================================

ErrorCode calculateHMAC(const void *data, size_t dataLen,
                        const uint8_t *sessionKey,
                        char *hmac) {
  if (!data || dataLen == 0 || !sessionKey || !hmac) {
    return ErrorCode::InvalidInput;
  }

  // HMAC-SHA256 hesapla
  if (!Encryption::hmacSHA256(sessionKey, 32, data, dataLen, hmac)) {
    return ErrorCode::EncryptionFailed;
  }

  return ErrorCode::Success;
}

ErrorCode verifyHMAC(const void *data, size_t dataLen,
                     const uint8_t *sessionKey,
                     const char *expectedHMAC) {
  if (!data || dataLen == 0 || !sessionKey || !expectedHMAC) {
    return ErrorCode::InvalidInput;
  }

  // HMAC hesapla
  char calculatedHMAC[65];

  if (calculateHMAC(data, dataLen, sessionKey, calculatedHMAC) != ErrorCode::Success) {
    return ErrorCode::EncryptionFailed;
  }

  // Constant-time karşılaştırma
  if (!Encryption::constantTimeCompare(calculatedHMAC, expectedHMAC, 64)) {
    return ErrorCode::ChecksumMismatch;
  }

  return ErrorCode::Success;
}

// ============================================
// SUNUCU DOĞRULAMA KODU (DİJİTAL İMZA)
// ============================================

ErrorCode signData(const void *data, size_t dataLen,
                   char *signature) {
  if (!data || dataLen == 0 || !signature) {
    return ErrorCode::InvalidInput;
  }

  // Master key ile HMAC hesapla (dijital imza olarak)
  static const uint8_t SIGNATURE_KEY[32] = {
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
  };

  // HMAC-SHA256 hesapla
  if (!Encryption::hmacSHA256(SIGNATURE_KEY, 32, data, dataLen, signature)) {
    return ErrorCode::EncryptionFailed;
  }

  // İki kez hash'le (daha güçlü imza)
  char doubleHash[65];

  if (!Encryption::sha256Hash(signature, 64, doubleHash)) {
    return ErrorCode::EncryptionFailed;
  }

  SafeString::safeCopy(signature, 65, doubleHash);
  signature[64] = '\0';
  // İkinci hash'i ekle (128 karakter toplam imza)
  std::strncat(signature, doubleHash, 64);
  signature[128] = '\0';
  return ErrorCode::Success;
}

ErrorCode verifySignature(const void *data, size_t dataLen,
                          const char *signature) {
  if (!data || dataLen == 0 || !signature) {
    return ErrorCode::InvalidInput;
  }

  // İmza hesapla
  char calculatedSignature[129];

  if (signData(data, dataLen, calculatedSignature) != ErrorCode::Success) {
    return ErrorCode::EncryptionFailed;
  }

  // Constant-time karşılaştırma
  if (!Encryption::constantTimeCompare(calculatedSignature, signature, 128)) {
    return ErrorCode::ChecksumMismatch;
  }

  return ErrorCode::Success;
}

} // namespace SessionManager

} // namespace TravelExpense
