# Seyahat Gideri Takibi Projesi - Proje Planı

## Proje Bilgileri

**Proje Adı:** Seyahat Gideri Takibi (Travel Expense Tracking)  
**Ders:** Güvenli Programlama  
**Dönem:** 2025-2026  
**Öğrenci:** Binnur Altınışık  
**Proje Tipi:** C++ Konsol Uygulaması + DLL  

## Proje Özeti

Bu proje, kullanıcıların seyahat planlarını yapmasına, giderlerini kaydetmesine, bütçelerini yönetmesine ve özet raporlar oluşturmasına olanak sağlayan güvenli bir C++ konsol uygulamasıdır. Uygulama, güvenlik standartlarına uygun şekilde geliştirilecek ve sertifikasyon sürecinden geçecek şekilde tasarlanacaktır.

## Proje Aşamaları

### Faz 1: VİZE AŞAMASI (%40)

#### İş Paketi 1.1: Proje Planlama ve Dokümantasyon Hazırlığı
- **Süre:** 1 hafta
- **Çıktılar:**
  - Proje planı dokümantasyonu
  - Güvenlik analizi dokümantasyonu
  - Risk değerlendirme raporu
  - Tehdit modelleme dokümantasyonu
- **Durum:** 🔄 Devam Ediyor

#### İş Paketi 1.2: Güvenlik Analizi ve Tehdit Modelleme
- **Süre:** 1 hafta
- **Çıktılar:**
  - Güvenlik açığı analizi
  - Tehdit modeli (Threat Model)
  - Risk matrisi
  - Güvenlik gereksinimleri dokümantasyonu
- **Bağımlılıklar:** İş Paketi 1.1
- **Durum:** ⏳ Beklemede

#### İş Paketi 1.3: Temel Uygulama Modüllerinin Geliştirilmesi
- **Süre:** 2 hafta
- **Alt Görevler:**
  - Kullanıcı Kimlik Doğrulama modülü
  - Seyahat Planlama modülü
  - Gider Kaydı modülü
  - Bütçe Yönetimi modülü
  - Özet Rapor modülü
- **Çıktılar:**
  - Tüm modüllerin header ve source dosyaları
  - Temel fonksiyon implementasyonları
  - Dosya I/O işlemleri (binary format)
- **Durum:** ⏳ Beklemede

#### İş Paketi 1.4: Veri Güvenliği Katmanının Uygulanması
- **Süre:** 1.5 hafta
- **Alt Görevler:**
  - Kullanımda veri güvenliği (bellekte şifreleme, güvenli silme)
  - İletimde veri güvenliği (oturum anahtarları, şifreleme)
  - Depolamada veri güvenliği (Whitebox AES/DES, ikili dosya şifreleme)
- **Çıktılar:**
  - Şifreleme modülleri
  - Güvenli bellek yönetimi fonksiyonları
  - Şifreli dosya depolama mekanizması
- **Bağımlılıklar:** İş Paketi 1.3
- **Durum:** ⏳ Beklemede

#### İş Paketi 1.5: Kod Sertleştirme Tekniklerinin Uygulanması
- **Süre:** 1 hafta
- **Alt Görevler:**
  - Kontrol akışı gizleme (Opaque loops)
  - String ve fonksiyon isimlerinin gizlenmesi
  - Bellek koruması (güvenli silme mekanizmaları)
  - Opaque boolean değişkenleri
- **Çıktılar:**
  - Sertleştirilmiş kod yapısı
  - Gizlenmiş string'ler ve fonksiyon isimleri
  - Kontrol akışı karmaşıklığı artırılmış kod
- **Bağımlılıklar:** İş Paketi 1.4
- **Durum:** ⏳ Beklemede

#### İş Paketi 1.6: RASP Tekniklerinin Uygulanması
- **Süre:** 1 hafta
- **Alt Görevler:**
  - Checksum doğrulama mekanizması
  - Anti-debug mekanizmaları
  - Tamper tespiti (müdahale algılama)
- **Çıktılar:**
  - Checksum doğrulama fonksiyonları
  - Debug tespiti ve önleme mekanizmaları
  - Müdahale algılama sistemi
- **Bağımlılıklar:** İş Paketi 1.5
- **Durum:** ⏳ Beklemede

#### İş Paketi 1.7: Konsol Menü Arayüzünün Geliştirilmesi
- **Süre:** 1 hafta
- **Alt Görevler:**
  - Ana menü tasarımı
  - Menü navigasyonu
  - Input/Output validasyonu
  - Hata yönetimi
- **Çıktılar:**
  - Çalışan konsol uygulaması
  - Kullanıcı dostu menü sistemi
- **Bağımlılıklar:** İş Paketi 1.3
- **Durum:** ⏳ Beklemede

#### İş Paketi 1.8: Birim Testlerinin Yazılması
- **Süre:** 1 hafta
- **Alt Görevler:**
  - Her modül için unit testler
  - Güvenlik fonksiyonları için testler
  - Test coverage raporları
- **Çıktılar:**
  - GoogleTest tabanlı test suite
  - Test coverage raporları
- **Bağımlılıklar:** İş Paketi 1.6
- **Durum:** ⏳ Beklemede

#### İş Paketi 1.9: Vize Ara Raporunun Hazırlanması
- **Süre:** 3 gün
- **Çıktılar:**
  - Ara rapor dokümantasyonu
  - İlerleme raporu
  - Güvenlik gereksinimlerinin dokümantasyonu
  - Test sonuçları
- **Bağımlılıklar:** Tüm iş paketleri
- **Durum:** ⏳ Beklemede

### Faz 2: FİNAL AŞAMASI (%60)

#### İş Paketi 2.1: İleri Düzey Güvenlik Özelliklerinin Eklenmesi
- **Süre:** 2 hafta
- **Çıktılar:**
  - SSL/TLS entegrasyonu
  - Sertifika pinning
  - Dinamik anahtar yönetimi
  - HOOK saldırı tespiti
- **Durum:** ⏳ Beklemede

#### İş Paketi 2.2: İkili Uygulama Koruma Özelliklerinin Geliştirilmesi
- **Süre:** 1.5 hafta
- **Çıktılar:**
  - Emulator tespiti
  - Güvensiz cihaz tespiti
  - Kontrol akışı sayacı
  - Onarma mekanizmaları
- **Durum:** ⏳ Beklemede

#### İş Paketi 2.3: Penetrasyon Testi ve Güvenlik Sertifikasyonu
- **Süre:** 2 hafta
- **Çıktılar:**
  - Penetrasyon testi planı
  - Güvenlik sertifikasyonu dokümantasyonu
  - Güvenlik açığı raporları
  - İyileştirme önerileri
- **Durum:** ⏳ Beklemede

#### İş Paketi 2.4: Final Raporu ve Dokümantasyonun Tamamlanması
- **Süre:** 1 hafta
- **Çıktılar:**
  - Final proje raporu
  - Eksiksiz kod dokümantasyonu
  - Güvenlik dokümantasyonu
  - Test sonuçları ve coverage raporları
- **Durum:** ⏳ Beklemede

## Takvim

### Vize Dönemi (8-9 Hafta)

| Hafta | Tarih Aralığı | İş Paketleri | Durum |
|-------|---------------|--------------|-------|
| 1 | [Başlangıç] | 1.1, 1.2 | 🔄 |
| 2 | | 1.3 (Başlangıç) | ⏳ |
| 3-4 | | 1.3 (Devam) | ⏳ |
| 5-6 | | 1.4 | ⏳ |
| 7 | | 1.5 | ⏳ |
| 8 | | 1.6, 1.7 | ⏳ |
| 9 | | 1.8, 1.9 | ⏳ |

### Final Dönemi (6-7 Hafta)

| Hafta | Tarih Aralığı | İş Paketleri | Durum |
|-------|---------------|--------------|-------|
| 10-11 | | 2.1 | ⏳ |
| 12-13 | | 2.2 | ⏳ |
| 14-15 | | 2.3 | ⏳ |
| 16 | | 2.4 | ⏳ |

## Risk Yönetimi

### Yüksek Riskli Durumlar

1. **Güvenlik Teknolojilerinin Karmaşıklığı**
   - **Risk:** Whitebox kriptografi ve RASP tekniklerinin uygulanması zor olabilir
   - **Çözüm:** Erken araştırma ve proof-of-concept geliştirme
   - **Atanmış Kişi:** [Takım Üyesi]

2. **Zaman Kısıtlamaları**
   - **Risk:** Tüm iş paketlerinin zamanında tamamlanamama riski
   - **Çözüm:** Buffer zaman ekleme ve öncelik belirleme
   - **Atanmış Kişi:** [Takım Üyesi]

3. **Test Coverage**
   - **Risk:** Test coverage hedeflerine ulaşamama
   - **Çözüm:** Test-driven development yaklaşımı
   - **Atanmış Kişi:** [Takım Üyesi]

## Kaynak Yönetimi

### Gerekli Araçlar ve Kütüphaneler

- **IDE:** Visual Studio Community Edition
- **Build System:** CMake >= 3.12
- **Test Framework:** GoogleTest
- **Kriptografi:** Whitebox AES/DES implementasyonu
- **Dokümantasyon:** Doxygen
- **Version Control:** Git/GitHub
- **Code Coverage:** OpenCppCoverage / gcov

### Gerekli Bilgi ve Beceriler

- C++11+ programlama
- Güvenlik temel kavramları
- Kriptografi bilgisi
- Unit testing
- CMake kullanımı

## İletişim ve İşbirlikçi Çalışma

### Toplantı Takvimi
- **Haftalık Toplantı:** Her [Gün] saat [Saat]
- **Sprint Review:** Her 2 haftada bir
- **Acil Durum Toplantıları:** İhtiyaç halinde

### İşbirlikçi Araçlar
- **Version Control:** GitHub
- **Proje Yönetimi:** GitHub Projects / Issues
- **Dokümantasyon:** Markdown files in repository
- **Code Review:** GitHub Pull Requests

## Onay

- [ ] Proje Planı Hazırlandı
- [ ] Proje Planı Onaylandı (Ders Sorumlusu)
- [ ] GitHub Repository Oluşturuldu
- [ ] İlk Milestone Belirlendi

**Plan Onay Tarihi:** [Tarih]  
**Plan Onaylayan:** [İsim]  
**Son Güncelleme:** [Tarih]

---

## Değişiklik Geçmişi

| Versiyon | Tarih | Değişiklik | Yapan |
|----------|-------|------------|-------|
| 1.0 | [Tarih] | İlk versiyon oluşturuldu | [İsim] |

