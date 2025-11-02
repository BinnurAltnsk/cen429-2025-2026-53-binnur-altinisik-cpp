# Güvenlik Dokümantasyonu

## 1. Genel Bakış

Bu dokümantasyon, Seyahat Gideri Takibi projesinin güvenlik modelini, uygulanan güvenlik önlemlerini ve güvenlik sertifikasyon sürecini açıklar.

## 2. Güvenlik Mimarisi

### 2.1 Güvenlik Katmanları

```
┌─────────────────────────────────────────┐
│        Kullanıcı Arayüzü Katmanı         │
├─────────────────────────────────────────┤
│         Uygulama Katmanı                  │
│  (travelexpenseapp - Menü Sistemi)       │
├─────────────────────────────────────────┤
│         İş Mantığı Katmanı                │
│  (travelexpense - Core Functions)       │
├─────────────────────────────────────────┤
│         Güvenlik Katmanı                  │
│  - Şifreleme (Whitebox AES)              │
│  - Güvenli Bellek Yönetimi               │
│  - Checksum Doğrulama                    │
│  - Anti-Debug Mekanizmaları              │
├─────────────────────────────────────────┤
│         Veri Erişim Katmanı               │
│  - Binary Dosya I/O                      │
│  - Şifrelenmiş Depolama                 │
└─────────────────────────────────────────┘
```

### 2.2 Güvenlik Politikaları

#### 2.2.1 Veri Gizliliği Politikası
- Tüm hassas veriler şifrelenmiş formatta saklanır
- Kullanıcı şifreleri hashlenmiş olarak saklanır (salt + hash)
- Oturum bilgileri şifrelenmiş formatta iletilir

#### 2.2.2 Veri Bütünlüğü Politikası
- Checksum doğrulama ile dosya bütünlüğü kontrol edilir
- HMAC ile veri bütünlüğü doğrulanır
- Binary dosyalarda imza kontrolü yapılır

#### 2.2.3 Kimlik Doğrulama Politikası
- Kullanıcı kimlik doğrulama güçlü hash algoritmaları kullanır
- Oturum yönetimi güvenli token'lar ile yapılır
- Çoklu faktörlü kimlik doğrulama (opsiyonel)

## 3. Veri Güvenliği

### 3.1 Kullanımda Veri Güvenliği

#### 3.1.1 Bellekte Şifreleme
- **Teknoloji:** Whitebox AES
- **Uygulama:** Hassas veriler bellekte şifrelenmiş formatta tutulur
- **Temizleme:** Güvenli bellek silme fonksiyonları kullanılır

```cpp
// Örnek kullanım
void secureMemoryCleanup(void* ptr, size_t size) {
    // Bellek içeriğini güvenli şekilde sil
    memset_s(ptr, size, 0, size);
    // Bellek bölgesini rastgele verilerle doldur
    // ...
}
```

#### 3.1.2 Güvenli Bellek Silme
- **Fonksiyon:** `secureMemoryCleanup()`
- **Özellikler:**
  - Çoklu geçişli silme
  - Rastgele veri ile doldurma
  - Bellek kısımlarının tamamen silinmesi

### 3.2 İletim Sırasındaki Veri Güvenliği

#### 3.2.1 Oturum Anahtarı Yönetimi
- **Şifreleme:** AES-256
- **Key Exchange:** Güvenli anahtar değişimi protokolü
- **Key Storage:** Şifrelenmiş formatta saklama

#### 3.2.2 Veri Şifreleme (Taşıma Sırasında)
- **Algoritma:** AES-GCM (Authenticated Encryption)
- **Key Length:** 256-bit
- **IV Management:** Rastgele IV kullanımı

### 3.3 Depolamada Veri Güvenliği

#### 3.3.1 Whitebox Kriptografi
- **Algoritma:** Whitebox AES
- **Uygulama Alanı:** Dosya depolama
- **Özellikler:**
  - Static key embedded
  - Kod içinde şifreleme mantığı
  - Reverse engineering zorluğu

#### 3.3.2 Dosya Şifreleme
- **Dosya Formatı:** Binary + Encryption Header
- **Şifreleme Modu:** AES-CBC
- **Key Derivation:** PBKDF2

#### 3.3.3 Veri Bütünlüğü Kontrolü
- **Checksum Algoritması:** SHA-256
- **HMAC:** HMAC-SHA256
- **Doğrulama:** Dosya açılışında otomatik kontrol

## 4. Kod Sertleştirme

### 4.1 Kontrol Akışı Gizleme

#### 4.1.1 Opaque Loops
- **Amaç:** Kod analizini zorlaştırmak
- **Uygulama:** Karmaşık döngü yapıları
- **Örnek:**
```cpp
// Basit döngü yerine
for (int i = 0; i < 10; i++) { ... }

// Opaque loop kullanımı
int opaque_var = generateOpaqueValue();
for (int i = 0; i < opaque_var; i++) {
    if (complexCondition(i, opaque_var)) { ... }
}
```

### 4.2 String ve Fonksiyon Gizleme

#### 4.2.1 String Şifreleme
- **Yöntem:** Runtime'da string'leri şifreleyip saklama
- **Decryption:** Kullanım anında decrypt etme
- **Örnek:**
```cpp
// String gizleme
const char* hiddenString = decryptAtRuntime(encryptedString);
```

#### 4.2.2 Fonksiyon İsimlerinin Gizlenmesi
- **Yöntem:** Obfuscated function names
- **Uygulama:** Makro kullanımı ile fonksiyon isimlerini değiştirme

### 4.3 Bellek Koruması

#### 4.3.1 Güvenli Bellek Yönetimi
- **SafeStack:** Clang SafeStack kullanımı (opsiyonel)
- **Stack Canary:** Stack overflow koruması
- **ASLR:** Address Space Layout Randomization

### 4.4 Diğer Sertleştirme Teknikleri

#### 4.4.1 Opaque Boolean Değişkenleri
```cpp
// Normal boolean yerine
bool isAuthenticated = true;

// Opaque boolean
int auth_flag = generateOpaqueFlag();
if (auth_flag == AUTHENTICATED_OPAQUE_VALUE) { ... }
```

#### 4.4.2 Sahte İşlemler (Dummy Operations)
- **Amaç:** Kod analizini zorlaştırmak
- **Uygulama:** Kullanılmayan kod blokları ekleme

## 5. RASP (Runtime Application Self-Protection)

### 5.1 Checksum Doğrulama

#### 5.1.1 Kod Bloğu Checksum
- **Algoritma:** SHA-256
- **Doğrulama Zamanı:** Runtime'da periyodik kontrol
- **Tepki:** Checksum uyuşmazlığında uygulama kapanışı

```cpp
bool verifyCodeChecksum() {
    uint8_t currentChecksum[32];
    calculateChecksum(currentChecksum);
    return memcmp(currentChecksum, expectedChecksum, 32) == 0;
}
```

### 5.2 Anti-Debug Mekanizmaları

#### 5.2.1 Debug Tespiti
- **Windows API:** `IsDebuggerPresent()`, `CheckRemoteDebuggerPresent()`
- **Linux API:** `ptrace()` kontrolü
- **Tepki:** Debug tespit edildiğinde uygulama kapanışı

#### 5.2.2 Process Hiding
- **Yöntem:** Debugger process'lerini tespit etme
- **Uygulama:** Sistem process listesi kontrolü

### 5.3 Tamper Tespiti

#### 5.3.1 Dosya Değişiklik Tespiti
- **Yöntem:** Dosya hash'i kontrolü
- **Zamanlama:** Uygulama başlangıcında ve periyodik olarak
- **Tepki:** Değişiklik tespit edildiğinde uygulama kapanışı

#### 5.3.2 Binary Integrity Check
- **Yöntem:** Binary dosya checksum kontrolü
- **Uygulama:** Kendi binary'sini kontrol etme

### 5.4 Güvensiz Cihaz Tespiti

#### 5.4.1 Emulator Tespiti
- **Yöntem:** Sistem özelliklerini kontrol etme
- **Kontrol Noktaları:**
  - CPU bilgileri
  - Bellek bilgileri
  - Donanım özellikleri

#### 5.4.2 Root/Jailbreak Tespiti
- **Windows:** Administrator privileges kontrolü
- **Linux:** Root access kontrolü

### 5.5 HOOK Saldırı Tespiti

#### 5.5.1 API Hooking Tespiti
- **Yöntem:** Kritik fonksiyonların adres kontrolü
- **Uygulama:** Import Address Table (IAT) kontrolü

## 6. Güvenlik Sertifikasyonu

### 6.1 Sertifikasyon Standartları

#### 6.1.1 ETSI TS 102 484
- **Kapsam:** Yazılım güvenliği
- **Uyumluluk:** Temel seviye

#### 6.1.2 OWASP Mobile Top 10
- **Kapsam:** Mobil güvenlik
- **Uyumluluk:** Masaüstü uygulamasına uyarlanmış

### 6.2 Penetrasyon Testi Planı

#### 6.2.1 Test Kapsamı
- Binary analizi
- Bellek analizi
- Dosya sistem analizi
- Network trafiği analizi (varsa)

#### 6.2.2 Test Senaryoları
1. Reverse engineering denemeleri
2. Debug ile kod analizi
3. Dosya manipülasyonu
4. Bellek dump analizi

## 7. Güvenlik İyileştirme Önerileri

### 7.1 Kısa Vadeli İyileştirmeler (Vize)
- [x] Temel şifreleme mekanizması
- [x] Güvenli bellek silme
- [x] Checksum doğrulama
- [x] Anti-debug mekanizmaları
- [ ] Kod obfuscation

### 7.2 Uzun Vadeli İyileştirmeler (Final)
- [ ] SSL/TLS entegrasyonu
- [ ] Sertifika pinning
- [ ] İleri düzey RASP teknikleri
- [ ] Penetrasyon testi sonuçlarına göre iyileştirmeler

## 8. Güvenlik Olay Müdahale Planı

### 8.1 Güvenlik Olayı Senaryoları

#### Senaryo 1: Güvenlik Açığı Keşfi
- **Tepki:** Hemen yama geliştirme
- **Süreç:** GitHub Security Advisory oluşturma

#### Senaryo 2: Veri İhlali
- **Tepki:** Kullanıcıları bilgilendirme
- **Süreç:** Şifre değişikliği önerisi

### 8.2 Güvenlik Güncellemeleri

- **Otomatik Güncelleme:** Yok (konsol uygulaması)
- **Manuel Güncelleme:** Kullanıcı bilgilendirme ile

---

**Hazırlayan:** [İsim]  
**Tarih:** [Tarih]  
**Versiyon:** 1.0  
**Son Güncelleme:** [Tarih]
