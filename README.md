# Seyahat Gideri Takibi Projesi (Travel Expense Tracking)

## Genel Bakış

Bu proje, kullanıcıların seyahat planlarını yapmasına, giderlerini kaydetmesine, bütçelerini yönetmesine ve özet raporlar oluşturmasına olanak sağlayan güvenli bir C++ konsol uygulamasıdır. Proje, güvenlik standartlarına uygun şekilde geliştirilmiştir ve sertifikasyon sürecinden geçecek şekilde tasarlanmıştır.

## Proje Özellikleri

### Temel Özellikler

- **Kullanıcı Kimlik Doğrulama:** Kullanıcı hesapları, misafir modu desteği
- **Seyahat Planlama:** Seyahat ayrıntılarını ve güzergahı düzenleme
- **Gider Kaydı:** Seyahat masraflarını kategorize ederek kaydetme
- **Bütçe Yönetimi:** Seyahat bütçelerini belirleme ve takip etme
- **Özet Rapor:** Seyahat masraflarını ve deneyimlerini derleyen raporlar

### Güvenlik Özellikleri

- **Veri Şifreleme:** Whitebox AES kullanarak hassas verilerin şifrelenmesi
- **Güvenli Bellek Yönetimi:** Hassas verilerin kullanım sonrası güvenli şekilde silinmesi
- **Kod Sertleştirme:** Opaque loops, string/fonksiyon gizleme
- **RASP Teknikleri:** Checksum doğrulama, anti-debug mekanizmaları, tamper tespiti
- **Binary Koruma:** İkili uygulama koruma özellikleri

## Gereksinimler

- **CMake:** >= 3.12
- **C++ Standard:** >= 11
- **SQLite3:** Veritabanı için (kurulum için `docs/SQLITE_SETUP.md` dosyasına bakın)
- **GoogleTest:** Birim testleri için
- **Visual Studio Community Edition:** Windows için
- **Ninja:** WSL/Linux için
- **Whitebox Kriptografi:** Veri şifreleme için

### SQLite3 Kurulumu

**Hızlı Kurulum (Önerilen):**

1. https://www.sqlite.org/download.html adresine gidin
2. "Amalgamation" bölümünden `sqlite-amalgamation-XXX.zip` dosyasını indirin
3. Zip içinden `sqlite3.c` ve `sqlite3.h` dosyalarını çıkarın
4. Bu dosyaları şu klasöre kopyalayın:
   ```
   src/travelexpense/third_party/sqlite3/
   ```

Detaylı kurulum talimatları için: [`docs/SQLITE_SETUP.md`](docs/SQLITE_SETUP.md)

## Proje Yapısı

```
cen429-2025-2026-53-binnur-altinisik-cpp/
├── src/
│   ├── travelexpense/              # Kütüphane (İş Mantığı)
│   │   ├── header/
│   │   │   └── travelexpense.h
│   │   └── src/
│   │       └── travelexpense.cpp
│   │
│   ├── travelexpenseapp/            # Konsol Uygulaması (Menü)
│   │   ├── header/
│   │   │   └── travelexpenseapp.h
│   │   └── src/
│   │       └── travelexpenseapp.cpp
│   │
│   └── tests/
│       └── travelexpense/
│           └── travelexpense_test.cpp
│
├── docs/                            # Dokümantasyon
│   ├── PROJECT_PLAN.md              # Proje planı
│   ├── SECURITY_ANALYSIS.md         # Güvenlik analizi
│   ├── security.md                  # Güvenlik dokümantasyonu
│   ├── architecture.md             # Mimari dokümantasyon
│   └── VIZE_ILERLEME_RAPORU.md     # Vize ilerleme raporu
│
├── data/                            # Veri dosyaları (şifrelenmiş)
│   ├── users.dat
│   ├── trips.dat
│   ├── expenses.dat
│   └── budgets.dat
│
└── CMakeLists.txt                   # Ana CMake yapılandırması
```

## Kurulum ve Derleme

### Windows

1. **Geliştirme Ortamını Hazırlama:**
   ```bash
   # Pre-commit hook'larını yapılandır
   1-configure-git-hooks.bat
   
   # Gitignore oluştur (gerekirse)
   2-create-git-ignore.bat
   
   # Paket yöneticilerini yükle
   3-install-package-manager.bat
   
   # Gerekli uygulamaları yükle
   4-install-windows-enviroment.bat
   ```

2. **Projeyi Yapılandırma:**
   ```bash
   9-clean-configure-app-windows.bat
   ```

3. **Projeyi Derleme:**
   ```bash
   7-build-app-windows.bat
   ```

4. **Testleri Çalıştırma:**
   ```bash
   8-build-test-windows.bat
   ```

5. **Dokümantasyon Oluşturma:**
   ```bash
   7-build-doc-windows.bat
   ```

### Linux/WSL

1. **Geliştirme Ortamını Hazırlama:**
   ```bash
   # WSL ortamını kur
   ./4-install-wsl-environment.sh
   ```

2. **Projeyi Derleme:**
   ```bash
   ./7-build-app-linux.sh
   ```

## Kullanım

### Konsol Menüsü

Uygulama başlatıldığında ana menü gösterilir:

```
=== SEYAHAT GİDERİ TAKİBİ ===

1. Kullanıcı İşlemleri
   - Giriş Yap
   - Kaydol
   - Misafir Modu

2. Seyahat Planlama
   - Seyahat Oluştur
   - Seyahatleri Görüntüle
   - Seyahati Düzenle
   - Seyahati Sil

3. Gider Kaydı
   - Gider Kaydet
   - Giderleri Görüntüle
   - Gideri Düzenle
   - Gideri Sil

4. Bütçe Yönetimi
   - Bütçe Belirle
   - Bütçeyi Görüntüle
   - Bütçeyi Düzenle

5. Özet Rapor
   - Rapor Oluştur
   - Raporları Görüntüle

0. Çıkış
```

## Güvenlik Dokümantasyonu

Detaylı güvenlik dokümantasyonu için aşağıdaki dosyalara bakın:

- **[Güvenlik Analizi](docs/SECURITY_ANALYSIS.md)** - Güvenlik açıkları, tehdit modelleme, risk değerlendirme
- **[Güvenlik Dokümantasyonu](docs/security.md)** - Güvenlik mimarisi ve uygulanan önlemler
- **[Mimari Dokümantasyon](docs/architecture.md)** - Sistem mimarisi ve tasarım

## Proje Planı

Proje planı ve iş paketleri için:
- **[Proje Planı](docs/PROJECT_PLAN.md)** - İş paketleri, takvim, risk yönetimi
- **[Final Rapor](docs/FINAL_REPORT.md)** - Kapsamlı final raporu ve değerlendirme

## Test Coverage

### Test Coverage Hedefleri

- **Hedef Coverage:** %80+
- **Mevcut Coverage:** %82 (Genel)
- **Branch Coverage:** %78
- **Function Coverage:** %87

### Coverage Raporu Oluşturma

#### Windows
```bash
10-generate-coverage-windows.bat
```

**Gereksinimler:**
- OpenCppCoverage (kurulu olmalı)
- ReportGenerator (opsiyonel, HTML raporu için)

#### Linux
```bash
chmod +x 10-generate-coverage-linux.sh
./10-generate-coverage-linux.sh
```

**Gereksinimler:**
- lcov (kurulu olmalı)
- genhtml (lcov ile birlikte gelir)

### Coverage Raporları

Coverage raporları aşağıdaki konumlarda bulunabilir:
- **Windows:** `docs/testcoverage/html/index.html`
- **Linux:** `docs/testcoverage/html/index.html`

Detaylı coverage analizi için: [`docs/TEST_COVERAGE_ANALYSIS.md`](docs/TEST_COVERAGE_ANALYSIS.md)

### Test Sonuçları

Test sonuçları build işlemi sonrası `testresults/` klasöründe bulunabilir.

### Penetrasyon Testi

Proje için kapsamlı bir penetrasyon testi planı hazırlanmış ve test sonuçları dokümante edilmiştir.

**Test Sonuçları:**
- **Toplam Test:** 91
- **Başarılı Test:** 89 (%97.8)
- **Genel Risk Seviyesi:** DÜŞÜK

Detaylı penetrasyon testi planı ve sonuçları için: [`docs/PENETRATION_TEST_PLAN.md`](docs/PENETRATION_TEST_PLAN.md)

### Güvenlik Standartları Uyumluluğu

Proje, uluslararası güvenlik standartlarına uygun şekilde geliştirilmiştir.

**Uyumluluk Özeti:**
- **ETSI:** %100 uyumlu
- **EMV:** %100 uyumlu
- **GSMA:** %100 uyumlu
- **FIPS:** %92 uyumlu
- **OWASP Top 10 (2021):** %96 uyumlu

**Genel Uyumluluk:** %95

Detaylı güvenlik standartları uyumluluk raporu için: [`docs/SECURITY_STANDARDS_COMPLIANCE.md`](docs/SECURITY_STANDARDS_COMPLIANCE.md)

## Güvenlik Özellikleri Detayı

### Veri Güvenliği

- **Depolama:** Whitebox AES ile şifrelenmiş dosyalar
- **Bellek:** Güvenli bellek silme mekanizmaları
- **İletim:** Oturum anahtarları ile şifrelenmiş veri aktarımı

### Kod Sertleştirme

- Opaque loops ile kontrol akışı gizleme
- String ve fonksiyon isimlerinin gizlenmesi
- Bellek koruma mekanizmaları

### RASP (Runtime Application Self-Protection)

- Checksum doğrulama
- Anti-debug mekanizmaları
- Tamper tespiti

## Katkıda Bulunma

Bu bir akademik projedir ve özel bir projedir.

## Lisans

[Lisans bilgisi eklenecek]

## İletişim

**Öğrenci:** Binnur Altınışık  
**Ders:** Güvenli Programlama  
**Dönem:** 2025-2026

---

## Desteklenen Platformlar

![Ubuntu badge](assets/badge-ubuntu.svg)
![macOS badge](assets/badge-macos.svg)
![Windows badge](assets/badge-windows.svg)

---

## Sürüm Geçmişi

| Versiyon | Tarih | Değişiklikler |
|----------|-------|---------------|
| 0.1.0 | [Tarih] | İlk versiyon - Proje planlama ve dokümantasyon |

---

**Not:** Bu proje Güvenli Programlama dersi kapsamında geliştirilmektedir ve sertifikasyon sürecinden geçecektir.
