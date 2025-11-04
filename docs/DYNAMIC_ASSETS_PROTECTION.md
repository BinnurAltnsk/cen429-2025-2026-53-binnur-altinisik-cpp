# Dinamik Varlıkların Korunması

## 1. Genel Bakış

Bu dokümantasyon, Seyahat Gideri Takibi uygulamasında dinamik varlıkların korunması için kullanılan cihaz ve uygulama fingerprinting mekanizmalarını açıklar. Dinamik varlıklar, çalışma zamanında oluşturulan ve cihaz/uygulama bağlamına bağlı olan verilerdir.

## 2. Dinamik Varlık Tipleri

### 2.1 Cihaz Fingerprint

**Tanım:**
- Cihazın benzersiz özelliklerini temsil eden bir tanımlayıcı
- MAC adresi, sistem bilgileri ve donanım özelliklerini içerir

**Kullanım Senaryoları:**
- Cihaz bazlı kimlik doğrulama
- Cihaz bazlı şifreleme anahtarları
- Lisans kontrolü (belirli cihazlarda çalışma)

**Koruma Mekanizmaları:**
- SHA-256 hash ile fingerprint oluşturma
- Güvenli depolama (şifrelenmiş formatta)

### 2.2 Uygulama Fingerprint

**Tanım:**
- Uygulamanın benzersiz özelliklerini temsil eden bir tanımlayıcı
- Executable dosyasının hash'ini, sürüm bilgisini ve build zamanını içerir

**Kullanım Senaryoları:**
- Uygulama bütünlüğü kontrolü
- Uygulama bazlı şifreleme anahtarları
- Versiyon kontrolü

**Koruma Mekanizmaları:**
- Self-checksum ile fingerprint oluşturma
- SHA-256 hash ile fingerprint oluşturma

### 2.3 Birleşik Fingerprint

**Tanım:**
- Cihaz ve uygulama fingerprint'lerinin birleştirilmiş hali
- Cihaz-uygulama bağlantısı için kullanılır

**Kullanım Senaryoları:**
- Cihaz-uygulama bağlantısı kontrolü
- Birleşik şifreleme anahtarları
- Güvenli oturum yönetimi

## 3. Fingerprinting Modülü

### 3.1 Cihaz Fingerprinting

#### `generateDeviceFingerprint`

**Fonksiyon:**
```cpp
ErrorCode generateDeviceFingerprint(char* fingerprint);
```

**Açıklama:**
- Cihazın benzersiz özelliklerini kullanarak fingerprint oluşturur
- Windows: MAC adresi + sistem bilgileri + bilgisayar adı
- Linux: MAC adresi + sistem bilgileri + hostname

**Fingerprint Bileşenleri:**
- MAC adresi (birincil)
- Sistem bilgileri (işlemci tipi, işlemci sayısı)
- Bilgisayar adı/hostname

**Güvenlik:**
- SHA-256 hash ile fingerprint oluşturma
- Fallback mekanizması (timestamp + process ID)

**Örnek Kullanım:**
```cpp
char fingerprint[65];
ErrorCode result = TravelExpense::Fingerprinting::generateDeviceFingerprint(fingerprint);
if (result == ErrorCode::SUCCESS) {
    // Fingerprint başarıyla oluşturuldu
    printf("Device fingerprint: %s\n", fingerprint);
}
```

#### `verifyDeviceFingerprint`

**Fonksiyon:**
```cpp
ErrorCode verifyDeviceFingerprint(const char* expectedFingerprint);
```

**Açıklama:**
- Beklenen fingerprint ile mevcut cihaz fingerprint'ini karşılaştırır
- Case-insensitive karşılaştırma yapar

**Örnek Kullanım:**
```cpp
const char* expectedFp = "abc123...";
ErrorCode result = TravelExpense::Fingerprinting::verifyDeviceFingerprint(expectedFp);
if (result == ErrorCode::SUCCESS) {
    // Cihaz geçerli
}
```

### 3.2 Uygulama Fingerprinting

#### `generateApplicationFingerprint`

**Fonksiyon:**
```cpp
ErrorCode generateApplicationFingerprint(char* fingerprint);
```

**Açıklama:**
- Uygulamanın benzersiz özelliklerini kullanarak fingerprint oluşturur
- Executable dosyasının checksum'unu, sürüm bilgisini ve build zamanını içerir

**Fingerprint Bileşenleri:**
- Self-checksum (executable dosyasının hash'i)
- Uygulama sürümü
- Build zamanı bilgisi
- Uygulama adı

**Güvenlik:**
- SHA-256 hash ile fingerprint oluşturma
- RASP modülünden self-checksum kullanımı

**Örnek Kullanım:**
```cpp
char fingerprint[65];
ErrorCode result = TravelExpense::Fingerprinting::generateApplicationFingerprint(fingerprint);
if (result == ErrorCode::SUCCESS) {
    // Uygulama fingerprint başarıyla oluşturuldu
    printf("Application fingerprint: %s\n", fingerprint);
}
```

#### `verifyApplicationFingerprint`

**Fonksiyon:**
```cpp
ErrorCode verifyApplicationFingerprint(const char* expectedFingerprint);
```

**Açıklama:**
- Beklenen fingerprint ile mevcut uygulama fingerprint'ini karşılaştırır
- Uygulama bütünlüğü kontrolü için kullanılır

**Örnek Kullanım:**
```cpp
const char* expectedFp = "def456...";
ErrorCode result = TravelExpense::Fingerprinting::verifyApplicationFingerprint(expectedFp);
if (result == ErrorCode::SUCCESS) {
    // Uygulama geçerli
}
```

### 3.3 Birleşik Fingerprinting

#### `generateCombinedFingerprint`

**Fonksiyon:**
```cpp
ErrorCode generateCombinedFingerprint(char* fingerprint);
```

**Açıklama:**
- Cihaz ve uygulama fingerprint'lerini birleştirerek tek bir fingerprint oluşturur
- Cihaz-uygulama bağlantısı için kullanılır

**Örnek Kullanım:**
```cpp
char fingerprint[65];
ErrorCode result = TravelExpense::Fingerprinting::generateCombinedFingerprint(fingerprint);
if (result == ErrorCode::SUCCESS) {
    // Birleşik fingerprint başarıyla oluşturuldu
    printf("Combined fingerprint: %s\n", fingerprint);
}
```

## 4. Dinamik Anahtar Yönetimi

### 4.1 Dinamik Anahtar Oluşturma

#### `generateDynamicKey`

**Fonksiyon:**
```cpp
ErrorCode generateDynamicKey(const char* fingerprint, uint8_t* key, size_t keyLen);
```

**Açıklama:**
- Fingerprint'i kullanarak PBKDF2 ile dinamik bir anahtar oluşturur
- Salt olarak fingerprint'in ilk 16 byte'ı kullanılır
- 10000 iterasyon ile PBKDF2 uygulanır

**Güvenlik:**
- PBKDF2 key derivation (10000 iterasyon)
- Fingerprint bazlı salt
- 32 byte (256-bit) anahtar

**Örnek Kullanım:**
```cpp
const char* fingerprint = "abc123...";
uint8_t key[32];
ErrorCode result = TravelExpense::Fingerprinting::generateDynamicKey(fingerprint, key, 32);
if (result == ErrorCode::SUCCESS) {
    // Dinamik anahtar başarıyla oluşturuldu
}
```

#### `generateDeviceBasedKey`

**Fonksiyon:**
```cpp
ErrorCode generateDeviceBasedKey(uint8_t* key, size_t keyLen);
```

**Açıklama:**
- Cihaz fingerprint'ini kullanarak dinamik bir anahtar oluşturur
- Cihaz bazlı şifreleme için kullanılır

**Örnek Kullanım:**
```cpp
uint8_t key[32];
ErrorCode result = TravelExpense::Fingerprinting::generateDeviceBasedKey(key, 32);
if (result == ErrorCode::SUCCESS) {
    // Cihaz bazlı anahtar başarıyla oluşturuldu
}
```

#### `generateApplicationBasedKey`

**Fonksiyon:**
```cpp
ErrorCode generateApplicationBasedKey(uint8_t* key, size_t keyLen);
```

**Açıklama:**
- Uygulama fingerprint'ini kullanarak dinamik bir anahtar oluşturur
- Uygulama bazlı şifreleme için kullanılır

**Örnek Kullanım:**
```cpp
uint8_t key[32];
ErrorCode result = TravelExpense::Fingerprinting::generateApplicationBasedKey(key, 32);
if (result == ErrorCode::SUCCESS) {
    // Uygulama bazlı anahtar başarıyla oluşturuldu
}
```

#### `generateCombinedKey`

**Fonksiyon:**
```cpp
ErrorCode generateCombinedKey(uint8_t* key, size_t keyLen);
```

**Açıklama:**
- Birleşik fingerprint'i kullanarak dinamik bir anahtar oluşturur
- Cihaz-uygulama bağlantısı için şifreleme anahtarı olarak kullanılır

**Örnek Kullanım:**
```cpp
uint8_t key[32];
ErrorCode result = TravelExpense::Fingerprinting::generateCombinedKey(key, 32);
if (result == ErrorCode::SUCCESS) {
    // Birleşik anahtar başarıyla oluşturuldu
}
```

## 5. Güvenlik Önlemleri

### 5.1 Fingerprint Güvenliği

**Hash Algoritması:**
- SHA-256 (256-bit çıktı)

**Fingerprint Formatı:**
- 64 karakter hex string
- Null-terminated string

**Güvenlik Özellikleri:**
- Deterministic: Aynı cihaz/uygulama için aynı fingerprint
- Collision-resistant: SHA-256 hash özellikleri
- Tamper-resistant: Değişiklikler fingerprint'i değiştirir

### 5.2 Anahtar Güvenliği

**Key Derivation:**
- PBKDF2 (10000 iterasyon)
- Fingerprint bazlı salt
- 32 byte (256-bit) anahtar

**Güvenlik Özellikleri:**
- Deterministic: Aynı fingerprint için aynı anahtar
- Salt-based: Her fingerprint için benzersiz salt
- Iterative: Zaman alıcı saldırılar için koruma

### 5.3 Bellek Güvenliği

**Güvenli Bellek Yönetimi:**
- Hassas veriler için güvenli bellek temizliği
- Stack ve heap bellek yönetimi
- Pointer güvenliği

## 6. Kullanım Senaryoları

### 6.1 Cihaz Bazlı Kimlik Doğrulama

```cpp
// 1. Cihaz fingerprint'ini al
char deviceFp[65];
ErrorCode result = TravelExpense::Fingerprinting::generateDeviceFingerprint(deviceFp);

// 2. Beklenen fingerprint ile karşılaştır
const char* expectedFp = "..."; // Veritabanından veya yapılandırmadan
result = TravelExpense::Fingerprinting::verifyDeviceFingerprint(expectedFp);

if (result == ErrorCode::SUCCESS) {
    // Cihaz geçerli
} else {
    // Cihaz geçersiz - erişim reddedildi
}
```

### 6.2 Dinamik Anahtar ile Şifreleme

```cpp
// 1. Cihaz bazlı anahtar oluştur
uint8_t key[32];
ErrorCode result = TravelExpense::Fingerprinting::generateDeviceBasedKey(key, 32);

// 2. Anahtarı kullanarak veriyi şifrele
const char* plaintext = "Gizli veri";
uint8_t iv[16];
// IV oluştur
// ...

uint8_t ciphertext[256];
size_t ciphertextLen = sizeof(ciphertext);
if (TravelExpense::Encryption::encryptAES256(plaintext, strlen(plaintext), key, iv,
                                               ciphertext, ciphertextLen)) {
    // Şifreleme başarılı
}
```

### 6.3 Birleşik Fingerprint ile Oturum Yönetimi

```cpp
// 1. Birleşik fingerprint oluştur
char combinedFp[65];
ErrorCode result = TravelExpense::Fingerprinting::generateCombinedFingerprint(combinedFp);

// 2. Birleşik anahtar oluştur
uint8_t sessionKey[32];
result = TravelExpense::Fingerprinting::generateCombinedKey(sessionKey, 32);

// 3. Oturum anahtarını şifrele
uint8_t encryptedSessionKey[48];
size_t encryptedLen = sizeof(encryptedSessionKey);
result = TravelExpense::SessionManager::encryptSessionKey(sessionKey, encryptedSessionKey,
                                                            encryptedLen);
```

## 7. Entegrasyon

### 7.1 SessionManager Entegrasyonu

Fingerprinting modülü, `SessionManager` modülü ile entegre edilmiştir:
- `SessionManager::getDeviceFingerprint` - Cihaz fingerprint'i için
- `SessionManager::getApplicationVersion` - Uygulama sürümü için

### 7.2 RASP Entegrasyonu

Fingerprinting modülü, `RASP` modülünden yararlanır:
- `RASP::calculateSelfChecksum` - Uygulama fingerprint'i için

### 7.3 Encryption Entegrasyonu

Fingerprinting modülü, `Encryption` modülünü kullanır:
- `Encryption::sha256Hash` - Fingerprint oluşturma için
- `Encryption::pbkdf2` - Dinamik anahtar türetme için

## 8. Referanslar

- [Session Manager Documentation](API_SECURITY.md#25-oturum-yönetimi-apileri)
- [RASP Documentation](security.md#5-rasp-runtime-application-self-protection)
- [Encryption Documentation](security.md#3-şifreleme-katmanı)
- [API Security Documentation](API_SECURITY.md)

---

**Son Güncelleme:** 2025  
**Hazırlayan:** Binnur Altınışık

