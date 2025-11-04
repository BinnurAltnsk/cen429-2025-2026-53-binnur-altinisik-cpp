# SSL/TLS ve Certificate Pinning

## 1. Genel Bakış

Bu dokümantasyon, Seyahat Gideri Takibi uygulamasında SSL/TLS bağlantıları ve Certificate Pinning mekanizmalarını açıklar. TLS modülü, güvenli ağ iletişimi için gerekli fonksiyonları sağlar.

**Önemli Not:** Bu konsol uygulaması olduğu için network iletişimi sınırlıdır. TLS modülü, gelecekte HTTP/HTTPS iletişimi için kullanılabilir. Şu anda temel bir implementasyon sağlanmıştır; gerçek bir TLS implementasyonu için OpenSSL veya benzeri bir kütüphane entegre edilebilir.

## 2. TLS Modülü Özellikleri

### 2.1 SSL/TLS Bağlantı Yönetimi

- ✅ TLS bağlamı başlatma ve temizleme
- ✅ TLS bağlantısı kurma ve kapatma
- ✅ TLS üzerinden veri gönderme ve alma
- ✅ Güvenli bellek yönetimi

### 2.2 Certificate Pinning

- ✅ Sertifika pin kaydı
- ✅ Sertifika fingerprint doğrulama
- ✅ Public key hash doğrulama
- ✅ Otomatik pin kontrolü

### 2.3 Mutual Authentication

- ✅ İstemci sertifikası yükleme
- ✅ CA sertifika yolu ayarlama
- ✅ Sunucu sertifikası doğrulama

## 3. TLS API'leri

### 3.1 SSL/TLS Bağlantı Yönetimi

#### `initializeTLSContext`

**Fonksiyon:**
```cpp
ErrorCode initializeTLSContext(TLSContext* ctx);
```

**Açıklama:**
- TLS bağlamını başlatır
- Gerçek implementasyonda OpenSSL SSL_CTX oluşturulur

**Örnek Kullanım:**
```cpp
TLSContext ctx;
ErrorCode result = TravelExpense::TLS::initializeTLSContext(&ctx);
if (result == ErrorCode::SUCCESS) {
    // TLS bağlamı başarıyla başlatıldı
}
```

#### `connectTLS`

**Fonksiyon:**
```cpp
ErrorCode connectTLS(TLSContext* ctx, const char* hostname, uint16_t port);
```

**Açıklama:**
- TLS bağlantısı kurar
- Otomatik olarak certificate pinning kontrolü yapar

**Örnek Kullanım:**
```cpp
ErrorCode result = TravelExpense::TLS::connectTLS(&ctx, "example.com", 443);
if (result == ErrorCode::SUCCESS) {
    // TLS bağlantısı başarıyla kuruldu
}
```

#### `sendTLS` ve `receiveTLS`

**Fonksiyonlar:**
```cpp
ErrorCode sendTLS(TLSContext* ctx, const void* data, size_t dataLen, size_t& bytesSent);
ErrorCode receiveTLS(TLSContext* ctx, void* buffer, size_t bufferLen, size_t& bytesReceived);
```

**Açıklama:**
- TLS üzerinden veri gönderme ve alma

**Örnek Kullanım:**
```cpp
// Veri gönder
const char* message = "Hello, TLS!";
size_t bytesSent = 0;
ErrorCode result = TravelExpense::TLS::sendTLS(&ctx, message, strlen(message), bytesSent);

// Veri al
char buffer[1024];
size_t bytesReceived = 0;
result = TravelExpense::TLS::receiveTLS(&ctx, buffer, sizeof(buffer), bytesReceived);
```

### 3.2 Certificate Pinning

#### `registerCertificatePin`

**Fonksiyon:**
```cpp
ErrorCode registerCertificatePin(const CertificatePin* pin);
```

**Açıklama:**
- Certificate pin bilgisini kaydeder
- Hostname, fingerprint ve public key hash içerir

**CertificatePin Yapısı:**
```cpp
struct CertificatePin {
    char hostname[256];       // Hostname
    char fingerprint[65];      // SHA-256 fingerprint (64 karakter hex)
    char publicKeyHash[65];    // Public key hash (64 karakter hex)
    bool pinCertificate;      // Sertifika pin'i etkin mi?
    bool pinPublicKey;         // Public key pin'i etkin mi?
};
```

**Örnek Kullanım:**
```cpp
CertificatePin pin;
strncpy(pin.hostname, "example.com", sizeof(pin.hostname) - 1);
strncpy(pin.fingerprint, "abc123...", sizeof(pin.fingerprint) - 1);  // 64 karakter hex
strncpy(pin.publicKeyHash, "def456...", sizeof(pin.publicKeyHash) - 1);  // 64 karakter hex
pin.pinCertificate = true;
pin.pinPublicKey = true;

ErrorCode result = TravelExpense::TLS::registerCertificatePin(&pin);
if (result == ErrorCode::SUCCESS) {
    // Certificate pin başarıyla kaydedildi
}
```

#### `verifyCertificatePin`

**Fonksiyon:**
```cpp
ErrorCode verifyCertificatePin(TLSContext* ctx, const char* hostname);
```

**Açıklama:**
- Mevcut sertifika ile kayıtlı pin'i karşılaştırır
- `connectTLS` içinde otomatik olarak çağrılır

**Örnek Kullanım:**
```cpp
ErrorCode result = TravelExpense::TLS::verifyCertificatePin(&ctx, "example.com");
if (result == ErrorCode::SUCCESS) {
    // Certificate pin doğrulaması başarılı
} else {
    // Certificate pin doğrulaması başarısız - bağlantı reddedildi
}
```

#### `getCertificateFingerprint`

**Fonksiyon:**
```cpp
ErrorCode getCertificateFingerprint(TLSContext* ctx, char* fingerprint);
```

**Açıklama:**
- Sunucu sertifikasının SHA-256 fingerprint'ini alır
- 64 karakter hex string olarak döner

**Örnek Kullanım:**
```cpp
char fingerprint[65];
ErrorCode result = TravelExpense::TLS::getCertificateFingerprint(&ctx, fingerprint);
if (result == ErrorCode::SUCCESS) {
    // Fingerprint başarıyla alındı
    printf("Certificate fingerprint: %s\n", fingerprint);
}
```

#### `getCertificatePublicKeyHash`

**Fonksiyon:**
```cpp
ErrorCode getCertificatePublicKeyHash(TLSContext* ctx, char* publicKeyHash);
```

**Açıklama:**
- Sunucu sertifikasının public key hash'ini alır
- 64 karakter hex string olarak döner

**Örnek Kullanım:**
```cpp
char publicKeyHash[65];
ErrorCode result = TravelExpense::TLS::getCertificatePublicKeyHash(&ctx, publicKeyHash);
if (result == ErrorCode::SUCCESS) {
    // Public key hash başarıyla alındı
    printf("Public key hash: %s\n", publicKeyHash);
}
```

### 3.3 Mutual Authentication

#### `loadClientCertificate`

**Fonksiyon:**
```cpp
ErrorCode loadClientCertificate(TLSContext* ctx, const char* certPath,
                                  const char* keyPath, const char* keyPassword);
```

**Açıklama:**
- Mutual authentication için istemci sertifikası ve private key'i yükler
- PEM formatında dosya bekler

**Örnek Kullanım:**
```cpp
ErrorCode result = TravelExpense::TLS::loadClientCertificate(&ctx,
                                                                "client.crt",
                                                                "client.key",
                                                                "password123");  // nullptr ise şifre yok
if (result == ErrorCode::SUCCESS) {
    // İstemci sertifikası başarıyla yüklendi
}
```

#### `setCAPath`

**Fonksiyon:**
```cpp
ErrorCode setCAPath(TLSContext* ctx, const char* caPath);
```

**Açıklama:**
- Sunucu sertifikasını doğrulamak için CA sertifika yolu ayarlar
- Dosya yolu veya dizin yolu olabilir

**Örnek Kullanım:**
```cpp
ErrorCode result = TravelExpense::TLS::setCAPath(&ctx, "/etc/ssl/certs/ca-certificates.crt");
// veya dizin yolu:
// ErrorCode result = TravelExpense::TLS::setCAPath(&ctx, "/etc/ssl/certs/");
if (result == ErrorCode::SUCCESS) {
    // CA yolu başarıyla ayarlandı
}
```

#### `verifyServerCertificate`

**Fonksiyon:**
```cpp
ErrorCode verifyServerCertificate(TLSContext* ctx);
```

**Açıklama:**
- Sunucu sertifikasının geçerliliğini kontrol eder
- CA sertifikaları ile doğrulama yapar

**Örnek Kullanım:**
```cpp
ErrorCode result = TravelExpense::TLS::verifyServerCertificate(&ctx);
if (result == ErrorCode::SUCCESS) {
    // Sunucu sertifikası geçerli
} else {
    // Sunucu sertifikası geçersiz
}
```

### 3.4 Yardımcı Fonksiyonlar

#### `calculateCertificateFingerprint`

**Fonksiyon:**
```cpp
ErrorCode calculateCertificateFingerprint(const char* certPath, char* fingerprint);
```

**Açıklama:**
- Sertifika dosyasından SHA-256 fingerprint hesaplar
- PEM veya DER formatında dosya kabul eder

**Örnek Kullanım:**
```cpp
char fingerprint[65];
ErrorCode result = TravelExpense::TLS::calculateCertificateFingerprint("server.crt", fingerprint);
if (result == ErrorCode::SUCCESS) {
    // Fingerprint başarıyla hesaplandı
    printf("Certificate fingerprint: %s\n", fingerprint);
}
```

## 4. Güvenlik Önlemleri

### 4.1 Certificate Pinning

**Avantajlar:**
- ✅ Man-in-the-Middle (MITM) saldırılarına karşı koruma
- ✅ Sertifika yetkilisi (CA) kompromisi durumunda bile koruma
- ✅ Yalnızca güvenilir sertifikalarla bağlantı kurma

**Nasıl Çalışır:**
1. Sunucu sertifikasının fingerprint'i veya public key hash'i önceden kaydedilir
2. TLS bağlantısı kurulurken sunucu sertifikası kontrol edilir
3. Kayıtlı pin ile mevcut sertifika karşılaştırılır
4. Uyuşmazlık durumunda bağlantı reddedilir

### 4.2 Mutual Authentication

**Avantajlar:**
- ✅ Hem istemci hem sunucu kimlik doğrular
- ✅ Çift yönlü güvenlik sağlar
- ✅ Yetkisiz erişimlere karşı koruma

**Nasıl Çalışır:**
1. İstemci sertifikası ve private key yüklenir
2. TLS handshake sırasında istemci sertifikası sunucuya gönderilir
3. Sunucu istemci sertifikasını doğrular
4. Sunucu sertifikası da istemci tarafından doğrulanır

### 4.3 Güvenli Bağlantı

**Özellikler:**
- ✅ TLS 1.2+ protokol desteği (gerçek implementasyonda)
- ✅ Güçlü şifreleme algoritmaları
- ✅ Güvenli bellek yönetimi
- ✅ Certificate validation

## 5. Kullanım Senaryoları

### 5.1 Temel TLS Bağlantısı

```cpp
// 1. TLS bağlamını başlat
TLSContext ctx;
ErrorCode result = TravelExpense::TLS::initializeTLSContext(&ctx);

// 2. CA sertifika yolu ayarla
result = TravelExpense::TLS::setCAPath(&ctx, "/etc/ssl/certs/ca-certificates.crt");

// 3. TLS bağlantısı kur
result = TravelExpense::TLS::connectTLS(&ctx, "example.com", 443);

// 4. Veri gönder
const char* message = "Hello, TLS!";
size_t bytesSent = 0;
result = TravelExpense::TLS::sendTLS(&ctx, message, strlen(message), bytesSent);

// 5. Bağlantıyı kapat
TravelExpense::TLS::disconnectTLS(&ctx);
TravelExpense::TLS::cleanupTLSContext(&ctx);
```

### 5.2 Certificate Pinning ile Bağlantı

```cpp
// 1. TLS bağlamını başlat
TLSContext ctx;
ErrorCode result = TravelExpense::TLS::initializeTLSContext(&ctx);

// 2. Certificate pin kaydet
CertificatePin pin;
strncpy(pin.hostname, "example.com", sizeof(pin.hostname) - 1);
strncpy(pin.fingerprint, "abc123...", sizeof(pin.fingerprint) - 1);  // Gerçek fingerprint
pin.pinCertificate = true;
pin.pinPublicKey = false;  // Sadece sertifika pin'i
result = TravelExpense::TLS::registerCertificatePin(&pin);

// 3. TLS bağlantısı kur (otomatik pin kontrolü yapılır)
result = TravelExpense::TLS::connectTLS(&ctx, "example.com", 443);
if (result == ErrorCode::SUCCESS) {
    // Bağlantı başarılı - pin doğrulaması geçti
} else {
    // Bağlantı başarısız - pin doğrulaması geçmedi
}

// 4. Temizlik
TravelExpense::TLS::cleanupTLSContext(&ctx);
```

### 5.3 Mutual Authentication

```cpp
// 1. TLS bağlamını başlat
TLSContext ctx;
ErrorCode result = TravelExpense::TLS::initializeTLSContext(&ctx);

// 2. CA sertifika yolu ayarla
result = TravelExpense::TLS::setCAPath(&ctx, "/etc/ssl/certs/ca-certificates.crt");

// 3. İstemci sertifikası yükle
result = TravelExpense::TLS::loadClientCertificate(&ctx,
                                                    "client.crt",
                                                    "client.key",
                                                    nullptr);  // Şifresiz key

// 4. TLS bağlantısı kur
result = TravelExpense::TLS::connectTLS(&ctx, "example.com", 443);

// 5. Sunucu sertifikası doğrula
result = TravelExpense::TLS::verifyServerCertificate(&ctx);
if (result == ErrorCode::SUCCESS) {
    // Sunucu sertifikası geçerli
}

// 6. Temizlik
TravelExpense::TLS::cleanupTLSContext(&ctx);
```

## 6. Gerçek Implementasyon Notları

### 6.1 OpenSSL Entegrasyonu

**Şu Anki Durum:**
- ✅ Temel TLS API yapısı hazır
- ✅ Certificate pinning mekanizması implementasyonu var
- ⏳ OpenSSL entegrasyonu eksik (stub implementasyon)

**Gerçek Implementasyon İçin:**
1. OpenSSL kütüphanesini projeye ekle
2. `tls.cpp` dosyasındaki stub implementasyonları OpenSSL çağrıları ile değiştir
3. SSL_CTX_new(), SSL_new(), SSL_connect() gibi fonksiyonları kullan
4. X509 sertifika işlemleri için OpenSSL X509 API'lerini kullan

### 6.2 Bağımlılıklar

**Opsiyonel Bağımlılıklar:**
- OpenSSL (TLS/SSL kütüphanesi)
- libcurl (HTTP/HTTPS client - opsiyonel)

**Not:** Konsol uygulaması olduğu için network iletişimi sınırlıdır. TLS modülü, gelecekte HTTP/HTTPS iletişimi için kullanılabilir.

## 7. Güvenlik En İyi Uygulamaları

### 7.1 Certificate Pinning

✅ **Yapılması Gerekenler:**
- Production ortamında certificate pin'lerini güvenli şekilde saklayın
- Sertifika yenileme durumlarında pin'leri güncelleyin
- Birden fazla pin (backup) kullanmayı düşünün
- Public key pin'i tercih edin (sertifika yenilemelerinde daha esnek)

❌ **Yapılmaması Gerekenler:**
- Pin'leri plaintext olarak saklamayın
- Sadece fingerprint'e güvenmeyin (public key hash de kullanın)
- Pin doğrulamasını atlamayın

### 7.2 TLS Bağlantıları

✅ **Yapılması Gerekenler:**
- TLS 1.2+ protokol kullanın
- Güçlü şifreleme algoritmaları seçin
- CA sertifikalarını düzenli güncelleyin
- Bağlantı sonrası temizlik yapın

❌ **Yapılmaması Gerekenler:**
- TLS 1.0/1.1 kullanmayın (eski ve güvensiz)
- Zayıf şifreleme algoritmaları kullanmayın
- Certificate validation'ı devre dışı bırakmayın

## 8. Referanslar

- [API Security Documentation](API_SECURITY.md)
- [Security Documentation](security.md)
- [OpenSSL Documentation](https://www.openssl.org/docs/)
- [Certificate Pinning Best Practices](https://owasp.org/www-community/controls/Certificate_and_Public_Key_Pinning)

---

**Son Güncelleme:** 2025  
**Hazırlayan:** Binnur Altınışık  
**Not:** Bu implementasyon konsol uygulaması için temel yapı sağlar. Gerçek TLS implementasyonu için OpenSSL entegrasyonu gereklidir.

