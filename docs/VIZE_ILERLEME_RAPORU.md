# Vize İlerleme Raporu

## Proje Bilgileri

**Proje Adı:** Seyahat Gideri Takibi (Travel Expense Tracking)  
**Ders:** Güvenli Programlama  
**Öğrenci:** Binnur Altınışık  
**Rapor Tarihi:** [Tarih]  
**Rapor Versiyonu:** 1.0

---

## 1. Özet

Bu rapor, Seyahat Gideri Takibi projesinin vize dönemindeki ilerlemesini, tamamlanan iş paketlerini, karşılaşılan zorlukları ve gelecek planları detaylandırmaktadır.

### 1.1 Proje Durumu

- **Genel İlerleme:** [%] tamamlandı
- **Planlanan İş Paketleri:** 9
- **Tamamlanan İş Paketleri:** [X]
- **Devam Eden İş Paketleri:** [Y]
- **Bekleyen İş Paketleri:** [Z]

### 1.2 Temel Başarılar

- [ ] Proje planı hazırlandı ve onaylandı
- [ ] Güvenlik analizi dokümantasyonu tamamlandı
- [ ] Temel modül yapısı oluşturuldu
- [ ] Güvenlik katmanı tasarlandı
- [ ] Test framework entegrasyonu yapıldı

---

## 2. Tamamlanan İş Paketleri

### 2.1 İş Paketi 1.1: Proje Planlama ve Dokümantasyon Hazırlığı

**Durum:** ✅ Tamamlandı  
**Tarih:** [Tarih]  
**Süre:** 1 hafta

#### Çıktılar:
- [x] Proje planı dokümantasyonu (`PROJECT_PLAN.md`)
- [x] Güvenlik analizi dokümantasyonu (`SECURITY_ANALYSIS.md`)
- [x] Mimari dokümantasyon şablonu (`architecture.md`)
- [x] Güvenlik dokümantasyon şablonu (`security.md`)

#### Yapılan Çalışmalar:
- Proje gereksinimlerinin analizi
- İş paketlerinin belirlenmesi
- Takvim oluşturulması
- GitHub repository yapısının hazırlanması

---

### 2.2 İş Paketi 1.2: Güvenlik Analizi ve Tehdit Modelleme

**Durum:** ✅ Tamamlandı  
**Tarih:** [Tarih]  
**Süre:** 1 hafta

#### Çıktılar:
- [x] Güvenlik açığı analizi
- [x] Tehdit modeli (STRIDE)
- [x] Risk matrisi
- [x] Güvenlik gereksinimleri dokümantasyonu

#### Tespit Edilen Güvenlik Açıkları:

| # | Açıklık | Öncelik | Durum |
|---|---------|---------|-------|
| 1 | Açık Veri Depolama | Yüksek | Çözülecek |
| 2 | Güvensiz Bellek Yönetimi | Yüksek | Çözülecek |
| 3 | Zayıf Kimlik Doğrulama | Yüksek | Çözülecek |
| 4 | Kod Analizi Kolaylığı | Orta | Çözülecek |
| 5 | Debug Kontrolü Yok | Orta | Çözülecek |

---

### 2.3 İş Paketi 1.3: Temel Uygulama Modüllerinin Geliştirilmesi

**Durum:** 🔄 Devam Ediyor  
**Başlangıç Tarihi:** [Tarih]  
**Tahmini Süre:** 2 hafta

#### Tamamlanan Alt Görevler:
- [x] Modül yapısının oluşturulması
- [x] Veri modellerinin tanımlanması
- [ ] Kullanıcı Kimlik Doğrulama modülü
- [ ] Seyahat Planlama modülü
- [ ] Gider Kaydı modülü
- [ ] Bütçe Yönetimi modülü
- [ ] Özet Rapor modülü

#### Kod İlerlemesi:

```cpp
// Tamamlanan dosyalar:
src/travelexpense/header/travelexpense.h      [%100]
src/travelexpense/src/travelexpense.cpp        [%XX]
src/travelexpense/header/userAuth.h           [%XX]
src/travelexpense/src/userAuth.cpp            [%XX]
// ...
```

---

### 2.4 İş Paketi 1.4: Veri Güvenliği Katmanının Uygulanması

**Durum:** ⏳ Beklemede  
**Başlangıç Tarihi:** [Tarih]  
**Tahmini Süre:** 1.5 hafta

#### Planlanan Alt Görevler:
- [ ] Whitebox AES şifreleme implementasyonu
- [ ] Güvenli bellek silme fonksiyonları
- [ ] Oturum anahtarı yönetimi
- [ ] Dosya şifreleme mekanizması

---

### 2.5 İş Paketi 1.5: Kod Sertleştirme Tekniklerinin Uygulanması

**Durum:** ⏳ Beklemede  
**Başlangıç Tarihi:** [Tarih]  
**Tahmini Süre:** 1 hafta

---

### 2.6 İş Paketi 1.6: RASP Tekniklerinin Uygulanması

**Durum:** ⏳ Beklemede  
**Başlangıç Tarihi:** [Tarih]  
**Tahmini Süre:** 1 hafta

---

### 2.7 İş Paketi 1.7: Konsol Menü Arayüzünün Geliştirilmesi

**Durum:** 🔄 Devam Ediyor  
**Başlangıç Tarihi:** [Tarih]  
**Tahmini Süre:** 1 hafta

#### Tamamlanan Alt Görevler:
- [x] Menü yapısının tasarlanması
- [ ] Ana menü implementasyonu
- [ ] Navigasyon sistemi
- [ ] Input/Output validasyonu

---

### 2.8 İş Paketi 1.8: Birim Testlerinin Yazılması

**Durum:** ⏳ Beklemede  
**Başlangıç Tarihi:** [Tarih]  
**Tahmini Süre:** 1 hafta

---

### 2.9 İş Paketi 1.9: Vize Ara Raporunun Hazırlanması

**Durum:** 🔄 Devam Ediyor (Bu rapor)  
**Tarih:** [Tarih]  
**Süre:** 3 gün

---

## 3. Güvenlik Gereksinimlerinin Karşılanma Durumu

### 3.1 Veri Güvenliği

| Gereksinim | Durum | Notlar |
|------------|-------|--------|
| Kullanımda veri güvenliği | 🔄 Geliştiriliyor | Güvenli bellek silme tasarım aşamasında |
| İletimde veri güvenliği | ⏳ Planlanıyor | Oturum anahtarları tasarlanıyor |
| Depolamada veri güvenliği | ⏳ Planlanıyor | Whitebox AES tasarım aşamasında |

### 3.2 Kod Sertleştirme

| Teknik | Durum | İlerleme |
|--------|-------|----------|
| Kontrol akışı gizleme | ⏳ Planlanıyor | - |
| String/fonksiyon gizleme | ⏳ Planlanıyor | - |
| Bellek koruması | 🔄 Geliştiriliyor | %30 |

### 3.3 RASP Teknikleri

| Teknik | Durum | İlerleme |
|--------|-------|----------|
| Checksum doğrulama | ⏳ Planlanıyor | - |
| Anti-debug mekanizmaları | ⏳ Planlanıyor | - |
| Tamper tespiti | ⏳ Planlanıyor | - |

---

## 4. Karşılaşılan Zorluklar ve Çözümler

### 4.1 Teknik Zorluklar

#### Zorluk 1: Whitebox Kriptografi Implementasyonu
- **Sorun:** Whitebox AES implementasyonu karmaşık
- **Çözüm:** Basitleştirilmiş bir versiyon ile başlama, ileride geliştirme
- **Durum:** Araştırma aşamasında

#### Zorluk 2: Güvenli Bellek Silme
- **Sorun:** Platform bazlı farklılıklar (Windows/Linux)
- **Çözüm:** Platform-specific kodlar ile çözülecek
- **Durum:** Tasarım aşamasında

#### Zorluk 3: Anti-Debug Mekanizmaları
- **Sorun:** Platform bazlı API farklılıkları
- **Çözüm:** Conditional compilation kullanılacak
- **Durum:** Araştırma aşamasında

### 4.2 Zaman Yönetimi Zorlukları

- **Sorun:** İş paketlerinin zaman tahminleri gerçekçi olmayabilir
- **Çözüm:** Buffer zaman ekleme ve öncelik belirleme
- **Durum:** İzleniyor

---

## 5. Test Sonuçları

### 5.1 Unit Test Sonuçları

| Modül | Test Sayısı | Başarılı | Başarısız | Coverage |
|-------|-------------|----------|-----------|----------|
| UserAuth | [X] | [Y] | [Z] | [%] |
| TripManager | [X] | [Y] | [Z] | [%] |
| ExpenseManager | [X] | [Y] | [Z] | [%] |
| BudgetManager | [X] | [Y] | [Z] | [%] |
| ReportGenerator | [X] | [Y] | [Z] | [%] |
| **TOPLAM** | **[X]** | **[Y]** | **[Z]** | **[%]** |

### 5.2 Test Coverage Hedefleri

- **Hedef:** %80
- **Mevcut:** [%]
- **Durum:** [Hedefin altında/üstünde]

---

## 6. Sonraki Adımlar (Vize Sonrası)

### 6.1 Kısa Vadeli Planlar (Önümüzdeki 2-3 Hafta)

1. **Tamamlanması Gerekenler:**
   - Tüm temel modüllerin implementasyonu
   - Veri güvenliği katmanının uygulanması
   - Kod sertleştirme tekniklerinin uygulanması
   - RASP tekniklerinin uygulanması

2. **Test Faaliyetleri:**
   - Unit testlerin tamamlanması
   - Integration testlerinin yazılması
   - Security testlerinin yazılması

### 6.2 Orta Vadeli Planlar (Final Öncesi)

1. İleri düzey güvenlik özelliklerinin eklenmesi
2. Penetrasyon testi planının hazırlanması
3. Güvenlik sertifikasyon dokümantasyonunun hazırlanması

---

## 7. Risk Analizi Güncellemesi

### 7.1 Yeni Riskler

| Risk ID | Risk Açıklaması | Olasılık | Etki | Öncelik |
|---------|----------------|----------|------|---------|
| R007 | Whitebox kriptografi implementasyonu zorluğu | Orta | Yüksek | Yüksek |
| R008 | Zaman kısıtlamaları | Yüksek | Orta | Yüksek |

### 7.2 Risk Azaltma Planları

- **R007:** Basitleştirilmiş versiyon ile başlama
- **R008:** Öncelik sıralaması ve zaman yönetimi

---

## 8. Değerlendirme Kriterleri Karşılanma Durumu

### 8.1 Vize Rubriği Uyumluluk

| Kriter | Ağırlık | Durum | Notlar |
|--------|---------|-------|--------|
| Güvenlik Analizi | %15 | ✅ Tamamlandı | Threat model, risk assessment |
| Veri Güvenliği | %20 | 🔄 Geliştiriliyor | Tasarım aşamasında |
| Kod Sertleştirme | %20 | ⏳ Planlanıyor | Henüz başlanmadı |
| RASP Teknikleri | %15 | ⏳ Planlanıyor | Henüz başlanmadı |
| Proje Yönetimi | %20 | ✅ Tamamlandı | GitHub planı hazır |
| Ara Rapor | %10 | ✅ Tamamlandı | Bu rapor |

**Genel İlerleme:** [%XX] karşılandı

---

## 9. Sonuç ve Öneriler

### 9.1 Özet

Vize döneminde proje planlama, güvenlik analizi ve temel mimari tasarım tamamlanmıştır. Kod geliştirme aşamasına geçilmiş ve temel modüller geliştirilmeye başlanmıştır. Güvenlik katmanlarının uygulanması önümüzdeki haftalarda tamamlanacaktır.

### 9.2 Öneriler

1. **Hızlandırılması Gerekenler:**
   - Temel modül implementasyonu
   - Veri güvenliği katmanı

2. **Dikkat Edilmesi Gerekenler:**
   - Whitebox kriptografi implementasyonu
   - Platform bazlı kod farklılıkları
   - Test coverage hedefleri

3. **İyileştirme Önerileri:**
   - Daha detaylı kod dokümantasyonu
   - Daha sık test çalıştırma
   - Erken güvenlik testleri

---

## Ekler

### Ek A: Kod İstatistikleri

- **Toplam Satır:** [X]
- **Header Dosyası Sayısı:** [Y]
- **Source Dosyası Sayısı:** [Z]
- **Test Dosyası Sayısı:** [W]

### Ek B: Dokümantasyon İstatistikleri

- **Dokümantasyon Sayfası:** [X]
- **Toplam Kelime:** [Y]

### Ek C: GitHub Commit İstatistikleri

- **Toplam Commit:** [X]
- **Aktif Branches:** [Y]
- **Pull Requests:** [Z]

---

**Hazırlayan:** Binnur Altınışık  
**Tarih:** [Tarih]  
**Onaylayan:** [Ders Sorumlusu İsmi]  
**Onay Tarihi:** [Tarih]

