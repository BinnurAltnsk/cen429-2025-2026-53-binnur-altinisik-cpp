# Güvenlik Analizi Dokümantasyonu

## 1. Güvenlik Açığı Analizi

### 1.1 Tespit Edilen Güvenlik Açıkları

#### Yüksek Öncelikli Açıklar

| # | Açıklık Adı | Açıklama | Etki | Öncelik |
|---|-------------|----------|------|---------|
| 1 | Açık Veri Depolama | Kullanıcı verileri ve gider bilgileri şifrelenmeden depolanıyor | Veri sızıntısı, kimlik hırsızlığı | Yüksek |
| 2 | Güvensiz Bellek Yönetimi | Hassas veriler bellekten güvenli şekilde silinmiyor | Bellek dump ile veri çalınması | Yüksek |
| 3 | Zayıf Kimlik Doğrulama | Kullanıcı şifreleri hashlenmemiş veya zayıf hashlenmiş | Brute force saldırıları | Yüksek |
| 4 | Kod Analizi Kolaylığı | String'ler ve fonksiyon isimleri açık şekilde saklanıyor | Reverse engineering | Orta |
| 5 | Debug Kontrolü Yok | Uygulama debug edilebilir durumda | Kod analizi ve manipülasyon | Orta |

#### Orta Öncelikli Açıklar

| # | Açıklık Adı | Açıklama | Etki | Öncelik |
|---|-------------|----------|------|---------|
| 6 | Tamper Tespiti Yok | Dosya veya kod değişikliği tespit edilemiyor | Kötü amaçlı değişiklikler | Orta |
| 7 | Checksum Kontrolü Yok | Uygulama bütünlüğü kontrol edilmiyor | Manipüle edilmiş binary'ler | Orta |
| 8 | Session Yönetimi Zayıf | Oturum anahtarları güvensiz | Session hijacking | Orta |
| 9 | Input Validation Eksikliği | Kullanıcı girdileri yeterince doğrulanmıyor | Buffer overflow, injection | Orta |

#### Düşük Öncelikli Açıklar

| # | Açıklık Adı | Açıklama | Etki | Öncelik |
|---|-------------|----------|------|---------|
| 10 | Logging Güvenliği | Hassas veriler loglara yazılıyor olabilir | Bilgi sızıntısı | Düşük |
| 11 | Hata Mesajları Fazla Detaylı | Hata mesajları sistem bilgisi veriyor | Bilgi toplama | Düşük |

### 1.2 Açıklık Sınıflandırması

**OWASP Top 10 (2021) Uyumluluğu:**
- A01:2021 – Broken Access Control → İş Paketi 1.3 (Kullanıcı Kimlik Doğrulama)
- A02:2021 – Cryptographic Failures → İş Paketi 1.4 (Veri Güvenliği)
- A03:2021 – Injection → İş Paketi 1.7 (Input Validation)
- A04:2021 – Insecure Design → Tüm Faz 1
- A05:2021 – Security Misconfiguration → İş Paketi 1.6 (RASP)

## 2. Tehdit Modelleme (Threat Modeling)

### 2.1 Tehdit Modeli Diyagramı

```
                    ┌─────────────────┐
                    │   Saldırgan     │
                    │  (Threat Actor) │
                    └────────┬────────┘
                             │
                ┌────────────┴────────────┐
                │                         │
         ┌──────▼──────┐          ┌──────▼──────┐
         │ Dosya Sistemi│          │   Bellek    │
         │  Saldırıları │          │  Saldırıları│
         └─────────────┘          └─────────────┘
                │                         │
                └────────────┬────────────┘
                             │
                    ┌────────▼────────┐
                    │   Veri Çalma    │
                    │  (Data Theft)   │
                    └─────────────────┘
```

### 2.2 Tehdit Aktörleri (Threat Actors)

#### 2.2.1 External Attacker (Dış Saldırgan)
- **Motivasyon:** Finansal kazanç, kişisel bilgi çalma
- **Yetenekler:** Orta-Yüksek
- **Erişim:** Uzaktan erişim
- **Tespit Edilmesi:** Zor
- **Örnekler:**
  - Binary reverse engineering
  - Memory dump analizi
  - Dosya sistemine erişim

#### 2.2.2 Insider Threat (İç Tehdit)
- **Motivasyon:** Kişisel bilgi çalma, sistemi manipüle etme
- **Yetenekler:** Yüksek (sistem erişimi var)
- **Erişim:** Yerel erişim
- **Tespit Edilmesi:** Orta
- **Örnekler:**
  - Debugger ile kod analizi
  - Dosya manipülasyonu
  - Log analizi

### 2.3 Tehdit Senaryoları (Attack Scenarios)

#### Senaryo 1: Dosya Sisteminden Veri Çalma
- **Saldırgan:** Dış saldırgan
- **Hedef:** `data/users.dat`, `data/trips.dat`, `data/expenses.dat`
- **Saldırı Yöntemi:** Dosya sistemine fiziksel/dijital erişim
- **Etki:** Tüm kullanıcı verileri çalınabilir
- **Önlem:** Whitebox şifreleme, dosya yetkilendirme

#### Senaryo 2: Bellek Dump ile Hassas Veri Çalma
- **Saldırgan:** İç tehdit veya dış saldırgan (root erişimi)
- **Hedef:** Bellekteki şifreler, özel anahtarlar
- **Saldırı Yöntemi:** Process memory dump
- **Etki:** Aktif oturum bilgileri, şifreler çalınabilir
- **Önlem:** Güvenli bellek silme, bellek şifreleme

#### Senaryo 3: Reverse Engineering ile Kod Analizi
- **Saldırgan:** Dış saldırgan
- **Hedef:** Uygulama binary'si
- **Saldırı Yöntemi:** Disassembler, debugger kullanımı
- **Etki:** Şifreleme algoritmaları, gizli anahtarlar keşfedilebilir
- **Önlem:** Kod obfuscation, anti-debug mekanizmaları

#### Senaryo 4: Binary Manipülasyon (Tampering)
- **Saldırgan:** İç tehdit veya dış saldırgan
- **Hedef:** Uygulama binary dosyası
- **Saldırı Yöntemi:** Binary editing, hooking
- **Etki:** Güvenlik kontrolleri bypass edilebilir
- **Önlem:** Checksum doğrulama, tamper tespiti

### 2.4 STRIDE Modeli ile Tehdit Analizi

| Kategori | Tehdit | Açıklama | Öncelik |
|----------|--------|----------|---------|
| **S**poofing | Kimlik Taklidi | Sahte kullanıcı kimliği oluşturma | Orta |
| **T**ampering | Veri Manipülasyonu | Dosya veya bellek verilerini değiştirme | Yüksek |
| **R**epudiation | İnkar | İşlemlerin kaydını tutmama | Düşük |
| **I**nformation Disclosure | Bilgi Açığa Çıkarma | Hassas verilerin sızması | Yüksek |
| **D**enial of Service | Hizmet Reddi | Uygulamanın çalışmasını engelleme | Düşük |
| **E**levation of Privilege | Yetki Yükseltme | Kullanıcı yetkilerini artırma | Orta |

## 3. Risk Değerlendirme

### 3.1 Risk Matrisi

| Risk ID | Tehdit | Olasılık | Etki | Risk Skoru | Öncelik |
|---------|--------|----------|------|------------|---------|
| R001 | Açık veri depolama | Yüksek | Kritik | 25 (5x5) | 1 |
| R002 | Güvensiz bellek yönetimi | Yüksek | Yüksek | 20 (5x4) | 2 |
| R003 | Reverse engineering | Orta | Yüksek | 15 (3x5) | 3 |
| R004 | Binary tampering | Orta | Orta | 12 (3x4) | 4 |
| R005 | Zayıf kimlik doğrulama | Orta | Yüksek | 15 (3x5) | 3 |
| R006 | Debug erişimi | Düşük | Orta | 6 (2x3) | 5 |

**Risk Skoru = Olasılık (1-5) × Etki (1-5)**

### 3.2 Risk Seviyeleri

- **Kritik (20-25):** Hemen ele alınmalı
- **Yüksek (12-19):** Kısa sürede ele alınmalı
- **Orta (6-11):** Planlanmalı
- **Düşük (1-5):** Kabul edilebilir veya izlenmeli

### 3.3 Risk Azaltma Stratejileri

#### R001: Açık Veri Depolama
- **Azaltma:** Whitebox AES şifreleme uygulama
- **Maliyet:** Orta
- **Zaman:** 1.5 hafta
- **İş Paketi:** 1.4

#### R002: Güvensiz Bellek Yönetimi
- **Azaltma:** Güvenli bellek silme fonksiyonları
- **Maliyet:** Düşük
- **Zaman:** 3-5 gün
- **İş Paketi:** 1.4

#### R003: Reverse Engineering
- **Azaltma:** Kod obfuscation, string gizleme
- **Maliyet:** Orta
- **Zaman:** 1 hafta
- **İş Paketi:** 1.5

## 4. Güvenlik Gereksinimleri

### 4.1 Fonksiyonel Gereksinimler

1. **FR-SEC-001:** Tüm hassas veriler Whitebox AES ile şifrelenmelidir
2. **FR-SEC-002:** Bellekteki hassas veriler kullanım sonrası güvenli şekilde silinmelidir
3. **FR-SEC-003:** Kullanıcı şifreleri güçlü hash algoritmaları ile hashlenmelidir
4. **FR-SEC-004:** Oturum anahtarları şifrelenmiş formatta saklanmalıdır
5. **FR-SEC-005:** Uygulama bütünlüğü checksum ile doğrulanmalıdır
6. **FR-SEC-006:** Debug tespiti ve önleme mekanizması olmalıdır
7. **FR-SEC-007:** Tamper tespiti mekanizması olmalıdır

### 4.2 Güvenlik Olmayan Gereksinimler

1. **NFR-SEC-001:** Şifreleme/şifre çözme işlemleri <100ms tamamlanmalıdır
2. **NFR-SEC-002:** Güvenlik kontrolleri kullanıcı deneyimini %10'dan fazla etkilememelidir
3. **NFR-SEC-003:** Test coverage %80'in üzerinde olmalıdır

## 5. Güvenlik Kontrol Noktaları (Checkpoints)

### Vize Öncesi Kontroller

- [ ] Tüm yüksek öncelikli güvenlik açıkları giderildi
- [ ] Veri şifreleme mekanizması çalışıyor
- [ ] Güvenli bellek silme test edildi
- [ ] Checksum doğrulama çalışıyor
- [ ] Anti-debug mekanizması test edildi
- [ ] Kod obfuscation uygulandı
- [ ] Test coverage %70'in üzerinde

## 6. Referanslar

- OWASP Top 10 (2021)
- STRIDE Threat Model
- ETSI TS 102 484 - Security Requirements
- NIST Cybersecurity Framework

---

**Hazırlayan:** [İsim]  
**Tarih:** [Tarih]  
**Versiyon:** 1.0  
**Son Güncelleme:** [Tarih]

