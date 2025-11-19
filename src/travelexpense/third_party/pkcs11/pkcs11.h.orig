/**
 * @file pkcs11.h
 * @brief PKCS#11 v2.40 Standard Header
 *
 * Bu dosya PKCS#11 standardının temel tanımlarını içerir.
 * SoftHSM entegrasyonu için gerekli.
 *
 * PKCS#11 v2.40 standardına uygun minimal implementasyon
 */

#ifndef PKCS11_H
#define PKCS11_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

// PKCS#11 version
#define CK_PKCS11_VERSION_MAJOR         2
#define CK_PKCS11_VERSION_MINOR         40
#define CK_VERSION_MAJOR                 2
#define CK_VERSION_MINOR                 40

// Platform-specific types
typedef uint64_t CK_ULONG;
typedef int64_t CK_LONG;
typedef unsigned char CK_BYTE;
typedef CK_BYTE *CK_BYTE_PTR;
typedef CK_ULONG CK_SLOT_ID;
typedef CK_ULONG CK_SESSION_HANDLE;
typedef CK_ULONG CK_OBJECT_HANDLE;
typedef CK_ULONG CK_USER_TYPE;
typedef CK_ULONG CK_STATE;

// Boolean values
#define CK_FALSE                         0
#define CK_TRUE                          1

// Flags
#define CKF_RW_SESSION                   0x00000002
#define CKF_SERIAL_SESSION               0x00000004
#define CKF_TOKEN_PRESENT                0x00000100
#define CKF_HW                           0x00000001

// Return codes
#define CKR_OK                           0x00000000
#define CKR_GENERAL_ERROR                0x00000001
#define CKR_FUNCTION_FAILED              0x00000006
#define CKR_ARGUMENTS_BAD                0x00000007
#define CKR_NO_EVENT                     0x00000005
#define CKR_NEED_TO_CREATE_THREADS       0x00000001
#define CKR_CANT_LOCK                    0x0000000A
#define CKR_ATTRIBUTE_READ_ONLY          0x00000010
#define CKR_ATTRIBUTE_SENSITIVE          0x00000011
#define CKR_ATTRIBUTE_TYPE_INVALID       0x00000012
#define CKR_ATTRIBUTE_VALUE_INVALID      0x00000013
#define CKR_DATA_INVALID                 0x00000020
#define CKR_DATA_LEN_RANGE               0x00000021
#define CKR_DEVICE_ERROR                 0x00000030
#define CKR_DEVICE_MEMORY                0x00000031
#define CKR_DEVICE_REMOVED               0x00000032
#define CKR_ENCRYPTED_DATA_INVALID       0x00000040
#define CKR_ENCRYPTED_DATA_LEN_RANGE     0x00000041
#define CKR_FUNCTION_CANCELED             0x00000050
#define CKR_FUNCTION_NOT_PARALLEL         0x00000051
#define CKR_FUNCTION_NOT_SUPPORTED        0x00000054
#define CKR_KEY_HANDLE_INVALID            0x00000060
#define CKR_KEY_SIZE_RANGE                0x00000062
#define CKR_KEY_TYPE_INCONSISTENT         0x00000063
#define CKR_KEY_NOT_NEEDED                0x00000064
#define CKR_KEY_CHANGED                   0x00000065
#define CKR_KEY_NEEDED                    0x00000066
#define CKR_KEY_INDIGESTIBLE              0x00000067
#define CKR_KEY_FUNCTION_NOT_PERMITTED    0x00000068
#define CKR_KEY_NOT_WRAPPABLE             0x00000069
#define CKR_KEY_UNEXTRACTABLE             0x0000006A
#define CKR_MECHANISM_INVALID             0x00000070
#define CKR_MECHANISM_PARAM_INVALID       0x00000071
#define CKR_OBJECT_HANDLE_INVALID         0x00000082
#define CKR_OPERATION_ACTIVE              0x00000090
#define CKR_OPERATION_NOT_INITIALIZED     0x00000091
#define CKR_PIN_INCORRECT                 0x000000A0
#define CKR_PIN_INVALID                   0x000000A1
#define CKR_PIN_LEN_RANGE                 0x000000A2
#define CKR_PIN_EXPIRED                   0x000000A3
#define CKR_PIN_LOCKED                    0x000000A4
#define CKR_SESSION_CLOSED                0x000000B0
#define CKR_SESSION_COUNT                 0x000000B1
#define CKR_SESSION_HANDLE_INVALID        0x000000B3
#define CKR_SESSION_PARALLEL_NOT_SUPPORTED 0x000000B4
#define CKR_SESSION_READ_ONLY             0x000000B5
#define CKR_SESSION_EXISTS                0x000000B6
#define CKR_SESSION_READ_ONLY_EXISTS      0x000000B7
#define CKR_SESSION_READ_WRITE_SO_EXISTS  0x000000B8
#define CKR_SIGNATURE_INVALID              0x000000C0
#define CKR_SIGNATURE_LEN_RANGE           0x000000C1
#define CKR_TEMPLATE_INCOMPLETE           0x000000D0
#define CKR_TEMPLATE_INCONSISTENT          0x000000D1
#define CKR_TOKEN_NOT_PRESENT              0x000000E0
#define CKR_TOKEN_NOT_RECOGNIZED           0x000000E1
#define CKR_TOKEN_WRITE_PROTECTED         0x000000E2
#define CKR_UNWRAPPING_KEY_HANDLE_INVALID  0x000000F0
#define CKR_UNWRAPPING_KEY_SIZE_RANGE      0x000000F1
#define CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT 0x000000F2
#define CKR_USER_ALREADY_LOGGED_IN         0x00000100
#define CKR_USER_NOT_LOGGED_IN             0x00000101
#define CKR_USER_PIN_NOT_INITIALIZED       0x00000102
#define CKR_USER_TYPE_INVALID              0x00000103
#define CKR_USER_ANOTHER_ALREADY_LOGGED_IN 0x00000104
#define CKR_USER_TOO_MANY_TYPES            0x00000105
#define CKR_WRAPPED_KEY_INVALID            0x00000110
#define CKR_WRAPPED_KEY_LEN_RANGE          0x00000111
#define CKR_WRAPPING_KEY_HANDLE_INVALID    0x00000112
#define CKR_WRAPPING_KEY_SIZE_RANGE        0x00000113
#define CKR_WRAPPING_KEY_TYPE_INCONSISTENT 0x00000114
#define CKR_RANDOM_SEED_NOT_SUPPORTED      0x00000120
#define CKR_RANDOM_NO_RNG                  0x00000121
#define CKR_DOMAIN_PARAMS_INVALID          0x00000130
#define CKR_CURVE_NOT_SUPPORTED            0x00000140
#define CKR_BUFFER_TOO_SMALL               0x00000150
#define CKR_SAVED_STATE_INVALID            0x00000160
#define CKR_INFORMATION_SENSITIVE           0x00000170
#define CKR_STATE_UNSAVABLE                0x00000180
#define CKR_CRYPTOKI_NOT_INITIALIZED       0x00000190
#define CKR_CRYPTOKI_ALREADY_INITIALIZED   0x00000191
#define CKR_MUTEX_BAD                      0x000001A0
#define CKR_MUTEX_NOT_LOCKED               0x000001A1
#define CKR_NEW_PIN_MODE                   0x000001B0
#define CKR_NEXT_OTP                       0x000001B1
#define CKR_EXCEEDED_MAX_ITERATIONS        0x000001B5
#define CKR_FIPS_SELF_TEST_FAILED          0x000001B6
#define CKR_LIBRARY_LOAD_FAILED            0x000001C0
#define CKR_LIBRARY_NOT_FOUND              0x000001C1
#define CKR_FUNCTION_NOT_PARALLEL          0x000001C4
#define CKR_NOT_AUTHENTICATED              0x000001C5

// Mechanism types
#define CKM_RSA_PKCS_KEY_PAIR_GEN          0x00000000
#define CKM_RSA_PKCS                       0x00000001
#define CKM_RSA_9796                       0x00000002
#define CKM_RSA_X_509                      0x00000003
#define CKM_MD2_RSA_PKCS                   0x00000004
#define CKM_MD5_RSA_PKCS                   0x00000005
#define CKM_SHA1_RSA_PKCS                  0x00000006
#define CKM_RIPEMD128_RSA_PKCS             0x00000007
#define CKM_RIPEMD160_RSA_PKCS             0x00000008
#define CKM_RSA_PKCS_OAEP                  0x00000009
#define CKM_RSA_X9_31_KEY_PAIR_GEN         0x0000000A
#define CKM_RSA_X9_31                      0x0000000B
#define CKM_SHA1_RSA_X9_31                 0x0000000C
#define CKM_RSA_PKCS_PSS                   0x0000000D
#define CKM_SHA1_RSA_PKCS_PSS              0x0000000E
#define CKM_DSA_KEY_PAIR_GEN               0x00000010
#define CKM_DSA                            0x00000011
#define CKM_DSA_SHA1                       0x00000012
#define CKM_DH_PKCS_KEY_PAIR_GEN           0x00000020
#define CKM_DH_PKCS_DERIVE                 0x00000021
#define CKM_X9_42_DH_KEY_PAIR_GEN          0x00000030
#define CKM_X9_42_DH_DERIVE                0x00000031
#define CKM_X9_42_DH_HYBRID_DERIVE         0x00000032
#define CKM_X9_42_MQV_DERIVE               0x00000033
#define CKM_SHA256_RSA_PKCS                0x00000040
#define CKM_SHA384_RSA_PKCS                0x00000041
#define CKM_SHA512_RSA_PKCS                0x00000042
#define CKM_SHA256_RSA_PKCS_PSS            0x00000043
#define CKM_SHA384_RSA_PKCS_PSS            0x00000044
#define CKM_SHA512_RSA_PKCS_PSS            0x00000045
#define CKM_SHA224_RSA_PKCS                0x00000046
#define CKM_SHA224_RSA_PKCS_PSS            0x00000047
#define CKM_RC2_KEY_GEN                    0x00000100
#define CKM_RC2_ECB                       0x00000101
#define CKM_RC2_CBC                       0x00000102
#define CKM_RC2_MAC                       0x00000103
#define CKM_RC2_MAC_GENERAL                0x00000104
#define CKM_RC2_CBC_PAD                   0x00000105
#define CKM_RC4_KEY_GEN                   0x00000110
#define CKM_RC4                            0x00000111
#define CKM_DES_KEY_GEN                   0x00000120
#define CKM_DES_ECB                       0x00000121
#define CKM_DES_CBC                       0x00000122
#define CKM_DES_MAC                       0x00000123
#define CKM_DES_MAC_GENERAL                0x00000124
#define CKM_DES_CBC_PAD                   0x00000125
#define CKM_DES2_KEY_GEN                  0x00000130
#define CKM_DES3_KEY_GEN                  0x00000131
#define CKM_DES3_ECB                      0x00000132
#define CKM_DES3_CBC                      0x00000133
#define CKM_DES3_MAC                      0x00000134
#define CKM_DES3_MAC_GENERAL               0x00000135
#define CKM_DES3_CBC_PAD                  0x00000136
#define CKM_DES3_CMAC_GENERAL              0x00000137
#define CKM_DES3_CMAC                     0x00000138
#define CKM_CDMF_KEY_GEN                  0x00000140
#define CKM_CDMF_ECB                      0x00000141
#define CKM_CDMF_CBC                      0x00000142
#define CKM_CDMF_MAC                      0x00000143
#define CKM_CDMF_MAC_GENERAL               0x00000144
#define CKM_CDMF_CBC_PAD                  0x00000145
#define CKM_DES_OFB64                     0x00000150
#define CKM_DES_OFB8                      0x00000151
#define CKM_DES_CFB64                     0x00000152
#define CKM_DES_CFB8                      0x00000153
#define CKM_MD2                           0x00000200
#define CKM_MD2_HMAC                      0x00000201
#define CKM_MD2_HMAC_GENERAL               0x00000202
#define CKM_MD5                           0x00000210
#define CKM_MD5_HMAC                      0x00000211
#define CKM_MD5_HMAC_GENERAL               0x00000212
#define CKM_SHA_1                         0x00000220
#define CKM_SHA_1_HMAC                    0x00000221
#define CKM_SHA_1_HMAC_GENERAL             0x00000222
#define CKM_RIPEMD128                     0x00000240
#define CKM_RIPEMD128_HMAC                 0x00000241
#define CKM_RIPEMD128_HMAC_GENERAL         0x00000242
#define CKM_RIPEMD160                     0x00000243
#define CKM_RIPEMD160_HMAC                 0x00000244
#define CKM_RIPEMD160_HMAC_GENERAL         0x00000245
#define CKM_SHA256                        0x00000250
#define CKM_SHA256_HMAC                    0x00000251
#define CKM_SHA256_HMAC_GENERAL            0x00000252
#define CKM_SHA224                        0x00000255
#define CKM_SHA224_HMAC                    0x00000256
#define CKM_SHA224_HMAC_GENERAL            0x00000257
#define CKM_SHA384                        0x00000260
#define CKM_SHA384_HMAC                    0x00000261
#define CKM_SHA384_HMAC_GENERAL            0x00000262
#define CKM_SHA512                        0x00000270
#define CKM_SHA512_HMAC                    0x00000271
#define CKM_SHA512_HMAC_GENERAL            0x00000272
#define CKM_SECURID_KEY_GEN                0x00000280
#define CKM_SECURID                       0x00000282
#define CKM_HOTP_KEY_GEN                  0x00000290
#define CKM_HOTP                          0x00000291
#define CKM_ACTI_KEY_GEN                  0x000002A0
#define CKM_ACTI                          0x000002A1
#define CKM_CAST_KEY_GEN                  0x00000300
#define CKM_CAST_ECB                      0x00000301
#define CKM_CAST_CBC                      0x00000302
#define CKM_CAST_MAC                      0x00000303
#define CKM_CAST_MAC_GENERAL               0x00000304
#define CKM_CAST_CBC_PAD                  0x00000305
#define CKM_CAST3_KEY_GEN                 0x00000310
#define CKM_CAST3_ECB                     0x00000311
#define CKM_CAST3_CBC                     0x00000312
#define CKM_CAST3_MAC                     0x00000313
#define CKM_CAST3_MAC_GENERAL              0x00000314
#define CKM_CAST3_CBC_PAD                 0x00000315
#define CKM_CAST5_KEY_GEN                 0x00000320
#define CKM_CAST128_KEY_GEN               0x00000320
#define CKM_CAST5_ECB                     0x00000321
#define CKM_CAST128_ECB                   0x00000321
#define CKM_CAST5_CBC                     0x00000322
#define CKM_CAST128_CBC                   0x00000322
#define CKM_CAST5_MAC                     0x00000323
#define CKM_CAST128_MAC                   0x00000323
#define CKM_CAST5_MAC_GENERAL              0x00000324
#define CKM_CAST128_MAC_GENERAL            0x00000324
#define CKM_CAST5_CBC_PAD                 0x00000325
#define CKM_CAST128_CBC_PAD               0x00000325
#define CKM_RC5_KEY_GEN                   0x00000330
#define CKM_RC5_ECB                       0x00000331
#define CKM_RC5_CBC                       0x00000332
#define CKM_RC5_MAC                       0x00000333
#define CKM_RC5_MAC_GENERAL                0x00000334
#define CKM_RC5_CBC_PAD                   0x00000335
#define CKM_IDEA_KEY_GEN                  0x00000340
#define CKM_IDEA_ECB                      0x00000341
#define CKM_IDEA_CBC                      0x00000342
#define CKM_IDEA_MAC                      0x00000343
#define CKM_IDEA_MAC_GENERAL               0x00000344
#define CKM_IDEA_CBC_PAD                  0x00000345
#define CKM_GENERIC_SECRET_KEY_GEN         0x00000350
#define CKM_CONCATENATE_BASE_AND_KEY       0x00000360
#define CKM_CONCATENATE_BASE_AND_DATA      0x00000362
#define CKM_CONCATENATE_DATA_AND_BASE      0x00000363
#define CKM_XOR_BASE_AND_DATA              0x00000364
#define CKM_EXTRACT_KEY_FROM_KEY           0x00000365
#define CKM_SSL3_PRE_MASTER_KEY_GEN        0x00000370
#define CKM_SSL3_MASTER_KEY_DERIVE         0x00000371
#define CKM_SSL3_KEY_AND_MAC_DERIVE        0x00000372
#define CKM_SSL3_MASTER_KEY_DERIVE_DH      0x00000373
#define CKM_TLS_PRE_MASTER_KEY_GEN         0x00000374
#define CKM_TLS_MASTER_KEY_DERIVE          0x00000375
#define CKM_TLS_KEY_AND_MAC_DERIVE         0x00000376
#define CKM_TLS_MASTER_KEY_DERIVE_DH       0x00000377
#define CKM_TLS_PRF                        0x00000378
#define CKM_SSL3_MD5_MAC                   0x00000380
#define CKM_SSL3_SHA1_MAC                  0x00000381
#define CKM_MD5_KEY_DERIVATION             0x00000390
#define CKM_MD2_KEY_DERIVATION             0x00000391
#define CKM_SHA1_KEY_DERIVATION            0x00000392
#define CKM_SHA256_KEY_DERIVATION          0x00000393
#define CKM_SHA384_KEY_DERIVATION          0x00000394
#define CKM_SHA512_KEY_DERIVATION          0x00000395
#define CKM_SHA224_KEY_DERIVATION          0x00000396
#define CKM_PBE_MD2_DES_CBC                0x000003A0
#define CKM_PBE_MD5_DES_CBC                0x000003A1
#define CKM_PBE_MD5_CAST_CBC               0x000003A2
#define CKM_PBE_MD5_CAST3_CBC              0x000003A3
#define CKM_PBE_MD5_CAST5_CBC              0x000003A4
#define CKM_PBE_MD5_CAST128_CBC            0x000003A4
#define CKM_PBE_SHA1_CAST5_CBC             0x000003A5
#define CKM_PBE_SHA1_CAST128_CBC           0x000003A5
#define CKM_PBE_SHA1_RC4_128               0x000003A6
#define CKM_PBE_SHA1_RC4_40                0x000003A7
#define CKM_PBE_SHA1_DES3_EDE_CBC          0x000003A8
#define CKM_PBE_SHA1_DES2_EDE_CBC          0x000003A9
#define CKM_PBE_SHA1_RC2_128_CBC           0x000003AA
#define CKM_PBE_SHA1_RC2_40_CBC            0x000003AB
#define CKM_PKCS5_PBKD2                    0x000003B0
#define CKM_PBA_SHA1_WITH_SHA1_HMAC        0x000003C0
#define CKM_KEY_WRAP_SET_OAEP              0x000003D0
#define CKM_KEY_WRAP_SET_OAEP_PAD          0x000003D1
#define CKM_SHA_1_KEY_GEN                  0x000003E0
#define CKM_SHA256_KEY_GEN                 0x000003E1
#define CKM_SHA224_KEY_GEN                 0x000003E2
#define CKM_SHA384_KEY_GEN                 0x000003E3
#define CKM_SHA512_KEY_GEN                 0x000003E4
#define CKM_SECURE_KEY                     0x000003F0
#define CKM_XOR_BASE_AND_DATA              0x000003F2
#define CKM_EC_KEY_PAIR_GEN                0x00001040
#define CKM_EC_ECDSA                       0x00001041
#define CKM_EC_ECDSA_SHA1                  0x00001042
#define CKM_EC_ECDSA_SHA224                0x00001043
#define CKM_EC_ECDSA_SHA256                0x00001044
#define CKM_EC_ECDSA_SHA384                0x00001045
#define CKM_EC_ECDSA_SHA512                0x00001046
#define CKM_ECDH1_DERIVE                   0x00001050
#define CKM_ECDH1_COFACTOR_DERIVE          0x00001051
#define CKM_ECMQV_DERIVE                   0x00001052
#define CKM_AES_KEY_GEN                    0x00001080
#define CKM_AES_ECB                        0x00001081
#define CKM_AES_CBC                        0x00001082
#define CKM_AES_MAC                        0x00001083
#define CKM_AES_MAC_GENERAL                 0x00001084
#define CKM_AES_CBC_PAD                    0x00001085
#define CKM_AES_CTR                        0x00001086
#define CKM_AES_GCM                        0x00001087
#define CKM_AES_CCM                        0x00001088
#define CKM_AES_CTS                        0x00001089
#define CKM_AES_CMAC                       0x0000108A
#define CKM_AES_CMAC_GENERAL               0x0000108B
#define CKM_BLOWFISH_KEY_GEN               0x00001090
#define CKM_BLOWFISH_CBC                   0x00001091
#define CKM_TWOFISH_KEY_GEN                0x00001092
#define CKM_TWOFISH_CBC                    0x00001093
#define CKM_AES_GCM                        0x00001087
#define CKM_AES_CCM                        0x00001088
#define CKM_BLOWFISH_KEY_GEN               0x00001090
#define CKM_BLOWFISH_CBC                   0x00001091
#define CKM_TWOFISH_KEY_GEN                0x00001092
#define CKM_TWOFISH_CBC                    0x00001093
#define CKM_DES_ECB_ENCRYPT_DATA           0x00001120
#define CKM_DES_CBC_ENCRYPT_DATA           0x00001121
#define CKM_DES3_ECB_ENCRYPT_DATA          0x00001122
#define CKM_DES3_CBC_ENCRYPT_DATA          0x00001123
#define CKM_AES_ECB_ENCRYPT_DATA           0x00001124
#define CKM_AES_CBC_ENCRYPT_DATA           0x00001125
#define CKM_DSA_PARAMETER_GEN              0x00002000
#define CKM_DH_PKCS_PARAMETER_GEN          0x00002001
#define CKM_X9_42_DH_PARAMETER_GEN         0x00002002
#define CKM_DSA_PROBABLISTIC_PARAMETER_GEN 0x00002003
#define CKM_DSA_SHAWE_TAYLOR_PARAMETER_GEN 0x00002004
#define CKM_AES_OFB                        0x00002104
#define CKM_AES_CFB64                     0x00002105
#define CKM_AES_CFB8                      0x00002106
#define CKM_AES_CFB128                    0x00002107
#define CKM_AES_CFB1                      0x00002108
#define CKM_AES_KEY_WRAP                   0x00002109
#define CKM_AES_KEY_WRAP_PAD               0x0000210A
#define CKM_AES_KEY_WRAP_KWP               0x0000210B
#define CKM_ARIA_KEY_GEN                   0x00002110
#define CKM_ARIA_ECB                       0x00002111
#define CKM_ARIA_CBC                       0x00002112
#define CKM_ARIA_MAC                       0x00002113
#define CKM_ARIA_MAC_GENERAL                0x00002114
#define CKM_ARIA_CBC_PAD                   0x00002115
#define CKM_ARIA_ECB_ENCRYPT_DATA          0x00002116
#define CKM_ARIA_CBC_ENCRYPT_DATA          0x00002117
#define CKM_SEED_KEY_GEN                   0x00002130
#define CKM_SEED_ECB                       0x00002131
#define CKM_SEED_CBC                       0x00002132
#define CKM_SEED_MAC                       0x00002133
#define CKM_SEED_MAC_GENERAL                0x00002134
#define CKM_SEED_CBC_PAD                   0x00002135
#define CKM_SEED_ECB_ENCRYPT_DATA          0x00002136
#define CKM_SEED_CBC_ENCRYPT_DATA          0x00002137
#define CKM_CAMELLIA_KEY_GEN               0x00002155
#define CKM_CAMELLIA_ECB                   0x00002156
#define CKM_CAMELLIA_CBC                   0x00002157
#define CKM_CAMELLIA_MAC                   0x00002158
#define CKM_CAMELLIA_MAC_GENERAL            0x00002159
#define CKM_CAMELLIA_CBC_PAD               0x0000215A
#define CKM_CAMELLIA_ECB_ENCRYPT_DATA      0x0000215B
#define CKM_CAMELLIA_CBC_ENCRYPT_DATA      0x0000215C
#define CKM_CAMELLIA_CTR                   0x0000215D
#define CKM_CAMELLIA_CMAC                  0x0000215E
#define CKM_CAMELLIA_CMAC_GENERAL          0x0000215F

// Object classes
#define CKO_DATA                           0x00000000
#define CKO_CERTIFICATE                    0x00000001
#define CKO_PUBLIC_KEY                     0x00000002
#define CKO_PRIVATE_KEY                    0x00000003
#define CKO_SECRET_KEY                     0x00000004
#define CKO_HW_FEATURE                     0x00000005
#define CKO_DOMAIN_PARAMETERS              0x00000006
#define CKO_MECHANISM                      0x00000007
#define CKO_OTP_KEY                        0x00000008
#define CKO_VENDOR_DEFINED                 0x80000000

// Key types
#define CKK_RSA                            0x00000000
#define CKK_DSA                            0x00000001
#define CKK_DH                             0x00000002
#define CKK_EC                             0x00000003
#define CKK_X9_42_DH                       0x00000004
#define CKK_KEA                            0x00000005
#define CKK_GENERIC_SECRET                 0x00000010
#define CKK_RC2                            0x00000011
#define CKK_RC4                            0x00000012
#define CKK_DES                            0x00000013
#define CKK_DES2                           0x00000014
#define CKK_DES3                           0x00000015
#define CKK_CAST                           0x00000016
#define CKK_CAST3                          0x00000017
#define CKK_CAST5                          0x00000018
#define CKK_CAST128                        0x00000018
#define CKK_RC5                            0x00000019
#define CKK_IDEA                           0x0000001A
#define CKK_SKIPJACK                       0x0000001B
#define CKK_BATON                          0x0000001C
#define CKK_JUNIPER                        0x0000001D
#define CKK_CDMF                           0x0000001E
#define CKK_AES                            0x0000001F
#define CKK_BLOWFISH                       0x00000020
#define CKK_TWOFISH                        0x00000021
#define CKK_SECURID                        0x00000022
#define CKK_HOTP                           0x00000023
#define CKM_ACTI                           0x00000024
#define CKK_ARIA                           0x00000026
#define CKK_SEED                           0x00000027
#define CKK_CAMELLIA                       0x00000028
#define CKK_VENDOR_DEFINED                 0x80000000

// Attributes
#define CKA_CLASS                          0x00000000
#define CKA_TOKEN                          0x00000001
#define CKA_PRIVATE                        0x00000002
#define CKA_LABEL                          0x00000003
#define CKA_APPLICATION                    0x00000010
#define CKA_VALUE                          0x00000011
#define CKA_OBJECT_ID                      0x00000012
#define CKA_CERTIFICATE_TYPE               0x00000080
#define CKA_ISSUER                         0x00000081
#define CKA_SERIAL_NUMBER                  0x00000082
#define CKA_AC_ISSUER                      0x00000083
#define CKA_OWNER                          0x00000084
#define CKA_ATTR_TYPES                     0x00000085
#define CKA_TRUSTED                        0x00000086
#define CKA_CERTIFICATE_CATEGORY           0x00000087
#define CKA_JAVA_MIDP_SECURITY_DOMAIN      0x00000088
#define CKA_URL                            0x00000089
#define CKA_HASH_OF_SUBJECT_PUBLIC_KEY     0x0000008A
#define CKA_HASH_OF_ISSUER_PUBLIC_KEY      0x0000008B
#define CKA_NAME_HASH_ALGORITHM            0x0000008C
#define CKA_CHECK_VALUE                    0x00000090
#define CKA_KEY_TYPE                       0x00000100
#define CKA_SUBJECT                        0x00000101
#define CKA_ID                             0x00000102
#define CKA_SENSITIVE                      0x00000103
#define CKA_ENCRYPT                        0x00000104
#define CKA_DECRYPT                        0x00000105
#define CKA_WRAP                           0x00000106
#define CKA_UNWRAP                         0x00000107
#define CKA_SIGN                           0x00000108
#define CKA_SIGN_RECOVER                   0x00000109
#define CKA_VERIFY                         0x0000010A
#define CKA_VERIFY_RECOVER                 0x0000010B
#define CKA_DERIVE                         0x0000010C
#define CKA_START_DATE                     0x00000110
#define CKA_END_DATE                        0x00000111
#define CKA_MODULUS                        0x00000120
#define CKA_MODULUS_BITS                   0x00000121
#define CKA_PUBLIC_EXPONENT                0x00000122
#define CKA_PRIVATE_EXPONENT               0x00000123
#define CKA_PRIME_1                        0x00000124
#define CKA_PRIME_2                        0x00000125
#define CKA_EXPONENT_1                     0x00000126
#define CKA_EXPONENT_2                     0x00000127
#define CKA_COEFFICIENT                    0x00000128
#define CKA_PUBLIC_KEY_INFO                0x00000129
#define CKA_PRIME                          0x00000130
#define CKA_SUBPRIME                        0x00000131
#define CKA_BASE                           0x00000132
#define CKA_PRIME_BITS                     0x00000133
#define CKA_SUBPRIME_BITS                  0x00000134
#define CKA_VALUE_BITS                     0x00000160
#define CKA_VALUE_LEN                      0x00000161
#define CKA_EXTRACTABLE                    0x00000162
#define CKA_LOCAL                          0x00000163
#define CKA_NEVER_EXTRACTABLE              0x00000164
#define CKA_ALWAYS_SENSITIVE               0x00000165
#define CKA_KEY_GEN_MECHANISM              0x00000166
#define CKA_MODIFIABLE                     0x00000170
#define CKA_COPYABLE                       0x00000171
#define CKA_DESTROYABLE                    0x00000172
#define CKA_EC_PARAMS                      0x00000180
#define CKA_EC_POINT                       0x00000181
#define CKA_SECONDARY_AUTH                 0x00000200
#define CKA_AUTH_PIN_FLAGS                 0x00000201
#define CKA_ALWAYS_AUTHENTICATE            0x00000202
#define CKA_WRAP_WITH_TRUSTED              0x00000210
#define CKA_WRAP_TEMPLATE                  0x00000211
#define CKA_UNWRAP_TEMPLATE                0x00000212
#define CKA_OTP_FORMAT                     0x00000220
#define CKA_OTP_LENGTH                     0x00000221
#define CKA_OTP_TIME_INTERVAL              0x00000222
#define CKA_OTP_USER_FRIENDLY_MODE         0x00000223
#define CKA_OTP_CHALLENGE_REQUIREMENT      0x00000224
#define CKA_OTP_TIME_REQUIREMENT           0x00000225
#define CKA_OTP_COUNTER_REQUIREMENT        0x00000226
#define CKA_OTP_PIN_REQUIREMENT            0x00000227
#define CKA_OTP_COUNTER                    0x0000022E
#define CKA_OTP_TIME                       0x0000022F
#define CKA_OTP_USER_IDENTIFIER            0x0000022A
#define CKA_OTP_SERVICE_IDENTIFIER         0x0000022B
#define CKA_OTP_SERVICE_LOGO               0x0000022C
#define CKA_OTP_SERVICE_LOGO_TYPE         0x0000022D
#define CKA_GOSTR3410_PARAMS               0x00000250
#define CKA_GOSTR3411_PARAMS               0x00000251
#define CKA_GOST28147_PARAMS               0x00000252
#define CKA_HW_FEATURE_TYPE                0x00000300
#define CKA_RESET_ON_INIT                  0x00000301
#define CKA_HAS_RESET                      0x00000302
#define CKA_PIXEL_X                        0x00000400
#define CKA_PIXEL_Y                        0x00000401
#define CKA_RESOLUTION                     0x00000402
#define CKA_CHAR_ROWS                      0x00000403
#define CKA_CHAR_COLUMNS                   0x00000404
#define CKA_COLOR                          0x00000405
#define CKA_BITS_PER_PIXEL                 0x00000406
#define CKA_CHAR_SETS                      0x00000480
#define CKA_ENCODING_METHODS               0x00000481
#define CKA_MIME_TYPES                     0x00000482
#define CKA_MECHANISM_TYPE                 0x00000500
#define CKA_REQUIRED_CMS_ATTRIBUTES        0x00000501
#define CKA_DEFAULT_CMS_ATTRIBUTES         0x00000502
#define CKA_SUPPORTED_CMS_ATTRIBUTES       0x00000503
#define CKA_ALLOWED_MECHANISMS             0x00000600
#define CKA_VENDOR_DEFINED                 0x80000000

// User types
#define CKU_SO                             0
#define CKU_USER                           1
#define CKU_CONTEXT_SPECIFIC               2

// Session states
#define CKS_RO_PUBLIC_SESSION              0
#define CKS_RO_USER_FUNCTIONS              1
#define CKS_RW_PUBLIC_SESSION              2
#define CKS_RW_USER_FUNCTIONS              3
#define CKS_RW_SO_FUNCTIONS                4

// Structure definitions (simplified)
// CK_VERSION must be defined before CK_SLOT_INFO and CK_TOKEN_INFO
typedef struct CK_VERSION {
  CK_BYTE major;
  CK_BYTE minor;
} CK_VERSION;

typedef struct CK_SLOT_INFO {
  char slotDescription[64];
  char manufacturerID[32];
  CK_ULONG flags;
  CK_VERSION hardwareVersion;
  CK_VERSION firmwareVersion;
} CK_SLOT_INFO;

typedef struct CK_TOKEN_INFO {
  char label[32];
  char manufacturerID[32];
  char model[16];
  char serialNumber[16];
  CK_ULONG flags;
  CK_ULONG ulMaxSessionCount;
  CK_ULONG ulSessionCount;
  CK_ULONG ulMaxRwSessionCount;
  CK_ULONG ulRwSessionCount;
  CK_ULONG ulMaxPinLen;
  CK_ULONG ulMinPinLen;
  CK_ULONG ulTotalPublicMemory;
  CK_ULONG ulFreePublicMemory;
  CK_ULONG ulTotalPrivateMemory;
  CK_ULONG ulFreePrivateMemory;
  CK_VERSION hardwareVersion;
  CK_VERSION firmwareVersion;
  char utcTime[16];
} CK_TOKEN_INFO;

typedef struct CK_MECHANISM_INFO {
  CK_ULONG ulMinKeySize;
  CK_ULONG ulMaxKeySize;
  CK_ULONG flags;
} CK_MECHANISM_INFO;

typedef struct CK_ATTRIBUTE {
  CK_ULONG type;
  void *pValue;
  CK_ULONG ulValueLen;
} CK_ATTRIBUTE;

typedef struct CK_MECHANISM {
  CK_ULONG mechanism;
  void *pParameter;
  CK_ULONG ulParameterLen;
} CK_MECHANISM;

// Function pointer types
typedef CK_ULONG (*CK_C_Initialize)(void *);
typedef CK_ULONG (*CK_C_Finalize)(void *);
typedef CK_ULONG (*CK_C_GetInfo)(void *);
typedef CK_ULONG (*CK_C_GetSlotList)(CK_BYTE, CK_SLOT_ID *, CK_ULONG *);
typedef CK_ULONG (*CK_C_GetSlotInfo)(CK_SLOT_ID, CK_SLOT_INFO *);
typedef CK_ULONG (*CK_C_GetTokenInfo)(CK_SLOT_ID, CK_TOKEN_INFO *);
typedef CK_ULONG (*CK_C_OpenSession)(CK_SLOT_ID, CK_ULONG, void *, void *, CK_SESSION_HANDLE *);
typedef CK_ULONG (*CK_C_CloseSession)(CK_SESSION_HANDLE);
typedef CK_ULONG (*CK_C_CloseAllSessions)(CK_SLOT_ID);
typedef CK_ULONG (*CK_C_Login)(CK_SESSION_HANDLE, CK_USER_TYPE, CK_BYTE *, CK_ULONG);
typedef CK_ULONG (*CK_C_Logout)(CK_SESSION_HANDLE);
typedef CK_ULONG (*CK_C_CreateObject)(CK_SESSION_HANDLE, CK_ATTRIBUTE *, CK_ULONG, CK_OBJECT_HANDLE *);
typedef CK_ULONG (*CK_C_CopyObject)(CK_SESSION_HANDLE, CK_OBJECT_HANDLE, CK_ATTRIBUTE *, CK_ULONG, CK_OBJECT_HANDLE *);
typedef CK_ULONG (*CK_C_DestroyObject)(CK_SESSION_HANDLE, CK_OBJECT_HANDLE);
typedef CK_ULONG (*CK_C_GetObjectSize)(CK_SESSION_HANDLE, CK_OBJECT_HANDLE, CK_ULONG *);
typedef CK_ULONG (*CK_C_GetAttributeValue)(CK_SESSION_HANDLE, CK_OBJECT_HANDLE, CK_ATTRIBUTE *, CK_ULONG);
typedef CK_ULONG (*CK_C_SetAttributeValue)(CK_SESSION_HANDLE, CK_OBJECT_HANDLE, CK_ATTRIBUTE *, CK_ULONG);
typedef CK_ULONG (*CK_C_FindObjectsInit)(CK_SESSION_HANDLE, CK_ATTRIBUTE *, CK_ULONG);
typedef CK_ULONG (*CK_C_FindObjects)(CK_SESSION_HANDLE, CK_OBJECT_HANDLE *, CK_ULONG, CK_ULONG *);
typedef CK_ULONG (*CK_C_FindObjectsFinal)(CK_SESSION_HANDLE);
typedef CK_ULONG (*CK_C_EncryptInit)(CK_SESSION_HANDLE, CK_MECHANISM *, CK_OBJECT_HANDLE);
typedef CK_ULONG (*CK_C_Encrypt)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG, CK_BYTE *, CK_ULONG *);
typedef CK_ULONG (*CK_C_EncryptUpdate)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG, CK_BYTE *, CK_ULONG *);
typedef CK_ULONG (*CK_C_EncryptFinal)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG *);
typedef CK_ULONG (*CK_C_DecryptInit)(CK_SESSION_HANDLE, CK_MECHANISM *, CK_OBJECT_HANDLE);
typedef CK_ULONG (*CK_C_Decrypt)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG, CK_BYTE *, CK_ULONG *);
typedef CK_ULONG (*CK_C_DecryptUpdate)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG, CK_BYTE *, CK_ULONG *);
typedef CK_ULONG (*CK_C_DecryptFinal)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG *);
typedef CK_ULONG (*CK_C_DigestInit)(CK_SESSION_HANDLE, CK_MECHANISM *);
typedef CK_ULONG (*CK_C_Digest)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG, CK_BYTE *, CK_ULONG *);
typedef CK_ULONG (*CK_C_DigestUpdate)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG);
typedef CK_ULONG (*CK_C_DigestFinal)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG *);
typedef CK_ULONG (*CK_C_SignInit)(CK_SESSION_HANDLE, CK_MECHANISM *, CK_OBJECT_HANDLE);
typedef CK_ULONG (*CK_C_Sign)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG, CK_BYTE *, CK_ULONG *);
typedef CK_ULONG (*CK_C_SignUpdate)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG);
typedef CK_ULONG (*CK_C_SignFinal)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG *);
typedef CK_ULONG (*CK_C_VerifyInit)(CK_SESSION_HANDLE, CK_MECHANISM *, CK_OBJECT_HANDLE);
typedef CK_ULONG (*CK_C_Verify)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG, CK_BYTE *, CK_ULONG);
typedef CK_ULONG (*CK_C_VerifyUpdate)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG);
typedef CK_ULONG (*CK_C_VerifyFinal)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG);
typedef CK_ULONG (*CK_C_GenerateKey)(CK_SESSION_HANDLE, CK_MECHANISM *, CK_ATTRIBUTE *, CK_ULONG, CK_OBJECT_HANDLE *);
typedef CK_ULONG (*CK_C_GenerateKeyPair)(CK_SESSION_HANDLE, CK_MECHANISM *, CK_ATTRIBUTE *, CK_ULONG, CK_ATTRIBUTE *, CK_ULONG, CK_OBJECT_HANDLE *, CK_OBJECT_HANDLE *);
typedef CK_ULONG (*CK_C_WrapKey)(CK_SESSION_HANDLE, CK_MECHANISM *, CK_OBJECT_HANDLE, CK_OBJECT_HANDLE, CK_BYTE *, CK_ULONG *);
typedef CK_ULONG (*CK_C_UnwrapKey)(CK_SESSION_HANDLE, CK_MECHANISM *, CK_OBJECT_HANDLE, CK_BYTE *, CK_ULONG, CK_ATTRIBUTE *, CK_ULONG, CK_OBJECT_HANDLE *);
typedef CK_ULONG (*CK_C_DeriveKey)(CK_SESSION_HANDLE, CK_MECHANISM *, CK_OBJECT_HANDLE, CK_ATTRIBUTE *, CK_ULONG, CK_OBJECT_HANDLE *);
typedef CK_ULONG (*CK_C_SeedRandom)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG);
typedef CK_ULONG (*CK_C_GenerateRandom)(CK_SESSION_HANDLE, CK_BYTE *, CK_ULONG);

// PKCS#11 function list structure
typedef struct CK_FUNCTION_LIST {
  CK_VERSION version;
  CK_C_Initialize C_Initialize;
  CK_C_Finalize C_Finalize;
  CK_C_GetInfo C_GetInfo;
  CK_C_GetSlotList C_GetSlotList;
  CK_C_GetSlotInfo C_GetSlotInfo;
  CK_C_GetTokenInfo C_GetTokenInfo;
  CK_C_OpenSession C_OpenSession;
  CK_C_CloseSession C_CloseSession;
  CK_C_CloseAllSessions C_CloseAllSessions;
  CK_C_Login C_Login;
  CK_C_Logout C_Logout;
  CK_C_CreateObject C_CreateObject;
  CK_C_CopyObject C_CopyObject;
  CK_C_DestroyObject C_DestroyObject;
  CK_C_GetObjectSize C_GetObjectSize;
  CK_C_GetAttributeValue C_GetAttributeValue;
  CK_C_SetAttributeValue C_SetAttributeValue;
  CK_C_FindObjectsInit C_FindObjectsInit;
  CK_C_FindObjects C_FindObjects;
  CK_C_FindObjectsFinal C_FindObjectsFinal;
  CK_C_EncryptInit C_EncryptInit;
  CK_C_Encrypt C_Encrypt;
  CK_C_EncryptUpdate C_EncryptUpdate;
  CK_C_EncryptFinal C_EncryptFinal;
  CK_C_DecryptInit C_DecryptInit;
  CK_C_Decrypt C_Decrypt;
  CK_C_DecryptUpdate C_DecryptUpdate;
  CK_C_DecryptFinal C_DecryptFinal;
  CK_C_DigestInit C_DigestInit;
  CK_C_Digest C_Digest;
  CK_C_DigestUpdate C_DigestUpdate;
  CK_C_DigestFinal C_DigestFinal;
  CK_C_SignInit C_SignInit;
  CK_C_Sign C_Sign;
  CK_C_SignUpdate C_SignUpdate;
  CK_C_SignFinal C_SignFinal;
  CK_C_VerifyInit C_VerifyInit;
  CK_C_Verify C_Verify;
  CK_C_VerifyUpdate C_VerifyUpdate;
  CK_C_VerifyFinal C_VerifyFinal;
  CK_C_GenerateKey C_GenerateKey;
  CK_C_GenerateKeyPair C_GenerateKeyPair;
  CK_C_WrapKey C_WrapKey;
  CK_C_UnwrapKey C_UnwrapKey;
  CK_C_DeriveKey C_DeriveKey;
  CK_C_SeedRandom C_SeedRandom;
  CK_C_GenerateRandom C_GenerateRandom;
} CK_FUNCTION_LIST;

// Get function list function
typedef CK_ULONG (*CK_C_GetFunctionList)(CK_FUNCTION_LIST **);

#ifdef __cplusplus
}
#endif

#endif // PKCS11_H
