# Binary → SQLite Migration Kullanım Kılavuzu

## Genel Bakış

Bu migration modülü, binary dosya formatındaki verileri SQLite veritabanına aktarmak için kullanılır. Bu özellik, projenin binary dosya formatından SQLite formatına geçiş yapmasına yardımcı olur.

## Dosya Yapısı

### Header Dosyası
- `src/travelexpense/header/migration.h` - Migration fonksiyon bildirimleri

### Implementation Dosyası
- `src/travelexpense/src/migration.cpp` - Migration fonksiyon implementasyonları

## Binary Dosya Formatı

Binary dosyalar `data/` dizininde `.dat` uzantılı dosyalar olarak kaydedilir:

- `data/users.dat` - User struct'ları (binary format)
- `data/trips.dat` - Trip struct'ları (binary format)
- `data/expenses.dat` - Expense struct'ları (binary format)
- `data/budgets.dat` - Budget struct'ları (binary format)

Her dosya, ilgili struct'ların ardışık olarak binary formatında kaydedilmiş halidir.

## Kullanım

### 1. Migration Gerekliliğini Kontrol Etme

```cpp
#include "travelexpense.h"

using namespace TravelExpense;

bool needMigration = Migration::isMigrationNeeded("data");
if (needMigration) {
    // Migration yapılmalı
}
```

### 2. Tek Bir Entity Migration

```cpp
// Sadece Users migration
std::vector<User> users;
if (Migration::readUsersFromBinary("data/users.dat", users)) {
    int32_t migrated = 0;
    ErrorCode rc = Migration::migrateUsersToDatabase(users, migrated);
    if (rc == ErrorCode::SUCCESS) {
        printf("Migrated %d users\n", migrated);
    }
}
```

### 3. Tüm Binary Dosyaları Migration Etme

```cpp
#include "travelexpense.h"

using namespace TravelExpense;

// Veritabanını başlat
sqlite3* db = Database::initializeDatabase();
if (!db) {
    // Hata
    return;
}

Database::createTables(db);

// Migration yap
Migration::MigrationResult result;
ErrorCode rc = Migration::migrateAllFromBinary("data", result);

if (rc == ErrorCode::SUCCESS && result.success) {
    printf("Migration tamamlandı:\n");
    printf("  Users: %d\n", result.usersMigrated);
    printf("  Trips: %d\n", result.tripsMigrated);
    printf("  Expenses: %d\n", result.expensesMigrated);
    printf("  Budgets: %d\n", result.budgetsMigrated);
} else {
    printf("Migration hataları: %d\n", result.errors);
}

Database::closeDatabase(db);
```

## Migration Özellikleri

### Duplicate Kontrolü
- Her entity için duplicate kontrolü yapılır
- Eğer aynı ID veya unique değer (username gibi) zaten varsa, migration atlanır

### Transaction Desteği
- Her entity migration'ı transaction içinde yapılır
- Hata durumunda rollback yapılır
- Performans için batch insert kullanılır

### ID Yönetimi
- Binary dosyalardaki ID'ler korunur (eğer > 0 ise)
- ID = 0 ise, SQLite AUTOINCREMENT yeni ID üretir
- Duplicate ID'ler atlanır

### Veri Doğrulama
- Binary dosyalardan okunan veriler doğrulanır
- Geçersiz kayıtlar (ID = 0 ve diğer gerekli alanlar boş) atlanır

## Örnek Tam Migration Script

```cpp
#include "travelexpense.h"
#include <iostream>

int main() {
    using namespace TravelExpense;
    
    // Veritabanını başlat
    sqlite3* db = Database::initializeDatabase("data/travelexpense.db");
    if (!db) {
        std::cerr << "Veritabanı başlatılamadı!" << std::endl;
        return 1;
    }
    
    // Tabloları oluştur
    if (Database::createTables(db) != ErrorCode::SUCCESS) {
        std::cerr << "Tablolar oluşturulamadı!" << std::endl;
        Database::closeDatabase(db);
        return 1;
    }
    
    // Migration gerekliliğini kontrol et
    if (!Migration::isMigrationNeeded("data")) {
        std::cout << "Migration gerekli değil (binary dosyalar yok)" << std::endl;
        Database::closeDatabase(db);
        return 0;
    }
    
    std::cout << "Migration başlatılıyor..." << std::endl;
    
    // Migration yap
    Migration::MigrationResult result;
    ErrorCode rc = Migration::migrateAllFromBinary("data", result);
    
    if (rc == ErrorCode::SUCCESS && result.success) {
        std::cout << "\n=== Migration Tamamlandı ===" << std::endl;
        std::cout << "Users: " << result.usersMigrated << " kayıt" << std::endl;
        std::cout << "Trips: " << result.tripsMigrated << " kayıt" << std::endl;
        std::cout << "Expenses: " << result.expensesMigrated << " kayıt" << std::endl;
        std::cout << "Budgets: " << result.budgetsMigrated << " kayıt" << std::endl;
        std::cout << "Hatalar: " << result.errors << std::endl;
    } else {
        std::cerr << "\nMigration başarısız!" << std::endl;
        std::cerr << "Hatalar: " << result.errors << std::endl;
    }
    
    Database::closeDatabase(db);
    return (result.success) ? 0 : 1;
}
```

## Migration Sonrası

### Binary Dosyalar
Migration sonrası binary dosyalar korunur. İsterseniz bunları silebilir veya backup alabilirsiniz:

```bash
# Binary dosyaları backup al
mkdir -p backup
cp data/*.dat backup/

# Veya sil (dikkatli!)
# rm data/*.dat
```

### ID Dosyaları
ID dosyaları (`user.id`, `trip.id`, vb.) migration sonrası gereksiz olabilir çünkü SQLite AUTOINCREMENT kullanıyor. Ancak yine de silmeden önce backup alın.

## Notlar

1. **Transaction Kullanımı:** Her entity migration'ı ayrı transaction'da yapılır. Büyük veri setlerinde performans için optimize edilebilir.

2. **Duplicate Kontrolü:** Migration sırasında duplicate kayıtlar atlanır. Bu, migration'ın birden fazla kez çalıştırılabilmesini sağlar (idempotent).

3. **ID Koruma:** Binary dosyalardaki ID'ler korunur. Bu, foreign key ilişkilerinin korunması için önemlidir.

4. **Hata Yönetimi:** Migration sırasında oluşan hatalar `MigrationResult.errors` sayısında tutulur. Detaylı hata mesajları eklenebilir.

5. **Veri Bütünlüğü:** Foreign key constraint'leri migration sonrası kontrol edilmelidir. SQLite'da foreign keys migration sonrası doğrulanabilir.

## Güvenlik Notları

- Migration işlemi sırasında binary dosyalar okunur ama değiştirilmez
- SQLite veritabanı transaction kullanır, hata durumunda rollback yapılır
- Duplicate kontrolü yapılır, veri kaybı riski düşüktür

---

**Hazırlayan:** Binnur Altınışık  
**Tarih:** 2025

