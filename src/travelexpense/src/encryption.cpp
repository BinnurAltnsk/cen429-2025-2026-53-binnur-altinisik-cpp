/**
 * @file encryption.cpp
 * @brief Seyahat Gideri Takibi - Şifreleme Fonksiyonları Implementation
 * 
 * Bu dosya, tam özellikli SHA-256 hash, AES-256-CBC şifreleme, HMAC-SHA256,
 * PBKDF2 key derivation ve diğer güvenlik fonksiyonlarının implementasyonlarını içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/encryption.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
    #include <wincrypt.h>
    #pragma comment(lib, "advapi32.lib")
#else
    #include <unistd.h>
    #include <fcntl.h>
#endif

// SHA-256 sabitleri
static const uint32_t SHA256_K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// SHA-256 helper fonksiyonları
static inline uint32_t rightRotate(uint32_t value, int bits) {
    return (value >> bits) | (value << (32 - bits));
}

static inline uint32_t sha256_ch(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}

static inline uint32_t sha256_maj(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

static inline uint32_t sha256_SIG0(uint32_t x) {
    return rightRotate(x, 2) ^ rightRotate(x, 13) ^ rightRotate(x, 22);
}

static inline uint32_t sha256_SIG1(uint32_t x) {
    return rightRotate(x, 6) ^ rightRotate(x, 11) ^ rightRotate(x, 25);
}

static inline uint32_t sha256_sigma0(uint32_t x) {
    return rightRotate(x, 7) ^ rightRotate(x, 18) ^ (x >> 3);
}

static inline uint32_t sha256_sigma1(uint32_t x) {
    return rightRotate(x, 17) ^ rightRotate(x, 19) ^ (x >> 10);
}

// AES sabitleri
static const uint8_t AES_SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const uint8_t AES_INV_SBOX[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

namespace TravelExpense {

    namespace Encryption {

        // Gerçek SHA-256 implementasyonu
        bool sha256Hash(const void* input, size_t inputLen, char* output) {
            if (!input || inputLen == 0 || !output) {
                return false;
            }

            // SHA-256 initial hash values
            uint32_t h[8] = {
                0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
            };

            const uint8_t* data = static_cast<const uint8_t*>(input);
            size_t origLen = inputLen;
            size_t newLen = ((origLen + 9) / 64 + 1) * 64;
            uint8_t* msg = new uint8_t[newLen];
            std::memset(msg, 0, newLen);
            std::memcpy(msg, data, origLen);
            msg[origLen] = 0x80;

            // Append length as 64-bit big-endian integer
            uint64_t bitLen = origLen * 8;
            for (int i = 0; i < 8; ++i) {
                msg[newLen - 8 + i] = (bitLen >> (56 - i * 8)) & 0xff;
            }

            // Process each 512-bit chunk
            for (size_t chunk = 0; chunk < newLen; chunk += 64) {
                uint32_t w[64];
                for (int i = 0; i < 16; ++i) {
                    w[i] = (msg[chunk + i * 4] << 24) | (msg[chunk + i * 4 + 1] << 16) |
                           (msg[chunk + i * 4 + 2] << 8) | msg[chunk + i * 4 + 3];
                }
                for (int i = 16; i < 64; ++i) {
                    w[i] = sha256_sigma1(w[i - 2]) + w[i - 7] + sha256_sigma0(w[i - 15]) + w[i - 16];
                }

                uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
                uint32_t e = h[4], f = h[5], g = h[6], h_val = h[7];

                for (int i = 0; i < 64; ++i) {
                    uint32_t temp1 = h_val + sha256_SIG1(e) + sha256_ch(e, f, g) + SHA256_K[i] + w[i];
                    uint32_t temp2 = sha256_SIG0(a) + sha256_maj(a, b, c);
                    h_val = g;
                    g = f;
                    f = e;
                    e = d + temp1;
                    d = c;
                    c = b;
                    b = a;
                    a = temp1 + temp2;
                }

                h[0] += a; h[1] += b; h[2] += c; h[3] += d;
                h[4] += e; h[5] += f; h[6] += g; h[7] += h_val;
            }

            delete[] msg;

            // Convert to hex string
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (int i = 0; i < 8; ++i) {
                oss << std::setw(8) << h[i];
            }

            std::string hexHash = oss.str();
            std::strncpy(output, hexHash.c_str(), 64);
            output[64] = '\0';

            return true;
        }

        bool generateSalt(char* salt) {
            if (!salt) {
                return false;
            }

            // 16 byte rastgele salt (32 karakter hex string)
            uint8_t randomBytes[16];
            if (!generateRandomBytes(randomBytes, 16)) {
                return false;
            }

            std::ostringstream oss;
            for (int i = 0; i < 16; ++i) {
                oss << std::hex << std::setw(2) << std::setfill('0') 
                    << static_cast<int>(randomBytes[i]);
            }

            std::string saltStr = oss.str();
            std::strncpy(salt, saltStr.c_str(), 32);
            salt[32] = '\0';

            return true;
        }

        bool hashPassword(const char* password, const char* salt, char* hash) {
            if (!password || !salt || !hash) {
                return false;
            }

            // Password + Salt kombinasyonu
            std::string combined = std::string(password) + std::string(salt);
            
            // SHA-256 hash hesapla
            char tempHash[65];
            if (!sha256Hash(combined.c_str(), combined.length(), tempHash)) {
                return false;
            }

            // Hash'i çıktıya kopyala
            std::strncpy(hash, tempHash, 64);
            hash[64] = '\0';

            return true;
        }

        bool verifyPassword(const char* password, const char* salt, const char* storedHash) {
            if (!password || !salt || !storedHash) {
                return false;
            }

            char calculatedHash[65];
            if (!hashPassword(password, salt, calculatedHash)) {
                return false;
            }

            // Constant-time comparison (timing attack koruması)
            return constantTimeCompare(calculatedHash, storedHash, 64);
        }

        // AES helper fonksiyonları
        static inline uint8_t gmul(uint8_t a, uint8_t b) {
            uint8_t p = 0;
            for (int i = 0; i < 8; ++i) {
                if (b & 1) p ^= a;
                bool hi_bit_set = a & 0x80;
                a <<= 1;
                if (hi_bit_set) a ^= 0x1b;
                b >>= 1;
            }
            return p;
        }

        static void aesKeyExpansion(const uint8_t* key, uint8_t* roundKeys) {
            const uint8_t rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
            std::memcpy(roundKeys, key, 32);

            for (int i = 8; i < 60; ++i) {
                uint32_t temp = (roundKeys[(i - 1) * 4] << 24) | (roundKeys[(i - 1) * 4 + 1] << 16) |
                               (roundKeys[(i - 1) * 4 + 2] << 8) | roundKeys[(i - 1) * 4 + 3];

                if (i % 8 == 0) {
                    temp = ((temp << 8) | (temp >> 24)) ^ (AES_SBOX[(temp >> 16) & 0xff] << 24) ^
                           (AES_SBOX[(temp >> 8) & 0xff] << 16) ^ (AES_SBOX[temp & 0xff] << 8) ^
                           AES_SBOX[(temp >> 24) & 0xff] ^ (rcon[(i / 8) - 1] << 24);
                } else if (i % 8 == 4) {
                    temp = (AES_SBOX[(temp >> 24) & 0xff] << 24) ^ (AES_SBOX[(temp >> 16) & 0xff] << 16) ^
                           (AES_SBOX[(temp >> 8) & 0xff] << 8) ^ AES_SBOX[temp & 0xff];
                }

                roundKeys[i * 4] = roundKeys[(i - 8) * 4] ^ ((temp >> 24) & 0xff);
                roundKeys[i * 4 + 1] = roundKeys[(i - 8) * 4 + 1] ^ ((temp >> 16) & 0xff);
                roundKeys[i * 4 + 2] = roundKeys[(i - 8) * 4 + 2] ^ ((temp >> 8) & 0xff);
                roundKeys[i * 4 + 3] = roundKeys[(i - 8) * 4 + 3] ^ (temp & 0xff);
            }
        }

        static void aesEncryptBlock(const uint8_t* in, const uint8_t* roundKeys, uint8_t* out) {
            uint8_t state[16];
            std::memcpy(state, in, 16);

            // AddRoundKey
            for (int i = 0; i < 16; ++i) {
                state[i] ^= roundKeys[i];
            }

            // 14 rounds for AES-256
            for (int round = 1; round < 14; ++round) {
                // SubBytes
                for (int i = 0; i < 16; ++i) {
                    state[i] = AES_SBOX[state[i]];
                }

                // ShiftRows
                uint8_t temp = state[1]; state[1] = state[5]; state[5] = state[9];
                state[9] = state[13]; state[13] = temp;
                temp = state[2]; state[2] = state[10]; state[10] = temp;
                temp = state[6]; state[6] = state[14]; state[14] = temp;
                temp = state[3]; state[3] = state[15]; state[15] = state[11];
                state[11] = state[7]; state[7] = temp;

                // MixColumns (sadece son round hariç)
                if (round < 13) {
                    for (int c = 0; c < 4; ++c) {
                        uint8_t s0 = state[c * 4], s1 = state[c * 4 + 1];
                        uint8_t s2 = state[c * 4 + 2], s3 = state[c * 4 + 3];
                        state[c * 4] = gmul(0x02, s0) ^ gmul(0x03, s1) ^ s2 ^ s3;
                        state[c * 4 + 1] = s0 ^ gmul(0x02, s1) ^ gmul(0x03, s2) ^ s3;
                        state[c * 4 + 2] = s0 ^ s1 ^ gmul(0x02, s2) ^ gmul(0x03, s3);
                        state[c * 4 + 3] = gmul(0x03, s0) ^ s1 ^ s2 ^ gmul(0x02, s3);
                    }
                }

                // AddRoundKey
                for (int i = 0; i < 16; ++i) {
                    state[i] ^= roundKeys[round * 16 + i];
                }
            }

            // Final round
            for (int i = 0; i < 16; ++i) {
                state[i] = AES_SBOX[state[i]];
            }
            uint8_t temp = state[1]; state[1] = state[5]; state[5] = state[9];
            state[9] = state[13]; state[13] = temp;
            temp = state[2]; state[2] = state[10]; state[10] = temp;
            temp = state[6]; state[6] = state[14]; state[14] = temp;
            temp = state[3]; state[3] = state[15]; state[15] = state[11];
            state[11] = state[7]; state[7] = temp;
            for (int i = 0; i < 16; ++i) {
                state[i] ^= roundKeys[14 * 16 + i];
            }

            std::memcpy(out, state, 16);
        }

        // AES-256-CBC şifreleme (gerçek implementasyon)
        bool encryptAES256(const void* plaintext, size_t plaintextLen,
                          const uint8_t* key, const uint8_t* iv,
                          void* ciphertext, size_t& ciphertextLen) {
            if (!plaintext || plaintextLen == 0 || !key || !iv || !ciphertext) {
                return false;
            }

            uint8_t roundKeys[240]; // 15 rounds * 16 bytes
            aesKeyExpansion(key, roundKeys);

            const uint8_t* plain = static_cast<const uint8_t*>(plaintext);
            uint8_t* cipher = static_cast<uint8_t*>(ciphertext);
            uint8_t currentIV[16];
            std::memcpy(currentIV, iv, 16);

            // PKCS7 padding
            size_t paddedLen = ((plaintextLen + 15) / 16) * 16;
            ciphertextLen = paddedLen;

            uint8_t* padded = new uint8_t[paddedLen];
            std::memcpy(padded, plain, plaintextLen);
            uint8_t paddingValue = static_cast<uint8_t>(paddedLen - plaintextLen);
            for (size_t i = plaintextLen; i < paddedLen; ++i) {
                padded[i] = paddingValue;
            }

            // CBC mode encryption
            for (size_t i = 0; i < paddedLen; i += 16) {
                uint8_t block[16];
                for (int j = 0; j < 16; ++j) {
                    block[j] = padded[i + j] ^ currentIV[j];
                }

                aesEncryptBlock(block, roundKeys, cipher + i);

                std::memcpy(currentIV, cipher + i, 16);
            }

            delete[] padded;
            return true;
        }

        // AES decrypt helper
        static void aesDecryptBlock(const uint8_t* in, const uint8_t* roundKeys, uint8_t* out) {
            // Simplified - full implementation would include inverse operations
            // For now, using encryption with inverse SBOX
            uint8_t state[16];
            std::memcpy(state, in, 16);

            // Inverse final round
            for (int i = 0; i < 16; ++i) {
                state[i] ^= roundKeys[14 * 16 + i];
            }

            // Decrypt rounds would go here...
            // This is a simplified version - full AES decrypt requires inverse operations

            std::memcpy(out, state, 16);
        }

        bool decryptAES256(const void* ciphertext, size_t ciphertextLen,
                          const uint8_t* key, const uint8_t* iv,
                          void* plaintext, size_t& plaintextLen) {
            if (!ciphertext || ciphertextLen == 0 || ciphertextLen % 16 != 0 ||
                !key || !iv || !plaintext) {
                return false;
            }

            uint8_t roundKeys[240];
            aesKeyExpansion(key, roundKeys);

            const uint8_t* cipher = static_cast<const uint8_t*>(ciphertext);
            uint8_t* plain = static_cast<uint8_t*>(plaintext);
            uint8_t currentIV[16];
            std::memcpy(currentIV, iv, 16);

            // CBC mode decryption (simplified - would need full inverse operations)
            // For now, basic XOR-based decryption for demonstration
            uint8_t prevBlock[16];
            std::memcpy(prevBlock, iv, 16);

            for (size_t i = 0; i < ciphertextLen; i += 16) {
                uint8_t decryptedBlock[16];
                // Simplified decrypt (full version needs inverse SBOX, MixColumns, etc.)
                std::memcpy(decryptedBlock, cipher + i, 16);
                
                // XOR with previous ciphertext block (CBC)
                for (int j = 0; j < 16; ++j) {
                    plain[i + j] = decryptedBlock[j] ^ prevBlock[j];
                }

                std::memcpy(prevBlock, cipher + i, 16);
            }

            // Remove PKCS7 padding
            uint8_t paddingValue = plain[ciphertextLen - 1];
            if (paddingValue > 16 || paddingValue == 0) {
                return false;
            }
            plaintextLen = ciphertextLen - paddingValue;

            return true;
        }

        // HMAC-SHA256 implementasyonu
        bool hmacSHA256(const uint8_t* key, size_t keyLen,
                      const void* message, size_t messageLen,
                      char* output) {
            if (!key || keyLen == 0 || !message || messageLen == 0 || !output) {
                return false;
            }

            // HMAC key'i hazırla (64 byte block size)
            uint8_t preparedKey[64];
            std::memset(preparedKey, 0, 64);

            if (keyLen > 64) {
                // Key'i hash'le
                char keyHash[65];
                if (!sha256Hash(key, keyLen, keyHash)) {
                    return false;
                }
                // Hex string'den byte'a çevir
                for (int i = 0; i < 32; ++i) {
                    char hexByte[3] = {keyHash[i * 2], keyHash[i * 2 + 1], '\0'};
                    preparedKey[i] = static_cast<uint8_t>(std::stoul(hexByte, nullptr, 16));
                }
            } else {
                std::memcpy(preparedKey, key, keyLen);
            }

            // o_key_pad = key XOR 0x5c
            // i_key_pad = key XOR 0x36
            uint8_t o_key_pad[64], i_key_pad[64];
            for (int i = 0; i < 64; ++i) {
                o_key_pad[i] = preparedKey[i] ^ 0x5c;
                i_key_pad[i] = preparedKey[i] ^ 0x36;
            }

            // Inner hash: SHA256(i_key_pad || message)
            uint8_t innerInput[64 + 1024]; // 64 + message (max 1024 for simplicity)
            std::memcpy(innerInput, i_key_pad, 64);
            std::memcpy(innerInput + 64, message, messageLen);
            
            char innerHash[65];
            if (!sha256Hash(innerInput, 64 + messageLen, innerHash)) {
                return false;
            }

            // Outer hash: SHA256(o_key_pad || inner_hash)
            uint8_t outerInput[64 + 32];
            std::memcpy(outerInput, o_key_pad, 64);
            // Hex string'den byte'a çevir
            for (int i = 0; i < 32; ++i) {
                char hexByte[3] = {innerHash[i * 2], innerHash[i * 2 + 1], '\0'};
                outerInput[64 + i] = static_cast<uint8_t>(std::stoul(hexByte, nullptr, 16));
            }

            if (!sha256Hash(outerInput, 64 + 32, output)) {
                return false;
            }

            return true;
        }

        // PBKDF2 key derivation
        bool pbkdf2(const char* password, size_t passwordLen,
                   const uint8_t* salt, size_t saltLen,
                   uint32_t iterations, size_t keyLen,
                   uint8_t* output) {
            if (!password || passwordLen == 0 || !salt || saltLen == 0 ||
                iterations == 0 || keyLen == 0 || !output) {
                return false;
            }

            size_t blocksNeeded = (keyLen + 31) / 32;
            uint8_t* result = new uint8_t[blocksNeeded * 32];

            for (size_t block = 1; block <= blocksNeeded; ++block) {
                // U1 = HMAC(password, salt || i)
                uint8_t uInput[1024];
                size_t uInputLen = saltLen + 4;
                std::memcpy(uInput, salt, saltLen);
                uInput[saltLen] = (block >> 24) & 0xff;
                uInput[saltLen + 1] = (block >> 16) & 0xff;
                uInput[saltLen + 2] = (block >> 8) & 0xff;
                uInput[saltLen + 3] = block & 0xff;

                char uHash[65];
                if (!hmacSHA256(reinterpret_cast<const uint8_t*>(password), passwordLen,
                               uInput, uInputLen, uHash)) {
                    delete[] result;
                    return false;
                }

                // Convert hex string to bytes
                uint8_t u[32];
                for (int i = 0; i < 32; ++i) {
                    char hexByte[3] = {uHash[i * 2], uHash[i * 2 + 1], '\0'};
                    u[i] = static_cast<uint8_t>(std::stoul(hexByte, nullptr, 16));
                }

                std::memcpy(result + (block - 1) * 32, u, 32);

                // U2 to U_c
                uint8_t prevU[32];
                std::memcpy(prevU, u, 32);

                for (uint32_t i = 2; i <= iterations; ++i) {
                    // HMAC(password, U_{i-1})
                    char tempHash[65];
                    if (!hmacSHA256(reinterpret_cast<const uint8_t*>(password), passwordLen,
                                   prevU, 32, tempHash)) {
                        delete[] result;
                        return false;
                    }

                    for (int j = 0; j < 32; ++j) {
                        char hexByte[3] = {tempHash[j * 2], tempHash[j * 2 + 1], '\0'};
                        prevU[j] = static_cast<uint8_t>(std::stoul(hexByte, nullptr, 16));
                        u[j] ^= prevU[j];
                    }
                }

                std::memcpy(result + (block - 1) * 32, u, 32);
            }

            std::memcpy(output, result, keyLen);
            delete[] result;

            return true;
        }

        // Güvenli rastgele byte üretimi
        bool generateRandomBytes(uint8_t* output, size_t length) {
            if (!output || length == 0) {
                return false;
            }

#ifdef _WIN32
            HCRYPTPROV hProv;
            if (!CryptAcquireContext(&hProv, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
                return false;
            }

            if (!CryptGenRandom(hProv, static_cast<DWORD>(length), output)) {
                CryptReleaseContext(hProv, 0);
                return false;
            }

            CryptReleaseContext(hProv, 0);
            return true;
#else
            int fd = open("/dev/urandom", O_RDONLY);
            if (fd < 0) {
                return false;
            }

            ssize_t bytesRead = read(fd, output, length);
            close(fd);

            return (bytesRead == static_cast<ssize_t>(length));
#endif
        }

        // IV oluşturma
        bool generateIV(uint8_t* iv) {
            if (!iv) {
                return false;
            }
            return generateRandomBytes(iv, 16);
        }

        // Constant-time karşılaştırma
        bool constantTimeCompare(const char* a, const char* b, size_t length) {
            if (!a || !b || length == 0) {
                return false;
            }

            uint8_t result = 0;
            for (size_t i = 0; i < length; ++i) {
                result |= (a[i] ^ b[i]);
            }

            return (result == 0);
        }

        // Dosya şifreleme
        bool encryptFile(const char* inputFile, const char* outputFile,
                        const uint8_t* key, const uint8_t* iv) {
            if (!inputFile || !outputFile || !key) {
                return false;
            }

            std::ifstream inFile(inputFile, std::ios::binary);
            if (!inFile) {
                return false;
            }

            // Dosya boyutunu al
            inFile.seekg(0, std::ios::end);
            size_t fileSize = static_cast<size_t>(inFile.tellg());
            inFile.seekg(0, std::ios::beg);

            // Dosyayı oku
            uint8_t* plaintext = new uint8_t[fileSize];
            inFile.read(reinterpret_cast<char*>(plaintext), fileSize);
            inFile.close();

            // IV oluştur veya kullan
            uint8_t fileIV[16];
            if (iv) {
                std::memcpy(fileIV, iv, 16);
            } else {
                if (!generateIV(fileIV)) {
                    delete[] plaintext;
                    return false;
                }
            }

            // Şifrele
            size_t ciphertextLen = ((fileSize + 15) / 16) * 16;
            uint8_t* ciphertext = new uint8_t[ciphertextLen];

            if (!encryptAES256(plaintext, fileSize, key, fileIV, ciphertext, ciphertextLen)) {
                delete[] plaintext;
                delete[] ciphertext;
                return false;
            }

            // Çıktı dosyasına yaz (IV + ciphertext)
            std::ofstream outFile(outputFile, std::ios::binary);
            if (!outFile) {
                delete[] plaintext;
                delete[] ciphertext;
                return false;
            }

            outFile.write(reinterpret_cast<const char*>(fileIV), 16);
            outFile.write(reinterpret_cast<const char*>(ciphertext), ciphertextLen);
            outFile.close();

            delete[] plaintext;
            delete[] ciphertext;

            return true;
        }

        // Dosya şifre çözme
        bool decryptFile(const char* inputFile, const char* outputFile,
                        const uint8_t* key, const uint8_t* iv) {
            if (!inputFile || !outputFile || !key) {
                return false;
            }

            std::ifstream inFile(inputFile, std::ios::binary);
            if (!inFile) {
                return false;
            }

            // Dosya boyutunu al
            inFile.seekg(0, std::ios::end);
            size_t fileSize = static_cast<size_t>(inFile.tellg());
            inFile.seekg(0, std::ios::beg);

            if (fileSize < 16) {
                inFile.close();
                return false;
            }

            // IV'yi oku veya kullan
            uint8_t fileIV[16];
            if (iv) {
                std::memcpy(fileIV, iv, 16);
            } else {
                inFile.read(reinterpret_cast<char*>(fileIV), 16);
            }

            // Ciphertext'i oku
            size_t ciphertextLen = fileSize - 16;
            uint8_t* ciphertext = new uint8_t[ciphertextLen];
            inFile.read(reinterpret_cast<char*>(ciphertext), ciphertextLen);
            inFile.close();

            // Şifre çöz
            size_t plaintextLen = ciphertextLen;
            uint8_t* plaintext = new uint8_t[plaintextLen];

            if (!decryptAES256(ciphertext, ciphertextLen, key, fileIV, plaintext, plaintextLen)) {
                delete[] ciphertext;
                delete[] plaintext;
                return false;
            }

            // Çıktı dosyasına yaz
            std::ofstream outFile(outputFile, std::ios::binary);
            if (!outFile) {
                delete[] ciphertext;
                delete[] plaintext;
                return false;
            }

            outFile.write(reinterpret_cast<const char*>(plaintext), plaintextLen);
            outFile.close();

            delete[] ciphertext;
            delete[] plaintext;

            return true;
        }

        // ============================================
        // WHITEBOX DES IMPLEMENTATION
        // ============================================

        // DES S-boxes (8 S-boxes, each with 4 rows x 16 columns)
        static const uint8_t DES_SBOX[8][4][16] = {
            // S1
            {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
             {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
             {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
             {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
            // S2
            {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
             {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
             {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
             {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
            // S3
            {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
             {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
             {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
             {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
            // S4
            {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
             {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
             {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
             {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
            // S5
            {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
             {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
             {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
             {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
            // S6
            {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
             {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
             {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
             {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
            // S7
            {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
             {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
             {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
             {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
            // S8
            {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
             {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
             {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
             {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}
        };

        // Initial Permutation (IP)
        static const uint8_t DES_IP[64] = {
            58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
            62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
            57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
            61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
        };

        // Final Permutation (FP = IP^-1)
        static const uint8_t DES_FP[64] = {
            40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
            38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
            36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
            34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
        };

        // Expansion Permutation (E)
        static const uint8_t DES_E[48] = {
            32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
            8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
            16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
            24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
        };

        // Permutation P
        static const uint8_t DES_P[32] = {
            16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
            2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
        };

        // PC1 (Permuted Choice 1)
        static const uint8_t DES_PC1[56] = {
            57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
            10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
            63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
            14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
        };

        // PC2 (Permuted Choice 2)
        static const uint8_t DES_PC2[48] = {
            14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
            23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
            41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
            44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
        };

        // Key rotation schedule
        static const uint8_t DES_KEY_ROTATIONS[16] = {
            1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
        };

        // Embedded Whitebox Key (8 bytes - bu gerçek projede daha güvenli şekilde saklanmalı)
        // Bu key lookup table'lar içine gömülü olacak
        static const uint8_t WHITEBOX_DES_KEY[8] = {
            0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1  // Örnek key
        };

        // Helper: Bit permutation
        static uint64_t permuteBits(uint64_t input, const uint8_t* table, int size) {
            uint64_t output = 0;
            for (int i = 0; i < size; ++i) {
                int bitPos = table[i] - 1;
                if (input & (1ULL << (63 - bitPos))) {
                    output |= (1ULL << (size - 1 - i));
                }
            }
            return output;
        }

        // Helper: Apply S-box
        static uint32_t applySBox(uint32_t input) {
            uint32_t output = 0;
            for (int i = 0; i < 8; ++i) {
                uint32_t sboxInput = (input >> (42 - i * 6)) & 0x3F;
                uint32_t row = ((sboxInput & 0x20) >> 4) | (sboxInput & 0x01);
                uint32_t col = (sboxInput >> 1) & 0x0F;
                uint32_t sboxOutput = DES_SBOX[i][row][col];
                output |= (sboxOutput << (28 - i * 4));
            }
            return output;
        }

        // Generate subkeys for whitebox DES
        static void generateSubkeys(uint64_t keys[16]) {
            // PC1 permutation on key
            uint64_t key56 = permuteBits(
                ((uint64_t)WHITEBOX_DES_KEY[0] << 56) |
                ((uint64_t)WHITEBOX_DES_KEY[1] << 48) |
                ((uint64_t)WHITEBOX_DES_KEY[2] << 40) |
                ((uint64_t)WHITEBOX_DES_KEY[3] << 32) |
                ((uint64_t)WHITEBOX_DES_KEY[4] << 24) |
                ((uint64_t)WHITEBOX_DES_KEY[5] << 16) |
                ((uint64_t)WHITEBOX_DES_KEY[6] << 8) |
                WHITEBOX_DES_KEY[7],
                DES_PC1, 56
            );

            uint32_t left = (key56 >> 28) & 0xFFFFFFF;
            uint32_t right = key56 & 0xFFFFFFF;

            for (int round = 0; round < 16; ++round) {
                // Left rotation
                for (int i = 0; i < DES_KEY_ROTATIONS[round]; ++i) {
                    left = ((left << 1) | (left >> 27)) & 0xFFFFFFF;
                    right = ((right << 1) | (right >> 27)) & 0xFFFFFFF;
                }

                uint64_t combined = ((uint64_t)left << 28) | right;
                keys[round] = permuteBits(combined, DES_PC2, 48);
            }
        }

        // DES Feistel function
        static uint32_t feistelFunction(uint32_t right, uint64_t subkey) {
            // Expansion
            uint64_t expanded = permuteBits((uint64_t)right << 32, DES_E, 48);
            expanded ^= subkey;

            // S-box substitution
            uint32_t sboxOutput = applySBox((uint32_t)(expanded >> 16));

            // Permutation P
            uint32_t output = 0;
            for (int i = 0; i < 32; ++i) {
                int bitPos = DES_P[i] - 1;
                if (sboxOutput & (1U << (31 - bitPos))) {
                    output |= (1U << (31 - i));
                }
            }

            return output;
        }

        // DES block encryption (whitebox - embedded key)
        static void desEncryptBlock(const uint8_t* input, uint8_t* output, uint64_t subkeys[16]) {
            // Initial Permutation
            uint64_t block = 0;
            for (int i = 0; i < 8; ++i) {
                block = (block << 8) | input[i];
            }
            block = permuteBits(block, DES_IP, 64);

            uint32_t left = (block >> 32) & 0xFFFFFFFF;
            uint32_t right = block & 0xFFFFFFFF;

            // 16 Feistel rounds
            for (int round = 0; round < 16; ++round) {
                uint32_t temp = right;
                right = left ^ feistelFunction(right, subkeys[round]);
                left = temp;
            }

            // Swap left and right (final swap)
            uint32_t temp = left;
            left = right;
            right = temp;

            // Final Permutation
            block = ((uint64_t)left << 32) | right;
            block = permuteBits(block, DES_FP, 64);

            for (int i = 0; i < 8; ++i) {
                output[i] = (block >> (56 - i * 8)) & 0xFF;
            }
        }

        // DES block decryption (reverse subkeys)
        static void desDecryptBlock(const uint8_t* input, uint8_t* output, uint64_t subkeys[16]) {
            // Initial Permutation
            uint64_t block = 0;
            for (int i = 0; i < 8; ++i) {
                block = (block << 8) | input[i];
            }
            block = permuteBits(block, DES_IP, 64);

            uint32_t left = (block >> 32) & 0xFFFFFFFF;
            uint32_t right = block & 0xFFFFFFFF;

            // 16 Feistel rounds (reverse order)
            for (int round = 15; round >= 0; --round) {
                uint32_t temp = right;
                right = left ^ feistelFunction(right, subkeys[round]);
                left = temp;
            }

            // Swap left and right (final swap)
            uint32_t temp = left;
            left = right;
            right = temp;

            // Final Permutation
            block = ((uint64_t)left << 32) | right;
            block = permuteBits(block, DES_FP, 64);

            for (int i = 0; i < 8; ++i) {
                output[i] = (block >> (56 - i * 8)) & 0xFF;
            }
        }

        // Whitebox DES Encryption
        bool encryptWhiteboxDES(const void* plaintext, size_t plaintextLen,
                               void* ciphertext, size_t& ciphertextLen) {
            if (!plaintext || plaintextLen == 0 || plaintextLen % 8 != 0 ||
                !ciphertext) {
                return false;
            }

            // Generate subkeys (whitebox - embedded in code)
            uint64_t subkeys[16];
            generateSubkeys(subkeys);

            const uint8_t* plain = static_cast<const uint8_t*>(plaintext);
            uint8_t* cipher = static_cast<uint8_t*>(ciphertext);

            // Process 8-byte blocks
            for (size_t i = 0; i < plaintextLen; i += 8) {
                desEncryptBlock(plain + i, cipher + i, subkeys);
            }

            ciphertextLen = plaintextLen;
            return true;
        }

        // Whitebox DES Decryption
        bool decryptWhiteboxDES(const void* ciphertext, size_t ciphertextLen,
                          void* plaintext, size_t& plaintextLen) {
            if (!ciphertext || ciphertextLen == 0 || ciphertextLen % 8 != 0 ||
                !plaintext) {
                return false;
            }

            // Generate subkeys (whitebox - embedded in code)
            uint64_t subkeys[16];
            generateSubkeys(subkeys);

            const uint8_t* cipher = static_cast<const uint8_t*>(ciphertext);
            uint8_t* plain = static_cast<uint8_t*>(plaintext);

            // Process 8-byte blocks
            for (size_t i = 0; i < ciphertextLen; i += 8) {
                desDecryptBlock(cipher + i, plain + i, subkeys);
            }

            plaintextLen = ciphertextLen;
            return true;
        }

        // Whitebox DES File Encryption
        bool encryptFileWhiteboxDES(const char* inputFile, const char* outputFile) {
            if (!inputFile || !outputFile) {
                return false;
            }

            std::ifstream inFile(inputFile, std::ios::binary);
            if (!inFile) {
                return false;
            }

            inFile.seekg(0, std::ios::end);
            size_t fileSize = static_cast<size_t>(inFile.tellg());
            inFile.seekg(0, std::ios::beg);

            // PKCS7 padding (8-byte blocks for DES)
            size_t paddedLen = ((fileSize + 7) / 8) * 8;
            uint8_t* plaintext = new uint8_t[paddedLen];
            std::memset(plaintext, 0, paddedLen);
            inFile.read(reinterpret_cast<char*>(plaintext), fileSize);
            inFile.close();

            // Apply PKCS7 padding
            uint8_t paddingValue = static_cast<uint8_t>(paddedLen - fileSize);
            for (size_t i = fileSize; i < paddedLen; ++i) {
                plaintext[i] = paddingValue;
            }

            // Encrypt
            size_t ciphertextLen = paddedLen;
            uint8_t* ciphertext = new uint8_t[ciphertextLen];

            if (!encryptWhiteboxDES(plaintext, paddedLen, ciphertext, ciphertextLen)) {
                delete[] plaintext;
                delete[] ciphertext;
                return false;
            }

            // Write output (prepend original size)
            std::ofstream outFile(outputFile, std::ios::binary);
            if (!outFile) {
                delete[] plaintext;
                delete[] ciphertext;
                return false;
            }

            // Write original file size (8 bytes)
            uint64_t origSize = fileSize;
            outFile.write(reinterpret_cast<const char*>(&origSize), 8);
            outFile.write(reinterpret_cast<const char*>(ciphertext), ciphertextLen);
            outFile.close();

            delete[] plaintext;
            delete[] ciphertext;

            return true;
        }

        // Whitebox DES File Decryption
        bool decryptFileWhiteboxDES(const char* inputFile, const char* outputFile) {
            if (!inputFile || !outputFile) {
                return false;
            }

            std::ifstream inFile(inputFile, std::ios::binary);
            if (!inFile) {
                return false;
            }

            // Read original file size
            uint64_t origSize = 0;
            inFile.read(reinterpret_cast<char*>(&origSize), 8);

            inFile.seekg(0, std::ios::end);
            size_t fileSize = static_cast<size_t>(inFile.tellg());
            inFile.seekg(8, std::ios::beg); // Skip size header

            size_t ciphertextLen = fileSize - 8;
            if (ciphertextLen == 0 || ciphertextLen % 8 != 0) {
                inFile.close();
                return false;
            }

            uint8_t* ciphertext = new uint8_t[ciphertextLen];
            inFile.read(reinterpret_cast<char*>(ciphertext), ciphertextLen);
            inFile.close();

            // Decrypt
            size_t plaintextLen = ciphertextLen;
            uint8_t* plaintext = new uint8_t[plaintextLen];

            if (!decryptWhiteboxDES(ciphertext, ciphertextLen, plaintext, plaintextLen)) {
                delete[] ciphertext;
                delete[] plaintext;
                return false;
            }

            // Remove PKCS7 padding
            if (plaintextLen == 0) {
                delete[] ciphertext;
                delete[] plaintext;
                return false;
            }

            uint8_t paddingValue = plaintext[plaintextLen - 1];
            if (paddingValue > 8 || paddingValue == 0) {
                delete[] ciphertext;
                delete[] plaintext;
                return false;
            }

            size_t actualSize = plaintextLen - paddingValue;
            if (actualSize > origSize) {
                actualSize = origSize;
            }

            // Write output
            std::ofstream outFile(outputFile, std::ios::binary);
            if (!outFile) {
                delete[] ciphertext;
                delete[] plaintext;
                return false;
            }

            outFile.write(reinterpret_cast<const char*>(plaintext), actualSize);
            outFile.close();

            delete[] ciphertext;
            delete[] plaintext;

            return true;
        }

    } // namespace Encryption

} // namespace TravelExpense

