# Varlık Yönetimi Dokümantasyonu

**Tarih:** 2025  
**Proje:** Seyahat Gideri Takibi  
**Hazırlayan:** Binnur Altınışık

---

## Genel Bakış

Bu dokümantasyon, projedeki tüm varlıkların yönetimi ve korunması için gereken bilgileri içerir. Yönergede belirtilen Gereksinim #7'ye göre her varlık için aşağıdaki özellikler belirlenmiş ve dokümante edilmiştir.

---

## Varlık Özellikleri

Her varlık için aşağıdaki özellikler takip edilir:

1. **Adı** - Varlığın benzersiz adı
2. **Açıklaması** - Varlığın amacı ve kullanımı
3. **Konumu** - Varlığın saklandığı yer (veritabanı, tablo, sütun)
4. **Kaynağı** - Varlığın kaynağı
5. **Boyutu** - Varlığın boyutu (byte)
6. **Oluşturulma Zamanı** - Varlığın oluşturulduğu zaman
7. **Silinme Zamanı** - Varlığın silindiği zaman (0 = silinmemiş)
8. **Varsayılan Değeri** - Varlığın varsayılan değeri
9. **Varlık Koruma Şeması** - Koruma şeması (Gizlilik, Bütünlük, Kimlik Doğrulama)

---

## Varlık Listesi

### 1. Kullanıcı Verileri (User Data)

| Özellik | Değer |
|---------|-------|
| **Adı** | User Data |
| **Açıklaması** | Kullanıcı kimlik bilgileri ve profil verileri |
| **Konumu** | Veritabanı: `travelexpense.db`, Tablo: `users`, Sütunlar: `user_id`, `username`, `password_hash`, `salt`, `is_guest`, `created_at`, `last_login` |
| **Kaynağı** | Kullanıcı kayıt ve giriş işlemleri |
| **Boyutu** | ~150 byte (kullanıcı başına) |
| **Oluşturulma Zamanı** | Kayıt zamanı |
| **Silinme Zamanı** | Soft delete (silinme zamanı kaydedilir) |
| **Varsayılan Değeri** | `is_guest = 0`, `created_at = 0`, `last_login = 0` |
| **Koruma Şeması** | Gizlilik (✅), Bütünlük (✅), Kimlik Doğrulama (✅) |
| **Şifreleme** | `password_hash`: SHA-256 + Salt ile hash'lenmiş |

---

### 2. Seyahat Verileri (Trip Data)

| Özellik | Değer |
|---------|-------|
| **Adı** | Trip Data |
| **Açıklaması** | Kullanıcı seyahat planları ve detayları |
| **Konumu** | Veritabanı: `travelexpense.db`, Tablo: `trips`, Sütunlar: `trip_id`, `user_id`, `destination`, `start_date`, `end_date`, `accommodation`, `transportation`, `budget`, `total_expenses`, `created_at`, `updated_at` |
| **Kaynağı** | Kullanıcı seyahat oluşturma ve düzenleme işlemleri |
| **Boyutu** | ~500 byte (seyahat başına) |
| **Oluşturulma Zamanı** | Seyahat oluşturma zamanı |
| **Silinme Zamanı** | Soft delete (silinme zamanı kaydedilir) |
| **Varsayılan Değeri** | `budget = 0.0`, `total_expenses = 0.0`, `created_at = 0`, `updated_at = 0` |
| **Koruma Şeması** | Gizlilik (✅), Bütünlük (✅), Kimlik Doğrulama (✅) |
| **Şifreleme** | Dosya depolama: Whitebox AES ve Whitebox DES |

---

### 3. Gider Verileri (Expense Data)

| Özellik | Değer |
|---------|-------|
| **Adı** | Expense Data |
| **Açıklaması** | Seyahat giderleri ve harcamalar |
| **Konumu** | Veritabanı: `travelexpense.db`, Tablo: `expenses`, Sütunlar: `expense_id`, `trip_id`, `category`, `amount`, `currency`, `date`, `payment_method`, `description`, `created_at` |
| **Kaynağı** | Kullanıcı gider kayıt ve düzenleme işlemleri |
| **Boyutu** | ~300 byte (gider başına) |
| **Oluşturulma Zamanı** | Gider kayıt zamanı |
| **Silinme Zamanı** | Soft delete (silinme zamanı kaydedilir) |
| **Varsayılan Değeri** | `category = 0` (ACCOMMODATION), `currency = "TRY"`, `created_at = 0` |
| **Koruma Şeması** | Gizlilik (✅), Bütünlük (✅), Kimlik Doğrulama (✅) |
| **Şifreleme** | Dosya depolama: Whitebox AES ve Whitebox DES |

---

### 4. Bütçe Verileri (Budget Data)

| Özellik | Değer |
|---------|-------|
| **Adı** | Budget Data |
| **Açıklaması** | Seyahat bütçe planları ve takibi |
| **Konumu** | Veritabanı: `travelexpense.db`, Tablo: `budgets`, Sütunlar: `budget_id`, `trip_id`, `total_budget`, `spent_amount`, `category_budget_*`, `category_spent_*`, `created_at`, `updated_at` |
| **Kaynağı** | Kullanıcı bütçe belirleme ve güncelleme işlemleri |
| **Boyutu** | ~100 byte (bütçe başına) |
| **Oluşturulma Zamanı** | Bütçe oluşturma zamanı |
| **Silinme Zamanı** | Soft delete (silinme zamanı kaydedilir) |
| **Varsayılan Değeri** | `total_budget = 0.0`, `spent_amount = 0.0`, tüm kategoriler = `0.0`, `created_at = 0`, `updated_at = 0` |
| **Koruma Şeması** | Gizlilik (✅), Bütünlük (✅), Kimlik Doğrulama (✅) |
| **Şifreleme** | Dosya depolama: Whitebox AES ve Whitebox DES |

---

### 5. Oturum Verileri (Session Data)

| Özellik | Değer |
|---------|-------|
| **Adı** | Session Data |
| **Açıklaması** | Oturum anahtarları ve oturum bilgileri |
| **Konumu** | Bellek (runtime), Şifrelenmiş dosya depolama (opsiyonel) |
| **Kaynağı** | Oturum anahtarı oluşturma işlemleri |
| **Boyutu** | 32 byte (oturum anahtarı) + metadata |
| **Oluşturulma Zamanı** | Oturum başlatma zamanı |
| **Silinme Zamanı** | Oturum sonlandırma zamanı |
| **Varsayılan Değeri** | Yok (her oturum için rastgele oluşturulur) |
| **Koruma Şeması** | Gizlilik (✅), Bütünlük (✅), Kimlik Doğrulama (✅) |
| **Şifreleme** | Oturum anahtarları: AES-256-CBC ile şifrelenmiş |

---

### 6. Şifreleme Anahtarları (Encryption Keys)

| Özellik | Değer |
|---------|-------|
| **Adı** | Encryption Keys |
| **Açıklaması** | Whitebox AES ve Whitebox DES anahtarları |
| **Konumu** | Kod içine gömülü (embedded keys), SoftHSM (opsiyonel) |
| **Kaynağı** | Kod derleme zamanı (whitebox keys), SoftHSM token (opsiyonel) |
| **Boyutu** | 32 byte (Whitebox AES), 8 byte (Whitebox DES) |
| **Oluşturulma Zamanı** | Kod derleme zamanı (whitebox), Token oluşturma zamanı (SoftHSM) |
| **Silinme Zamanı** | Anahtar rotasyonu veya uygulama kapatma |
| **Varsayılan Değeri** | Embedded keys (kod içinde tanımlı) |
| **Koruma Şeması** | Gizlilik (✅), Bütünlük (✅), Kimlik Doğrulama (✅) |
| **Şifreleme** | Whitebox kriptografi (anahtarlar lookup table'lara gömülü) |

---

### 7. Yapılandırma Verileri (Config Data)

| Özellik | Değer |
|---------|-------|
| **Adı** | Config Data |
| **Açıklaması** | Uygulama yapılandırma ayarları |
| **Konumu** | Veritabanı: `travelexpense.db`, Tablo: `assets`, Konfigürasyon dosyaları |
| **Kaynağı** | Uygulama başlatma ve yapılandırma işlemleri |
| **Boyutu** | Değişken |
| **Oluşturulma Zamanı** | Uygulama kurulumu zamanı |
| **Silinme Zamanı** | Uygulama kaldırma zamanı |
| **Varsayılan Değeri** | Uygulama varsayılan ayarları |
| **Koruma Şeması** | Gizlilik (✅), Bütünlük (✅), Kimlik Doğrulama (✅) |
| **Şifreleme** | Hassas ayarlar: Whitebox AES veya Whitebox DES |

---

## Varlık Koruma Şemaları

### Gizlilik (Confidentiality)

- **Uygulama:** Veriler şifrelenerek saklanır
- **Teknikler:**
  - Whitebox AES (dosya depolama)
  - Whitebox DES (dosya depolama)
  - AES-256-CBC (oturum anahtarları)
  - SHA-256 + Salt (şifre hash'leme)

### Bütünlük (Integrity)

- **Uygulama:** Verilerin değiştirilmediği doğrulanır
- **Teknikler:**
  - HMAC-SHA256 (veri bütünlük kontrolü)
  - Checksum doğrulama (kod ve dosya integrity)
  - Dijital imza doğrulama (sunucu doğrulama)

### Kimlik Doğrulama (Authentication)

- **Uygulama:** Verilere erişim kimlik doğrulama ile kontrol edilir
- **Teknikler:**
  - Kullanıcı kimlik doğrulama (PBKDF2)
  - Oturum yönetimi (şifrelenmiş oturum anahtarları)
  - Cihaz ve sürüm kontrolü (fingerprinting)

---

## Varlık Yönetimi API

### Varlık Kayıt

```cpp
Asset asset;
asset.assetType = AssetType::USER_DATA;
strncpy(asset.name, "User Data", sizeof(asset.name) - 1);
strncpy(asset.description, "Kullanıcı kimlik bilgileri", sizeof(asset.description) - 1);
strncpy(asset.location, "travelexpense.db.users", sizeof(asset.location) - 1);
strncpy(asset.source, "UserAuth::registerUser", sizeof(asset.source) - 1);
asset.size = 150;
asset.protectionScheme = 0x07; // Gizlilik + Bütünlük + Kimlik Doğrulama
asset.isEncrypted = true;

int32_t assetId = 0;
ErrorCode result = AssetManager::registerAsset(asset, assetId);
```

### Varlık Sorgulama

```cpp
Asset asset;
ErrorCode result = AssetManager::getAsset(assetId, asset);
```

### Varlık Listeleme

```cpp
std::vector<Asset> assets;
ErrorCode result = AssetManager::listAssets(assets);
```

### Koruma Şeması Doğrulama

```cpp
uint8_t requiredScheme = 0x07; // Gizlilik + Bütünlük + Kimlik Doğrulama
bool isValid = AssetManager::verifyProtectionScheme(assetId, requiredScheme);
```

---

## Varlık Koruması Detayları

### 1. Kullanıcı Verileri Koruma

- **Şifre Hash'leme:** SHA-256 + Salt
- **Veritabanı:** SQLite3
- **Erişim Kontrolü:** Kullanıcı kimlik doğrulama gereklidir
- **Bütünlük:** HMAC-SHA256 ile doğrulanır

### 2. Seyahat ve Gider Verileri Koruma

- **Dosya Şifreleme:** Whitebox AES ve Whitebox DES (multiple encryption)
- **Veritabanı:** SQLite3 (şifrelenmiş)
- **Erişim Kontrolü:** Kullanıcı bazlı erişim kontrolü
- **Bütünlük:** HMAC-SHA256 ile doğrulanır

### 3. Oturum Verileri Koruma

- **Oturum Anahtarı Şifreleme:** AES-256-CBC
- **Depolama:** Bellek (güvenli silme ile)
- **Erişim Kontrolü:** Oturum yönetimi
- **Bütünlük:** HMAC-SHA256 ile doğrulanır

### 4. Şifreleme Anahtarları Koruma

- **Whitebox Kriptografi:** Anahtarlar kod içine gömülü
- **SoftHSM:** PKCS#11 standardı (opsiyonel)
- **Erişim Kontrolü:** Kod seviyesinde koruma
- **Bütünlük:** Checksum doğrulama

---

## Varlık Yaşam Döngüsü

1. **Oluşturma:** Varlık kayıt işlemi (`registerAsset`)
2. **Güncelleme:** Varlık bilgileri güncelleme (`updateAsset`)
3. **Koruma:** Koruma şeması uygulama ve doğrulama
4. **Silme:** Soft delete (`deleteAsset`)
5. **Arşivleme:** Silinmiş varlıklar arşivlenir (deleted_at != 0)

---

## Güvenlik Gereksinimleri

### Gizlilik Gereksinimleri

- Tüm hassas veriler şifrelenmelidir
- Şifreler hash'lenmiş olmalıdır (SHA-256 + Salt)
- Oturum anahtarları şifrelenerek iletilmelidir

### Bütünlük Gereksinimleri

- Veri bütünlük kontrolü (HMAC-SHA256)
- Checksum doğrulama (kod ve dosya integrity)
- Dijital imza doğrulama (sunucu doğrulama)

### Kimlik Doğrulama Gereksinimleri

- Kullanıcı kimlik doğrulama (PBKDF2)
- Oturum yönetimi
- Cihaz ve sürüm kontrolü

---

**Son Güncelleme:** 2025

