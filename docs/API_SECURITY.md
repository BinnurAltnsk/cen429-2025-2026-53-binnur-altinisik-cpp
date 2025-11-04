# Arayüz Tanımları ve Korunması

## 1. Genel Bakış

Bu dokümantasyon, Seyahat Gideri Takibi uygulamasının tüm API arayüzlerini, erişim kontrollerini ve kimlik doğrulama mekanizmalarını açıklar. Tüm API'ler güvenlik katmanları ile korunmaktadır.

## 2. API Kategorileri

### 2.1 Kullanıcı Kimlik Doğrulama API'leri

**Namespace:** `TravelExpense::UserAuth`

#### 2.1.1 Kullanıcı Kaydı

```cpp
ErrorCode registerUser(const char* username, const char* password);
```

**Açıklama:**
- Yeni kullanıcı kaydı oluşturur
- Şifreler SHA-256 + Salt ile hash'lenir
- Güvenli bellek yönetimi kullanılır

**Güvenlik Önlemleri:**
- ✅ Şifre hash'leme: SHA-256 + Salt
- ✅ Güvenli bellek temizliği: `Security::secureCleanup`
- ✅ SQL Injection koruması: Parameterized queries
- ✅ Input validation: Kullanıcı adı ve şifre uzunluk kontrolü
- ✅ Veritabanı: SQLite3 (şifrelenmiş)

**Erişim Kontrolü:**
- Herhangi bir kimlik doğrulama gerektirmez
- Public API

**Kullanım Örneği:**
```cpp
ErrorCode result = TravelExpense::UserAuth::registerUser("kullanici", "sifre123");
if (result == ErrorCode::SUCCESS) {
    // Başarılı kayıt
}
```

#### 2.1.2 Kullanıcı Girişi

```cpp
ErrorCode loginUser(const char* username, const char* password);
```

**Açıklama:**
- Kullanıcı kimlik doğrulaması yapar
- Şifre doğrulama: Constant-time comparison
- Oturum oluşturulur

**Güvenlik Önlemleri:**
- ✅ Şifre doğrulama: Constant-time comparison
- ✅ Güvenli bellek temizliği: `Security::secureMemoryCleanup`
- ✅ SQL Injection koruması: Parameterized queries
- ✅ Oturum yönetimi: `currentUser` pointer yönetimi

**Erişim Kontrolü:**
- Herhangi bir kimlik doğrulama gerektirmez
- Public API

**Kullanım Örneği:**
```cpp
ErrorCode result = TravelExpense::UserAuth::loginUser("kullanici", "sifre123");
if (result == ErrorCode::SUCCESS) {
    // Başarılı giriş
}
```

#### 2.1.3 Misafir Modu

```cpp
ErrorCode enableGuestMode();
```

**Açıklama:**
- Misafir kullanıcı modunu etkinleştirir
- Sınırlı erişim sağlar

**Güvenlik Önlemleri:**
- ✅ Kullanıcı izolasyonu: Guest kullanıcı ID = -1
- ✅ Oturum yönetimi: `currentUser` pointer yönetimi

**Erişim Kontrolü:**
- Herhangi bir kimlik doğrulama gerektirmez
- Public API

#### 2.1.4 Kullanıcı Çıkışı

```cpp
void logoutUser();
```

**Açıklama:**
- Mevcut oturumu sonlandırır
- Güvenli bellek temizliği yapar

**Güvenlik Önlemleri:**
- ✅ Güvenli bellek temizliği: `delete currentUser`

**Erişim Kontrolü:**
- Herhangi bir kimlik doğrulama gerektirmez
- Public API

#### 2.1.5 Mevcut Kullanıcı Bilgisi

```cpp
User* getCurrentUser();
```

**Açıklama:**
- Mevcut oturumdaki kullanıcı bilgisini döndürür
- `nullptr` ise giriş yapılmamış

**Güvenlik Önlemleri:**
- ✅ Pointer güvenliği: `nullptr` kontrolü gerekir

**Erişim Kontrolü:**
- Herhangi bir kimlik doğrulama gerektirmez
- Public API

---

### 2.2 Seyahat Yönetimi API'leri

**Namespace:** `TravelExpense::TripManager`

#### Güvenlik Önlemleri (Tüm API'ler İçin)

- ✅ Kullanıcı kimlik doğrulama: `getCurrentUser()` kontrolü
- ✅ Kullanıcı bazlı erişim kontrolü: `userId` ile izolasyon
- ✅ SQL Injection koruması: Parameterized queries
- ✅ Veri şifreleme: Whitebox AES/DES (dosya depolama için)
- ✅ Güvenli bellek yönetimi

**Erişim Kontrolü:**
- ✅ Tüm API'ler için kullanıcı kimlik doğrulama gereklidir
- ✅ Kullanıcı yalnızca kendi seyahatlerine erişebilir
- ❌ Guest kullanıcılar sınırlı erişime sahiptir

---

### 2.3 Gider Yönetimi API'leri

**Namespace:** `TravelExpense::ExpenseManager`

#### Güvenlik Önlemleri (Tüm API'ler İçin)

- ✅ Kullanıcı kimlik doğrulama: `getCurrentUser()` kontrolü
- ✅ Kullanıcı bazlı erişim kontrolü: `userId` ile izolasyon
- ✅ SQL Injection koruması: Parameterized queries
- ✅ Veri şifreleme: Whitebox AES/DES (dosya depolama için)
- ✅ Güvenli bellek yönetimi
- ✅ Bütünlük kontrolü: HMAC-SHA256

**Erişim Kontrolü:**
- ✅ Tüm API'ler için kullanıcı kimlik doğrulama gereklidir
- ✅ Kullanıcı yalnızca kendi giderlerine erişebilir
- ❌ Guest kullanıcılar sınırlı erişime sahiptir

---

### 2.4 Bütçe Yönetimi API'leri

**Namespace:** `TravelExpense::BudgetManager`

#### Güvenlik Önlemleri (Tüm API'ler İçin)

- ✅ Kullanıcı kimlik doğrulama: `getCurrentUser()` kontrolü
- ✅ Kullanıcı bazlı erişim kontrolü: `userId` ile izolasyon
- ✅ SQL Injection koruması: Parameterized queries
- ✅ Veri şifreleme: Whitebox AES/DES (dosya depolama için)
- ✅ Güvenli bellek yönetimi

**Erişim Kontrolü:**
- ✅ Tüm API'ler için kullanıcı kimlik doğrulama gereklidir
- ✅ Kullanıcı yalnızca kendi bütçelerine erişebilir

---

### 2.5 Oturum Yönetimi API'leri

**Namespace:** `TravelExpense::SessionManager`

#### 2.5.1 Oturum Anahtarı Yönetimi

```cpp
ErrorCode generateSessionKey(uint8_t* sessionKey, size_t sessionKeyLen);
ErrorCode encryptSessionKey(const uint8_t* plainSessionKey, ...);
ErrorCode decryptSessionKey(const uint8_t* encryptedSessionKey, ...);
```

**Güvenlik Önlemleri:**
- ✅ Güvenli rastgele sayı üretimi: `Encryption::generateRandomBytes`
- ✅ Oturum anahtarı şifreleme: AES-256-CBC
- ✅ Master key: Uygulama içine gömülü (Whitebox)
- ✅ Güvenli bellek yönetimi

**Erişim Kontrolü:**
- ✅ Internal API (uygulama içi kullanım)

#### 2.5.2 Cihaz ve Sürüm Kontrolü

```cpp
ErrorCode getDeviceFingerprint(char* fingerprint);
ErrorCode getApplicationVersion(char* version, size_t versionLen);
ErrorCode validateDeviceAndVersion(const char* deviceFingerprint, const char* appVersion);
```

**Güvenlik Önlemleri:**
- ✅ Cihaz fingerprint: MAC adresi + SHA-256 hash
- ✅ Sürüm kontrolü: Sabit sürüm string
- ✅ Cihaz/sürüm bağlantısı: Belirli cihaz ve sürümlerde çalışma

**Erişim Kontrolü:**
- ✅ Internal API (uygulama içi kullanım)

#### 2.5.3 Gizli Veri (Confidential Payload)

```cpp
ErrorCode encryptPayload(const void* plaintext, size_t plaintextLen, ...);
ErrorCode decryptPayload(const void* ciphertext, size_t ciphertextLen, ...);
```

**Güvenlik Önlemleri:**
- ✅ Oturum anahtarı ile şifreleme: AES-256-CBC
- ✅ Güvenli bellek yönetimi

**Erişim Kontrolü:**
- ✅ Internal API (uygulama içi kullanım)

#### 2.5.4 Bütünlük Kontrolü ve Kimlik Doğrulama

```cpp
ErrorCode calculateHMAC(const void* data, size_t dataLen, ...);
ErrorCode verifyHMAC(const void* data, size_t dataLen, ...);
```

**Güvenlik Önlemleri:**
- ✅ HMAC-SHA256: Veri bütünlüğü kontrolü
- ✅ Constant-time comparison: Zaman saldırısı koruması

**Erişim Kontrolü:**
- ✅ Internal API (uygulama içi kullanım)

#### 2.5.5 Sunucu Doğrulama Kodu (Dijital İmza)

```cpp
ErrorCode signData(const void* data, size_t dataLen, char* signature);
ErrorCode verifySignature(const void* data, size_t dataLen, const char* signature);
```

**Güvenlik Önlemleri:**
- ✅ Dijital imza: RSA veya ECDSA (implementasyon bağımlı)
- ✅ Güvenli imza doğrulama

**Erişim Kontrolü:**
- ✅ Internal API (uygulama içi kullanım)

---

### 2.6 Varlık Yönetimi API'leri

**Namespace:** `TravelExpense::AssetManager`

#### Güvenlik Önlemleri (Tüm API'ler İçin)

- ✅ Kullanıcı kimlik doğrulama: `getCurrentUser()` kontrolü
- ✅ Varlık koruma şemaları: Gizlilik, Bütünlük, Kimlik Doğrulama
- ✅ SQL Injection koruması: Parameterized queries
- ✅ Varlık şifreleme: Asset bazlı şifreleme
- ✅ Güvenli bellek yönetimi

**Erişim Kontrolü:**
- ✅ Tüm API'ler için kullanıcı kimlik doğrulama gereklidir
- ✅ Kullanıcı yalnızca kendi varlıklarına erişebilir

---

### 2.7 Şifreleme API'leri

**Namespace:** `TravelExpense::Encryption`

#### Güvenlik Önlemleri

- ✅ Whitebox AES: Anahtarlar kod içine gömülü
- ✅ Whitebox DES: Anahtarlar kod içine gömülü
- ✅ SHA-256: Hash algoritması
- ✅ AES-256-CBC: Şifreleme algoritması
- ✅ PBKDF2: Anahtar türetme
- ✅ HMAC-SHA256: Mesaj kimlik doğrulama kodu
- ✅ Güvenli rastgele sayı üretimi
- ✅ Constant-time comparison

**Erişim Kontrolü:**
- ✅ Internal API (uygulama içi kullanım)

---

### 2.8 RASP API'leri

**Namespace:** `TravelExpense::RASP`

#### Güvenlik Önlemleri

- ✅ Checksum doğrulama: Dosya ve self-binary
- ✅ Anti-debug: Debugger tespiti
- ✅ Tamper detection: Dosya değişiklik tespiti
- ✅ Hook detection: API hook tespiti
- ✅ Control flow counter: Kontrol akışı bütünlüğü

**Erişim Kontrolü:**
- ✅ Internal API (uygulama içi kullanım)

---

## 3. Erişim Kontrolü Mekanizmaları

### 3.1 Kimlik Doğrulama

**Yöntem:**
- Kullanıcı adı ve şifre ile kimlik doğrulama
- SHA-256 + Salt ile şifre hash'leme
- Constant-time şifre karşılaştırması

**Oturum Yönetimi:**
- `currentUser` pointer ile oturum takibi
- Oturum sonlandırma: `logoutUser()`

### 3.2 Yetkilendirme

**Kullanıcı Bazlı İzolasyon:**
- Her kullanıcı yalnızca kendi verilerine erişebilir
- Veritabanı sorgularında `userId` filtresi
- Guest kullanıcılar sınırlı erişime sahiptir

**Rol Bazlı Erişim:**
- Şu an için tek rol: Kullanıcı
- Gelecekte admin rolü eklenebilir

### 3.3 Input Validation

**Kontrol Edilenler:**
- Kullanıcı adı uzunluğu
- Şifre uzunluğu
- NULL pointer kontrolü
- Boş string kontrolü

**Validasyon Mekanizmaları:**
- Parameterized SQL queries (SQL Injection koruması)
- String uzunluk kontrolleri
- Type checking

---

## 4. Güvenlik Katmanları

### 4.1 Şifreleme Katmanı

**Veri Şifreleme:**
- Whitebox AES-256-CBC (dosya depolama)
- Whitebox DES (dosya depolama)
- AES-256-CBC (oturum anahtarları)

**Hash Algoritmaları:**
- SHA-256 (şifre hash'leme)
- HMAC-SHA256 (bütünlük kontrolü)

### 4.2 Bellek Güvenliği Katmanı

**Güvenli Bellek Yönetimi:**
- `Security::secureCleanup`: Hassas verilerin güvenli silinmesi
- `Security::secureMemoryCleanup`: Struct'ların güvenli temizlenmesi
- Güvenli bellek tahsisi ve serbest bırakma

### 4.3 RASP Katmanı

**Runtime Self-Protection:**
- Checksum doğrulama
- Anti-debug mekanizmaları
- Tamper detection
- Hook detection
- Control flow integrity

---

## 5. API Kullanım Senaryoları

### 5.1 Kullanıcı Kaydı ve Girişi

```cpp
// 1. Kullanıcı kaydı
ErrorCode result = TravelExpense::UserAuth::registerUser("kullanici", "sifre123");
if (result != ErrorCode::SUCCESS) {
    // Hata işleme
}

// 2. Kullanıcı girişi
result = TravelExpense::UserAuth::loginUser("kullanici", "sifre123");
if (result == ErrorCode::SUCCESS) {
    // Başarılı giriş
    User* currentUser = TravelExpense::UserAuth::getCurrentUser();
    // Kullanıcı işlemleri...
}

// 3. Çıkış
TravelExpense::UserAuth::logoutUser();
```

### 5.2 Oturum Yönetimi ile Veri İletimi

```cpp
// 1. Oturum anahtarı oluştur
uint8_t sessionKey[32];
ErrorCode result = TravelExpense::SessionManager::generateSessionKey(sessionKey, 32);

// 2. Veriyi şifrele
const char* plaintext = "Gizli veri";
uint8_t ciphertext[256];
size_t ciphertextLen = sizeof(ciphertext);
result = TravelExpense::SessionManager::encryptPayload(plaintext, strlen(plaintext),
                                                       sessionKey, ciphertext, ciphertextLen);

// 3. HMAC hesapla
char hmac[65];
result = TravelExpense::SessionManager::calculateHMAC(ciphertext, ciphertextLen,
                                                        sessionKey, hmac);

// 4. HMAC doğrula
result = TravelExpense::SessionManager::verifyHMAC(ciphertext, ciphertextLen,
                                                     sessionKey, hmac);
```

---

## 6. Güvenlik En İyi Uygulamaları

### 6.1 API Kullanımı

✅ **Yapılması Gerekenler:**
- Tüm API çağrılarında hata kontrolü yapın
- Hassas veriler için güvenli bellek temizliği kullanın
- Oturum yönetimini doğru kullanın
- Input validation yapın

❌ **Yapılmaması Gerekenler:**
- Şifreleri plaintext olarak saklamayın
- Güvenlik kontrollerini atlamayın
- Hassas verileri log dosyalarına yazmayın
- Oturum bilgilerini açık metin olarak iletmeyin

### 6.2 Erişim Kontrolü

✅ **Yapılması Gerekenler:**
- Her API çağrısından önce kullanıcı kimlik doğrulaması yapın
- Kullanıcı bazlı veri izolasyonu sağlayın
- Guest kullanıcılar için erişim sınırlamaları uygulayın

❌ **Yapılmaması Gerekenler:**
- Kullanıcı kimlik doğrulaması olmadan veri erişimi sağlamayın
- Bir kullanıcının başka bir kullanıcının verilerine erişmesine izin vermeyin

---

## 7. Güvenlik Testi

### 7.1 Test Senaryoları

1. **Kimlik Doğrulama Testleri:**
   - Geçersiz kullanıcı adı/şifre ile giriş denemesi
   - SQL Injection saldırı denemeleri
   - Brute force saldırı denemeleri

2. **Erişim Kontrolü Testleri:**
   - Başka kullanıcının verilerine erişim denemesi
   - Oturum olmadan API erişim denemesi
   - Guest kullanıcı ile sınırlı erişim testleri

3. **Bütünlük Testleri:**
   - HMAC doğrulama testleri
   - Dijital imza doğrulama testleri
   - Veri manipülasyon denemeleri

---

## 8. Uyumluluk ve Sertifikasyon

### 8.1 OWASP Top 10 (2021) Uyumluluğu

Bu API güvenlik modeli aşağıdaki OWASP Top 10 güvenlik açıklarını ele alır:

- ✅ **A01: Broken Access Control**: Kullanıcı bazlı erişim kontrolü
- ✅ **A02: Cryptographic Failures**: Güçlü şifreleme algoritmaları
- ✅ **A03: Injection**: Parameterized queries
- ✅ **A04: Insecure Design**: Güvenli mimari tasarım
- ✅ **A07: Identification and Authentication Failures**: Güvenli kimlik doğrulama
- ✅ **A08: Software and Data Integrity Failures**: HMAC ve dijital imzalar

### 8.2 ETSI/EMV/GSMA Uyumluluğu

- ✅ Şifreleme: Whitebox AES/DES (EMV uyumlu)
- ✅ Anahtar yönetimi: Güvenli anahtar türetme (PBKDF2)
- ✅ Bütünlük: HMAC-SHA256 (GSMA uyumlu)
- ✅ Kimlik doğrulama: Dijital imzalar (ETSI uyumlu)

---

## 9. Referanslar

- [Architecture Documentation](architecture.md)
- [Security Documentation](security.md)
- [Asset Management Documentation](ASSET_MANAGEMENT.md)
- [Static Assets Protection](STATIC_ASSETS_PROTECTION.md)

---

**Son Güncelleme:** 2025  
**Hazırlayan:** Binnur Altınışık

