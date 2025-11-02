# Seyahat Gideri Takibi Projesi

## Genel Bakış

Bu proje, kullanıcıların seyahat planlarını yapmasına, giderlerini kaydetmesine, bütçelerini yönetmesine ve özet raporlar oluşturmasına olanak sağlayan güvenli bir C++ konsol uygulamasıdır.

Proje, **Güvenli Programlama** dersi kapsamında geliştirilmektedir ve güvenlik standartlarına uygun şekilde tasarlanmıştır.

## Özellikler

### Temel Özellikler

- ✅ **Kullanıcı Kimlik Doğrulama:** Giriş, kayıt, misafir modu
- ✅ **Seyahat Planlama:** Seyahat oluşturma, görüntüleme, düzenleme, silme
- ✅ **Gider Kaydı:** Gider kaydetme, görüntüleme, düzenleme, silme
- ✅ **Bütçe Yönetimi:** Bütçe belirleme, görüntüleme, düzenleme
- ✅ **Özet Rapor:** Seyahat masrafları ve deneyimleri raporu

### Güvenlik Özellikleri

- 🔒 **Veri Şifreleme:** Whitebox AES ile hassas verilerin şifrelenmesi
- 🔒 **Güvenli Bellek Yönetimi:** Hassas verilerin güvenli şekilde silinmesi
- 🔒 **Kod Sertleştirme:** Opaque loops, string/fonksiyon gizleme
- 🔒 **RASP Teknikleri:** Checksum doğrulama, anti-debug, tamper tespiti

## Dokümantasyon

### Proje Yönetimi

- **[Proje Planı](PROJECT_PLAN.md)** - İş paketleri, takvim, risk yönetimi
- **[Vize İlerleme Raporu](VIZE_ILERLEME_RAPORU.md)** - Vize dönemi ilerleme raporu

### Güvenlik Dokümantasyonu

- **[Güvenlik Analizi](SECURITY_ANALYSIS.md)** - Güvenlik açıkları, tehdit modelleme, risk değerlendirme
- **[Güvenlik Dokümantasyonu](security.md)** - Güvenlik mimarisi ve uygulanan önlemler

### Teknik Dokümantasyon

- **[Mimari Dokümantasyon](architecture.md)** - Sistem mimarisi, veri modelleri, API tasarımı
- **[Geliştirici Dokümantasyonu](developers.md)** - Geliştirici rehberi

## Proje Yapısı

```
src/
├── travelexpense/          # Kütüphane (İş Mantığı)
│   ├── header/
│   └── src/
│
├── travelexpenseapp/        # Konsol Uygulaması (Menü)
│   ├── header/
│   └── src/
│
└── tests/
    └── travelexpense/
        └── travelexpense_test.cpp
```

## Hızlı Başlangıç

### Gereksinimler

- CMake >= 3.12
- C++ Standard >= 11
- GoogleTest (testler için)
- Visual Studio Community Edition (Windows)
- Ninja (WSL/Linux)

### Derleme

**Windows:**
```bash
7-build-app-windows.bat
```

**Linux/WSL:**
```bash
./7-build-app-linux.sh
```

### Çalıştırma

```bash
./build/Release/travelexpenseapp.exe
```

## Proje Durumu

### Vize Dönemi (Devam Ediyor)

- ✅ Proje planlama ve dokümantasyon
- ✅ Güvenlik analizi ve tehdit modelleme
- 🔄 Temel uygulama modüllerinin geliştirilmesi
- ⏳ Veri güvenliği katmanının uygulanması
- ⏳ Kod sertleştirme tekniklerinin uygulanması
- ⏳ RASP tekniklerinin uygulanması

### Final Dönemi (Planlanıyor)

- ⏳ İleri düzey güvenlik özellikleri
- ⏳ Penetrasyon testi
- ⏳ Güvenlik sertifikasyonu

## İletişim

**Öğrenci:** Binnur Altınışık  
**Ders:** Güvenli Programlama  
**Dönem:** 2025-2026

---

## Desteklenen Platformlar

![Ubuntu badge](../assets/badge-ubuntu.svg)
![macOS badge](../assets/badge-macos.svg)
![Windows badge](../assets/badge-windows.svg)
