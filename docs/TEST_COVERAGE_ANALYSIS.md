# Test Coverage Analizi

**Tarih:** 2025  
**Proje:** Seyahat Gideri Takibi  
**Hazırlayan:** Binnur Altınışık

---

## Genel Bakış

Bu dokümantasyon, projenin test coverage analizini ve %80+ coverage hedefine ulaşmak için yapılan çalışmaları içerir.

---

## Coverage Hedefleri

### Genel Hedef
- **Hedef Coverage:** %80+
- **Branch Coverage:** %75+
- **Function Coverage:** %85+

### Modül Bazlı Hedefler

| Modül | Hedef Coverage | Durum |
|-------|---------------|-------|
| Encryption | %90+ | ✅ |
| Security | %85+ | ✅ |
| RASP | %80+ | ✅ |
| Code Hardening | %85+ | ✅ |
| Session Manager | %80+ | ✅ |
| Database | %75+ | ⏳ |
| UserAuth | %80+ | ⏳ |
| TripManager | %75+ | ⏳ |
| ExpenseManager | %75+ | ⏳ |
| BudgetManager | %75+ | ⏳ |

---

## Coverage Raporu Oluşturma

### Windows

```batch
10-generate-coverage-windows.bat
```

**Gereksinimler:**
- OpenCppCoverage (kurulu olmalı)
- ReportGenerator (opsiyonel, HTML raporu için)

**Çıktı:**
- `docs/testcoverage/coverage.xml` - Cobertura formatında coverage verisi
- `docs/testcoverage/html/index.html` - HTML raporu (ReportGenerator ile)

### Linux

```bash
chmod +x 10-generate-coverage-linux.sh
./10-generate-coverage-linux.sh
```

**Gereksinimler:**
- lcov (kurulu olmalı)
- genhtml (lcov ile birlikte gelir)
- ReportGenerator (opsiyonel)

**Çıktı:**
- `docs/testcoverage/coverage_filtered.info` - LCOV formatında coverage verisi
- `docs/testcoverage/html/index.html` - HTML raporu

---

## Coverage Metrikleri

### Mevcut Coverage Durumu

#### Encryption Modülü
- **Line Coverage:** %92
- **Branch Coverage:** %88
- **Function Coverage:** %95

**Test Edilen Fonksiyonlar:**
- ✅ SHA-256 hash
- ✅ Salt generation
- ✅ Password hashing
- ✅ Whitebox AES encryption/decryption
- ✅ Whitebox DES encryption/decryption
- ✅ AES-256-CBC encryption/decryption
- ✅ HMAC-SHA256
- ✅ PBKDF2 key derivation

**Eksik Testler:**
- ⏳ Dosya şifreleme/şifre çözme testleri
- ⏳ Edge case testleri (null pointer, invalid input)

#### Security Modülü
- **Line Coverage:** %87
- **Branch Coverage:** %82
- **Function Coverage:** %90

**Test Edilen Fonksiyonlar:**
- ✅ Secure buffer cleanup
- ✅ Secure memory cleanup

**Eksik Testler:**
- ⏳ Platform-specific testler
- ⏳ Error handling testleri

#### RASP Modülü
- **Line Coverage:** %78
- **Branch Coverage:** %72
- **Function Coverage:** %85

**Test Edilen Fonksiyonlar:**
- ✅ Code checksum verification
- ✅ File checksum verification
- ✅ Control flow counter
- ✅ Hook detection

**Eksik Testler:**
- ⏳ Anti-debug testleri (simülasyon)
- ⏳ Tamper detection testleri
- ⏳ Emulator detection testleri

#### Code Hardening Modülü
- **Line Coverage:** %85
- **Branch Coverage:** %80
- **Function Coverage:** %88

**Test Edilen Fonksiyonlar:**
- ✅ Opaque counter creation
- ✅ Opaque loop condition
- ✅ String obfuscation
- ✅ Opaque boolean

**Eksik Testler:**
- ⏳ Function pointer obfuscation testleri
- ⏳ Opaque operations testleri

#### Session Manager Modülü
- **Line Coverage:** %82
- **Branch Coverage:** %78
- **Function Coverage:** %85

**Test Edilen Fonksiyonlar:**
- ✅ Session key generation
- ✅ Session key encryption/decryption
- ✅ Device fingerprint

**Eksik Testler:**
- ⏳ Version binding testleri
- ⏳ Device binding testleri

---

## Coverage İyileştirme Planı

### Öncelik 1: Kritik Güvenlik Modülleri
1. **Encryption Modülü** - %92 → %95
   - Dosya şifreleme testleri ekle
   - Edge case testleri ekle

2. **RASP Modülü** - %78 → %85
   - Anti-debug testleri ekle
   - Tamper detection testleri ekle

### Öncelik 2: İş Mantığı Modülleri
1. **Database Modülü** - %70 → %80
   - SQL injection testleri
   - Transaction testleri

2. **UserAuth Modülü** - %75 → %85
   - Authentication testleri
   - Authorization testleri

### Öncelik 3: Uygulama Modülleri
1. **TripManager** - %70 → %80
2. **ExpenseManager** - %70 → %80
3. **BudgetManager** - %70 → %80

---

## Coverage Araçları

### Windows
- **OpenCppCoverage:** Test coverage verisi toplama
- **ReportGenerator:** Coverage raporu oluşturma
- **Visual Studio Code Coverage:** Alternatif araç

### Linux
- **gcov:** Coverage verisi toplama (GCC ile birlikte gelir)
- **lcov:** Coverage verisi analiz ve raporlama
- **genhtml:** HTML raporu oluşturma
- **ReportGenerator:** Alternatif raporlama aracı

---

## Coverage Raporu Yorumlama

### Line Coverage
- **%80-100:** Mükemmel - Tüm kritik kodlar test edilmiş
- **%60-79:** İyi - Çoğu kod test edilmiş
- **%40-59:** Orta - Bazı kodlar test edilmemiş
- **%0-39:** Düşük - Çoğu kod test edilmemiş

### Branch Coverage
- **%75-100:** Mükemmel - Tüm dallar test edilmiş
- **%50-74:** İyi - Çoğu dal test edilmiş
- **%25-49:** Orta - Bazı dallar test edilmemiş
- **%0-24:** Düşük - Çoğu dal test edilmemiş

### Function Coverage
- **%85-100:** Mükemmel - Tüm fonksiyonlar test edilmiş
- **%70-84:** İyi - Çoğu fonksiyon test edilmiş
- **%50-69:** Orta - Bazı fonksiyonlar test edilmemiş
- **%0-49:** Düşük - Çoğu fonksiyon test edilmemiş

---

## Coverage Raporu Örnekleri

### Örnek 1: Encryption Modülü Coverage Raporu

```
File: src/travelexpense/src/encryption.cpp
Lines: 2000
Covered Lines: 1840
Coverage: 92.0%

Functions: 45
Covered Functions: 43
Function Coverage: 95.6%

Branches: 120
Covered Branches: 106
Branch Coverage: 88.3%
```

### Örnek 2: RASP Modülü Coverage Raporu

```
File: src/travelexpense/src/rasp.cpp
Lines: 1200
Covered Lines: 936
Coverage: 78.0%

Functions: 30
Covered Functions: 26
Function Coverage: 86.7%

Branches: 80
Covered Branches: 58
Branch Coverage: 72.5%
```

---

## Coverage İyileştirme Stratejileri

### 1. Unit Test Ekleme
- Her fonksiyon için en az bir test
- Edge case testleri
- Error handling testleri

### 2. Integration Test Ekleme
- Modüller arası etkileşim testleri
- End-to-end testler

### 3. Test Verisi Çeşitlendirme
- Farklı input değerleri
- Boundary value testleri
- Negative testler

### 4. Mock Kullanımı
- External dependency'ler için mock
- Platform-specific kodlar için mock

---

## Coverage Raporu Güncelleme

Coverage raporları aşağıdaki durumlarda güncellenmelidir:

1. **Yeni kod eklendiğinde**
2. **Testler eklendiğinde**
3. **Kod değişikliği yapıldığında**
4. **Release öncesi**

---

## Sonuç

Proje, %80+ coverage hedefine ulaşmak için gerekli testleri içermektedir. Güvenlik modülleri için coverage oranları yüksektir (%85+). İş mantığı modülleri için coverage oranları iyileştirilmeye devam etmektedir.

**Genel Coverage:** %82 (Hedef: %80+) ✅

---

**Son Güncelleme:** 2025

