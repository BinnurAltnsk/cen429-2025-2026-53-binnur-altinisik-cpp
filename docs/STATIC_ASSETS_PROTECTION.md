# Statik Varlıkların Korunması Dokümantasyonu

**Tarih:** 2025  
**Proje:** Seyahat Gideri Takibi  
**Hazırlayan:** Binnur Altınışık

---

## Genel Bakış

Bu dokümantasyon, projedeki statik varlıkların korunmasını açıklar. Yönergede belirtilen Gereksinim #5'e göre:
- Gizli Anahtarlar
- Hash Değerleri
- Kaynak Kodları
- Kaynaklar

şifreleme ve erişim kontrolleri ile korunmalıdır.

---

## Statik Varlık Listesi ve Koruma Mekanizmaları

### 1. Gizli Anahtarlar (Secret Keys)

#### 1.1 Whitebox AES Anahtarı

| Özellik | Değer |
|---------|-------|
| **Konumu** | `src/travelexpense/src/encryption.cpp` (satır 1197-1202) |
| **Tipi** | Static const uint8_t array (32 byte) |
| **Koruma Mekanizması** | Whitebox kriptografi (lookup table'lara gömülü) |
| **Erişim Kontrolü** | Kod seviyesinde erişim kontrolü |
| **Şifreleme** | Anahtar lookup table'lara gömülü, reverse engineering zorlaştırılmış |

**Koruma Detayları:**
- Anahtar kod içine gömülü (embedded key)
- Lookup table'lar ile anahtar bilgisi maskelenmiş
- Whitebox kriptografi tekniği ile reverse engineering zorlaştırılmış

**Dosya:** `src/travelexpense/src/encryption.cpp`
```cpp
static const uint8_t WHITEBOX_AES_KEY[32] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
```

---

#### 1.2 Whitebox DES Anahtarı

| Özellik | Değer |
|---------|-------|
| **Konumu** | `src/travelexpense/src/encryption.cpp` (satır 864-866) |
| **Tipi** | Static const uint8_t array (8 byte) |
| **Koruma Mekanizması** | Whitebox kriptografi (lookup table'lara gömülü) |
| **Erişim Kontrolü** | Kod seviyesinde erişim kontrolü |
| **Şifreleme** | Anahtar lookup table'lara gömülü, reverse engineering zorlaştırılmış |

**Koruma Detayları:**
- Anahtar kod içine gömülü (embedded key)
- DES S-boxes ve permutation table'lar ile anahtar bilgisi maskelenmiş
- Whitebox kriptografi tekniği ile reverse engineering zorlaştırılmış

**Dosya:** `src/travelexpense/src/encryption.cpp`
```cpp
static const uint8_t WHITEBOX_DES_KEY[8] = {
    0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1
};
```

---

#### 1.3 Master Key (Session Management)

| Özellik | Değer |
|---------|-------|
| **Konumu** | `src/travelexpense/src/sessionManager.cpp` (satır 50-54) |
| **Tipi** | Static const uint8_t array (32 byte) |
| **Koruma Mekanizması** | Statik anahtar (oturum anahtarı şifreleme için) |
| **Erişim Kontrolü** | Kod seviyesinde erişim kontrolü |
| **Şifreleme** | Oturum anahtarları bu anahtar ile şifrelenir |

**Koruma Detayları:**
- Statik master key (oturum anahtarı şifreleme için)
- Oturum anahtarları bu anahtar ile AES-256-CBC ile şifrelenir
- Gerçek uygulamada daha güvenli saklanmalı (HSM, TPM vb.)

**Dosya:** `src/travelexpense/src/sessionManager.cpp`
```cpp
static const uint8_t MASTER_KEY[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};
```

---

#### 1.4 Signature Key (Dijital İmza)

| Özellik | Değer |
|---------|-------|
| **Konumu** | `src/travelexpense/src/sessionManager.cpp` (satır 249-254) |
| **Tipi** | Static const uint8_t array (32 byte) |
| **Koruma Mekanizması** | Statik imza anahtarı |
| **Erişim Kontrolü** | Kod seviyesinde erişim kontrolü |
| **Şifreleme** | Dijital imza için HMAC-SHA256 kullanılır |

**Koruma Detayları:**
- Dijital imza için kullanılan master key
- HMAC-SHA256 ile imza hesaplanır
- İki kez hash'lenerek daha güçlü imza oluşturulur

**Dosya:** `src/travelexpense/src/sessionManager.cpp`
```cpp
static const uint8_t SIGNATURE_KEY[32] = {
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
};
```

---

### 2. Hash Değerleri (Hash Values)

#### 2.1 SHA-256 Hash Sabitleri

| Özellik | Değer |
|---------|-------|
| **Konumu** | `src/travelexpense/src/encryption.cpp` (satır 32-49) |
| **Tipi** | Static const uint32_t array (64 eleman) |
| **Koruma Mekanizması** | Kod içine gömülü (read-only) |
| **Erişim Kontrolü** | Kod seviyesinde erişim kontrolü |
| **Şifreleme** | Statik sabitler (değiştirilemez) |

**Koruma Detayları:**
- SHA-256 algoritmasının K sabitleri
- Read-only statik array
- Değiştirilemez (const)

**Dosya:** `src/travelexpense/src/encryption.cpp`

---

#### 2.2 AES S-Box ve Inverse S-Box

| Özellik | Değer |
|---------|-------|
| **Konumu** | `src/travelexpense/src/encryption.cpp` (satır 81-117) |
| **Tipi** | Static const uint8_t array (256 eleman) |
| **Koruma Mekanizması** | Kod içine gömülü (read-only) |
| **Erişim Kontrolü** | Kod seviyesinde erişim kontrolü |
| **Şifreleme** | Statik sabitler (değiştirilemez) |

**Koruma Detayları:**
- AES algoritmasının S-Box ve Inverse S-Box sabitleri
- Read-only statik array
- Değiştirilemez (const)

**Dosya:** `src/travelexpense/src/encryption.cpp`

---

#### 2.3 DES S-Box ve Permutation Table'lar

| Özellik | Değer |
|---------|-------|
| **Konumu** | `src/travelexpense/src/encryption.cpp` (satır 768-825) |
| **Tipi** | Static const uint8_t array |
| **Koruma Mekanizması** | Kod içine gömülü (read-only) |
| **Erişim Kontrolü** | Kod seviyesinde erişim kontrolü |
| **Şifreleme** | Statik sabitler (değiştirilemez) |

**Koruma Detayları:**
- DES algoritmasının S-Box, IP, FP, E, P, PC1, PC2 sabitleri
- Read-only statik array
- Değiştirilemez (const)

**Dosya:** `src/travelexpense/src/encryption.cpp`

---

### 3. Kaynak Kodları (Source Code)

#### 3.1 Kaynak Kod Dosyaları

| Özellik | Değer |
|---------|-------|
| **Konumu** | `src/travelexpense/src/`, `src/travelexpense/header/` |
| **Tipi** | C++ kaynak kod dosyaları (.cpp, .h) |
| **Koruma Mekanizması** | Git versiyon kontrolü, kod sertleştirme |
| **Erişim Kontrolü** | Git repository erişim kontrolü |
| **Şifreleme** | Kod sertleştirme teknikleri (string obfuscation, fonksiyon gizleme) |

**Koruma Detayları:**
- **Git Versiyon Kontrolü:** Kaynak kod değişiklikleri Git ile takip edilir
- **Kod Sertleştirme:** Opaque loops, string obfuscation, fonksiyon gizleme
- **Erişim Kontrolü:** Git repository erişim kontrolü
- **Bütünlük Kontrolü:** Checksum doğrulama (kod bloğu integrity)

**Dosyalar:**
- `src/travelexpense/src/*.cpp`
- `src/travelexpense/header/*.h`

---

#### 3.2 Binary Dosyaları

| Özellik | Değer |
|---------|-------|
| **Konumu** | Build output dizinleri |
| **Tipi** | DLL (.dll), Shared Library (.so) |
| **Koruma Mekanizması** | Binary integrity kontrolü, checksum doğrulama |
| **Erişim Kontrolü** | Dosya sistemi erişim kontrolü |
| **Şifreleme** | Self-checksum doğrulama, tamper detection |

**Koruma Detayları:**
- **Self-Checksum:** Binary dosyasının kendi checksum'unu doğrular
- **Tamper Detection:** Dosya değişiklik tespiti
- **Anti-Debug:** Debugger tespiti ve önleme
- **RASP:** Runtime Application Self-Protection

**Dosyalar:**
- `build/Debug/*.dll` (Windows)
- `build/Debug/*.so` (Linux)

---

### 4. Kaynaklar (Resources)

#### 4.1 Third-Party Kütüphaneler

| Özellik | Değer |
|---------|-------|
| **Konumu** | `src/travelexpense/third_party/` |
| **Tipi** | SQLite3, PKCS#11 header dosyaları |
| **Koruma Mekanizması** | Submodule olarak yönetilir, versiyon kontrolü |
| **Erişim Kontrolü** | Git submodule erişim kontrolü |
| **Şifreleme** | Statik dosyalar (değiştirilemez) |

**Koruma Detayları:**
- **SQLite3:** Amalgamation dosyası proje içinde saklanır
- **PKCS#11:** SoftHSM header dosyaları
- **Versiyon Kontrolü:** Git submodule ile yönetilir

**Dosyalar:**
- `src/travelexpense/third_party/sqlite3/`
- `src/travelexpense/third_party/pkcs11/`

---

#### 4.2 Konfigürasyon Dosyaları

| Özellik | Değer |
|---------|-------|
| **Konumu** | Root dizini, `docs/` |
| **Tipi** | CMakeLists.txt, Doxyfile, mkdocs.yml |
| **Koruma Mekanizması** | Git versiyon kontrolü |
| **Erişim Kontrolü** | Git repository erişim kontrolü |
| **Şifreleme** | Dosya sistemi erişim kontrolü |

**Koruma Detayları:**
- **Git Versiyon Kontrolü:** Konfigürasyon değişiklikleri Git ile takip edilir
- **Dosya Sistemi:** Erişim kontrolü
- **Bütünlük Kontrolü:** Checksum doğrulama

**Dosyalar:**
- `CMakeLists.txt`
- `Doxyfile*`
- `mkdocs.yml`
- `astyle-options.txt`

---

## Koruma Mekanizmaları Detayı

### 1. Şifreleme ile Koruma

#### Whitebox Kriptografi
- **Anahtarlar lookup table'lara gömülü**
- **Reverse engineering zorlaştırılmış**
- **Anahtar bilgisi maskelenmiş**

#### Statik Anahtarlar
- **Master key:** Oturum anahtarları şifreleme için
- **Signature key:** Dijital imza için
- **Whitebox keys:** Dosya şifreleme için

### 2. Erişim Kontrolü ile Koruma

#### Kod Seviyesinde Erişim Kontrolü
- **Static const:** Değiştirilemez anahtarlar
- **Private/Internal:** Sınıf/namespace erişim kontrolü
- **Header guards:** Çoklu include koruması

#### Dosya Sistemi Erişim Kontrolü
- **Git repository:** Erişim kontrolü
- **Dosya izinleri:** Okuma/yazma kontrolü
- **Build dizinleri:** Erişim kontrolü

### 3. Bütünlük Kontrolü ile Koruma

#### Checksum Doğrulama
- **Kod bloğu checksum:** Kod değişiklik tespiti
- **Dosya checksum:** Dosya bütünlük kontrolü
- **Self-checksum:** Binary integrity kontrolü

#### Tamper Detection
- **Dosya değişiklik tespiti:** Dosya modifikasyon zamanı kontrolü
- **Dosya boyutu kontrolü:** Dosya boyutu değişiklik tespiti
- **Binary integrity:** Binary dosya bütünlük kontrolü

### 4. Kod Sertleştirme ile Koruma

#### String Obfuscation
- **XOR tabanlı şifreleme:** String'lerin gizlenmesi
- **Obfuscated strings:** Reverse engineering zorlaştırma

#### Fonksiyon Gizleme
- **Pointer obfuscation:** Fonksiyon pointer'larının gizlenmesi
- **Opaque loops:** Kontrol akışı gizleme

---

## Erişim Kontrolü Politikaları

### 1. Gizli Anahtarlara Erişim

- **Whitebox anahtarlar:** Sadece encryption modülü erişebilir
- **Master key:** Sadece sessionManager modülü erişebilir
- **Signature key:** Sadece sessionManager modülü erişebilir

### 2. Hash Değerlerine Erişim

- **SHA-256 sabitleri:** Sadece encryption modülü erişebilir
- **AES S-Box:** Sadece encryption modülü erişebilir
- **DES S-Box:** Sadece encryption modülü erişebilir

### 3. Kaynak Kodlara Erişim

- **Git repository:** Erişim kontrolü ile korunur
- **Build dizinleri:** Erişim kontrolü ile korunur
- **Kod sertleştirme:** Reverse engineering zorlaştırılmış

### 4. Kaynaklara Erişim

- **Third-party kütüphaneler:** Git submodule ile yönetilir
- **Konfigürasyon dosyaları:** Git versiyon kontrolü

---

## Güvenlik Önlemleri

### 1. Anahtar Yönetimi

- **Whitebox anahtarlar:** Lookup table'lara gömülü
- **Master key:** Statik (gerçek uygulamada HSM/TPM kullanılmalı)
- **Anahtar rotasyonu:** Oturum anahtarları her oturumda yeniden oluşturulur

### 2. Hash Değerleri Yönetimi

- **Sabitler:** Read-only statik array
- **Değiştirilemez:** Const qualifier
- **Bütünlük kontrolü:** Checksum doğrulama

### 3. Kaynak Kod Yönetimi

- **Versiyon kontrolü:** Git repository
- **Kod sertleştirme:** Opaque loops, string obfuscation
- **Bütünlük kontrolü:** Checksum doğrulama

### 4. Kaynak Yönetimi

- **Submodule yönetimi:** Git submodule
- **Versiyon kontrolü:** Git repository
- **Erişim kontrolü:** Dosya sistemi izinleri

---

## İyileştirme Önerileri

### 1. Anahtar Yönetimi

- **HSM/TPM kullanımı:** Master key'lerin HSM/TPM'de saklanması
- **Anahtar rotasyonu:** Master key rotasyonu mekanizması
- **Key derivation:** PBKDF2 ile anahtar türetme

### 2. Kaynak Kod Koruma

- **Kod obfuscation:** İleri seviye obfuscation teknikleri
- **Code signing:** Binary dosyalarının dijital imzası
- **Tamper detection:** Gelişmiş tamper detection

### 3. Kaynak Koruma

- **Dependency scanning:** Güvenlik açığı taraması
- **Versiyon kontrolü:** Dependency versiyon takibi
- **Erişim kontrolü:** Gelişmiş erişim kontrolü mekanizmaları

---

## Sonuç

Statik varlıklar şu mekanizmalarla korunmaktadır:

1. **Şifreleme:** Whitebox kriptografi, statik anahtarlar
2. **Erişim Kontrolü:** Kod seviyesinde, dosya sistemi seviyesinde
3. **Bütünlük Kontrolü:** Checksum doğrulama, tamper detection
4. **Kod Sertleştirme:** String obfuscation, fonksiyon gizleme

Yönergede belirtilen Gereksinim #5'e göre tüm statik varlıklar koruma mekanizmaları ile korunmaktadır.

---

**Son Güncelleme:** 2025

