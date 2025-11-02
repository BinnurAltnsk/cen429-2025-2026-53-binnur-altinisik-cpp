# Proje Tasarımı ve Mimari Dokümantasyonu

## 1. Genel Bakış

### 1.1 Proje Amaçları

Seyahat Gideri Takibi projesi, kullanıcıların seyahat planlarını yapmasına, giderlerini kaydetmesine, bütçelerini yönetmesine ve özet raporlar oluşturmasına olanak sağlayan güvenli bir C++ konsol uygulamasıdır.

### 1.2 Mimari İlkeler

- **Modüler Yapı:** Her özellik ayrı modül olarak geliştirilir
- **Güvenlik Öncelikli:** Tüm katmanlarda güvenlik mekanizmaları
- **Test Edilebilirlik:** Her modül bağımsız olarak test edilebilir
- **Bakım Kolaylığı:** Temiz kod ve dokümantasyon

## 2. Sistem Mimarisi

### 2.1 Katmanlı Mimari

```
┌─────────────────────────────────────────────┐
│       Kullanıcı Arayüzü Katmanı              │
│       (Presentation Layer)                   │
│  - Konsol Menü Sistemi                      │
│  - Input/Output İşlemleri                  │
│  - Kullanıcı Etkileşimi                    │
├─────────────────────────────────────────────┤
│       Uygulama Katmanı                       │
│       (Application Layer)                    │
│  - Menü Navigasyonu                         │
│  - İş Akışı Yönetimi                        │
│  - Validasyon                               │
├─────────────────────────────────────────────┤
│       İş Mantığı Katmanı                     │
│       (Business Logic Layer)                 │
│  - Kullanıcı Yönetimi                       │
│  - Seyahat Planlama                         │
│  - Gider Kaydı                              │
│  - Bütçe Yönetimi                           │
│  - Rapor Oluşturma                          │
├─────────────────────────────────────────────┤
│       Güvenlik Katmanı                       │
│       (Security Layer)                       │
│  - Şifreleme İşlemleri                      │
│  - Kimlik Doğrulama                         │
│  - Güvenli Bellek Yönetimi                  │
│  - RASP Mekanizmaları                       │
├─────────────────────────────────────────────┤
│       Veri Erişim Katmanı                    │
│       (Data Access Layer)                   │
│  - Binary Dosya I/O                        │
│  - Veri Serileştirme                        │
│  - Şifrelenmiş Depolama                     │
└─────────────────────────────────────────────┘
```

### 2.2 Modül Yapısı

```
src/
├── travelexpense/                    # Kütüphane (İş Mantığı)
│   ├── header/
│   │   ├── travelexpense.h           # Ana header
│   │   ├── userAuth.h                # Kullanıcı kimlik doğrulama
│   │   ├── tripManager.h             # Seyahat yönetimi
│   │   ├── expenseManager.h          # Gider yönetimi
│   │   ├── budgetManager.h           # Bütçe yönetimi
│   │   ├── reportGenerator.h        # Rapor oluşturma
│   │   └── security.h                # Güvenlik fonksiyonları
│   └── src/
│       ├── travelexpense.cpp
│       ├── userAuth.cpp
│       ├── tripManager.cpp
│       ├── expenseManager.cpp
│       ├── budgetManager.cpp
│       ├── reportGenerator.cpp
│       ├── security.cpp               # Şifreleme, güvenli silme
│       └── fileIO.cpp                 # Dosya I/O işlemleri
│
├── travelexpenseapp/                  # Konsol Uygulaması
│   ├── header/
│   │   └── travelexpenseapp.h        # Menü sistem header
│   └── src/
│       └── travelexpenseapp.cpp      # Main + menü sistemi
│
└── tests/
    └── travelexpense/
        └── travelexpense_test.cpp    # GoogleTest testleri
```

## 3. Veri Modelleri

### 3.1 Kullanıcı Veri Modeli

```cpp
struct User {
    int userId;
    char username[50];
    char passwordHash[64];        // SHA-256 hash
    char salt[32];                // Salt değeri
    bool isGuest;
    time_t createdAt;
    time_t lastLogin;
};
```

### 3.2 Seyahat Veri Modeli

```cpp
struct Trip {
    int tripId;
    int userId;
    char destination[100];
    char startDate[11];           // YYYY-MM-DD formatı
    char endDate[11];
    char accommodation[200];
    char transportation[100];
    double budget;
    double totalExpenses;
    time_t createdAt;
    time_t updatedAt;
};
```

### 3.3 Gider Veri Modeli

```cpp
struct Expense {
    int expenseId;
    int tripId;
    char category[50];             // Konaklama, Ulaşım, Yemek, Eğlence
    double amount;
    char currency[4];              // TRY, USD, EUR, vb.
    char date[11];
    char paymentMethod[50];        // Nakit, Kredi Kartı, vb.
    char description[200];
    time_t createdAt;
};
```

### 3.4 Bütçe Veri Modeli

```cpp
struct Budget {
    int budgetId;
    int tripId;
    double totalBudget;
    double spentAmount;
    double categoryBudgets[4];      // [Konaklama, Ulaşım, Yemek, Eğlence]
    double categorySpent[4];
    time_t createdAt;
    time_t updatedAt;
};
```

## 4. Dosya Yapısı

### 4.1 Dosya Organizasyonu

```
data/
├── users.dat                  # Kullanıcı verileri (şifrelenmiş)
├── trips.dat                  # Seyahat verileri (şifrelenmiş)
├── expenses.dat               # Gider verileri (şifrelenmiş)
├── budgets.dat                # Bütçe verileri (şifrelenmiş)
└── reports.dat                # Oluşturulan raporlar (şifrelenmiş)
```

### 4.2 Dosya Formatı

Her dosya aşağıdaki yapıda olacaktır:

```
[File Header]
- Magic Number (4 bytes): 0x5452564C ("TRVL")
- Version (2 bytes): 0x0001
- Encryption Flag (1 byte): 0x01 (şifrelenmiş)
- Checksum (32 bytes): SHA-256 hash

[Data Blocks]
- Block Count (4 bytes): Kaç veri bloğu var
- [Block 1]
  - Block Size (4 bytes)
  - Encrypted Data (N bytes)
- [Block 2]
  - ...
```

## 5. API Tasarımı

### 5.1 Kullanıcı Yönetimi API

```cpp
namespace TravelExpense {
    namespace UserAuth {
        // Kullanıcı kaydı
        bool registerUser(const char* username, const char* password);
        
        // Kullanıcı girişi
        bool loginUser(const char* username, const char* password);
        
        // Misafir modu
        bool enableGuestMode();
        
        // Kullanıcı çıkışı
        void logoutUser();
        
        // Mevcut kullanıcı bilgisi
        User* getCurrentUser();
    }
}
```

### 5.2 Seyahat Yönetimi API

```cpp
namespace TravelExpense {
    namespace TripManager {
        // Seyahat oluştur
        int createTrip(const Trip& trip);
        
        // Seyahatleri listele
        std::vector<Trip> getTrips(int userId);
        
        // Seyahat güncelle
        bool updateTrip(int tripId, const Trip& trip);
        
        // Seyahat sil
        bool deleteTrip(int tripId);
        
        // Seyahat detayı
        Trip* getTrip(int tripId);
    }
}
```

### 5.3 Gider Yönetimi API

```cpp
namespace TravelExpense {
    namespace ExpenseManager {
        // Gider kaydet
        int logExpense(const Expense& expense);
        
        // Giderleri listele
        std::vector<Expense> getExpenses(int tripId);
        
        // Gider güncelle
        bool updateExpense(int expenseId, const Expense& expense);
        
        // Gider sil
        bool deleteExpense(int expenseId);
    }
}
```

### 5.4 Bütçe Yönetimi API

```cpp
namespace TravelExpense {
    namespace BudgetManager {
        // Bütçe belirle
        int setBudget(int tripId, double totalBudget, const double* categoryBudgets);
        
        // Bütçeyi görüntüle
        Budget* getBudget(int tripId);
        
        // Bütçeyi güncelle
        bool updateBudget(int budgetId, const Budget& budget);
        
        // Bütçe kontrolü
        bool checkBudgetLimit(int tripId, const char* category, double amount);
    }
}
```

### 5.5 Rapor Oluşturma API

```cpp
namespace TravelExpense {
    namespace ReportGenerator {
        // Rapor oluştur
        std::string generateReport(int tripId);
        
        // Raporları listele
        std::vector<std::string> getReports(int userId);
        
        // Raporu dosyaya kaydet
        bool saveReport(int tripId, const char* filepath);
    }
}
```

## 6. Güvenlik Entegrasyonu

### 6.1 Şifreleme Entegrasyonu

Tüm veri modelleri dosyaya yazılmadan önce şifrelenir:

```cpp
// Örnek şifreleme akışı
Trip trip = {...};
std::vector<uint8_t> encrypted = encryptData(&trip, sizeof(trip));
writeToFile("trips.dat", encrypted);
```

### 6.2 Güvenli Bellek Yönetimi

Hassas veriler kullanım sonrası güvenli şekilde silinir:

```cpp
void processUserData(User* user) {
    // İşlemler...
    secureMemoryCleanup(user, sizeof(User));
}
```

## 7. Hata Yönetimi

### 7.1 Hata Kodları

```cpp
enum ErrorCode {
    SUCCESS = 0,
    ERROR_FILE_NOT_FOUND = 1,
    ERROR_DECRYPTION_FAILED = 2,
    ERROR_CHECKSUM_MISMATCH = 3,
    ERROR_INVALID_USER = 4,
    ERROR_BUDGET_EXCEEDED = 5,
    // ...
};
```

### 7.2 Exception Handling

```cpp
try {
    Trip trip = TripManager::getTrip(tripId);
} catch (const FileException& e) {
    // Dosya hatası
} catch (const SecurityException& e) {
    // Güvenlik hatası
} catch (const std::exception& e) {
    // Genel hata
}
```

## 8. Performans Gereksinimleri

### 8.1 Performans Hedefleri

- **Dosya I/O:** < 100ms (10KB dosya için)
- **Şifreleme/Şifre Çözme:** < 50ms (1KB veri için)
- **Veri Arama:** < 200ms (1000 kayıt içinde)
- **Rapor Oluşturma:** < 500ms

### 8.2 Optimizasyon Stratejileri

- Veri önbelleğe alma (cache)
- Lazy loading
- Batch işlemler

## 9. Test Stratejisi

### 9.1 Unit Testler

Her modül için ayrı unit testler:
- `userAuth_test.cpp`
- `tripManager_test.cpp`
- `expenseManager_test.cpp`
- `budgetManager_test.cpp`
- `reportGenerator_test.cpp`
- `security_test.cpp`

### 9.2 Integration Testler

Modüller arası entegrasyon testleri:
- Kullanıcı → Seyahat → Gider akışı
- Bütçe kontrolü → Gider kaydı
- Rapor oluşturma akışı

### 9.3 Security Testler

Güvenlik fonksiyonları için testler:
- Şifreleme/şifre çözme doğruluğu
- Güvenli bellek silme
- Checksum doğrulama
- Anti-debug mekanizmaları

---

**Hazırlayan:** [İsim]  
**Tarih:** [Tarih]  
**Versiyon:** 1.0  
**Son Güncelleme:** [Tarih]
