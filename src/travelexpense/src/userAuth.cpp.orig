/**
 * @file userAuth.cpp
 * @brief Seyahat Gideri Takibi - Kullanıcı Kimlik Doğrulama Implementation
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/userAuth.h"
#include "../header/database.h"
#include "../header/encryption.h"
#include "../header/security.h"
#include <sqlite3.h>
#include <cstring>
#include <sstream>
#include <ctime>

namespace TravelExpense {

namespace UserAuth {

// Global mevcut kullanıcı pointer'ı
static User *currentUser = nullptr;

ErrorCode registerUser(const char *username, const char *password) {
  if (!username || !password || strlen(username) == 0 || strlen(password) == 0) {
    return ErrorCode::InvalidInput;
  }

  // Şimdilik basit bir kontrol
  if (strlen(username) >= 50) {
    return ErrorCode::InvalidInput;
  }

  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // Salt oluştur
  char salt[33] = {0};
  char passwordHash[65] = {0};

  if (!Encryption::generateSalt(salt)) {
    return ErrorCode::MemoryAllocation;
  }

  // Şifreyi hash'le (SHA-256 + Salt)
  if (!Encryption::hashPassword(password, salt, passwordHash)) {
    // Salt'ı güvenli şekilde sil
    Security::secureCleanup(salt, sizeof(salt));
    Security::secureCleanup(passwordHash, sizeof(passwordHash));
    return ErrorCode::EncryptionFailed;
  }

  // Hash ve salt uzunluklarını kontrol et (kaydetmeden önce)
  size_t hashLen = strlen(passwordHash);
  size_t saltLen = strlen(salt);

  if (hashLen != 64 || saltLen != 32) {
    Security::secureCleanup(salt, sizeof(salt));
    Security::secureCleanup(passwordHash, sizeof(passwordHash));
    return ErrorCode::EncryptionFailed;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                INSERT INTO users (username, password_hash, salt, is_guest, created_at, last_login)
                VALUES (?, ?, ?, 0, ?, 0);
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    Security::secureCleanup(salt, sizeof(salt));
    Security::secureCleanup(passwordHash, sizeof(passwordHash));
    return ErrorCode::FileIO;
  }

  // Parametreleri bağla
  time_t createdAt = time(nullptr);
  sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, passwordHash, -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, salt, -1, SQLITE_TRANSIENT);
  sqlite3_bind_int64(stmt, 4, static_cast<sqlite3_int64>(createdAt));
  // Sorguyu çalıştır
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    Security::secureCleanup(salt, sizeof(salt));
    Security::secureCleanup(passwordHash, sizeof(passwordHash));

    // UNIQUE constraint hatası (kullanıcı zaten var)
    if (rc == SQLITE_CONSTRAINT) {
      return ErrorCode::InvalidUser;
    }

    return ErrorCode::FileIO;
  }

  sqlite3_finalize(stmt);
  // Verilerin commit edildiğinden emin ol
  // SQLite'da autocommit mode varsayılan olarak açıktır,
  // ama bazı durumlarda açıkça commit etmek gerekebilir
  sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
  // Güvenli temizlik
  Security::secureCleanup(salt, sizeof(salt));
  Security::secureCleanup(passwordHash, sizeof(passwordHash));
  return ErrorCode::Success;
}

ErrorCode loginUser(const char *username, const char *password) {
  if (!username || !password || strlen(username) == 0 || strlen(password) == 0) {
    return ErrorCode::InvalidInput;
  }

  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = "SELECT user_id, username, password_hash, salt, is_guest, created_at, last_login FROM users WHERE username = ?;";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  // Parametreleri bağla
  rc = sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

  if (rc != SQLITE_OK) {
    sqlite3_finalize(stmt);
    return ErrorCode::FileIO;
  }

  // Sorguyu çalıştır
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_ROW) {
    // Eğer kullanıcı bulunamadıysa, hatayı döndür
    sqlite3_finalize(stmt);
    return ErrorCode::InvalidUser;
  }

  // Sonuçları al
  User foundUser;
  foundUser.userId = sqlite3_column_int(stmt, 0);

  // Veritabanından okunan değerlerin null olup olmadığını kontrol et
  if (sqlite3_column_type(stmt, 1) == SQLITE_NULL ||
      sqlite3_column_type(stmt, 2) == SQLITE_NULL ||
      sqlite3_column_type(stmt, 3) == SQLITE_NULL) {
    sqlite3_finalize(stmt);
    return ErrorCode::InvalidUser;
  }

  const char *dbUsername = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
  const char *storedHash = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
  const char *storedSalt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

  // Null pointer kontrolü
  if (!dbUsername || !storedHash || !storedSalt) {
    sqlite3_finalize(stmt);
    return ErrorCode::InvalidUser;
  }

  foundUser.isGuest = (sqlite3_column_int(stmt, 4) != 0);
  foundUser.createdAt = sqlite3_column_int64(stmt, 5);
  foundUser.lastLogin = sqlite3_column_int64(stmt, 6);
  strncpy(foundUser.username, dbUsername, sizeof(foundUser.username) - 1);
  foundUser.username[sizeof(foundUser.username) - 1] = '\0';
  // Hash ve salt'ı kopyala (hash 64 karakter, salt 32 karakter)
  // strncpy kullanarak tam uzunluğu kopyala, sonra null terminator ekle
  size_t hashLen = strlen(storedHash);
  size_t saltLen = strlen(storedSalt);

  // Hash ve salt uzunluklarını kontrol et
  if (hashLen != 64 || saltLen != 32) {
    sqlite3_finalize(stmt);
    return ErrorCode::InvalidUser;
  }

  // Hash ve salt'ı tam uzunlukta kopyala
  // passwordHash[64] buffer'ı var (0-63 arası indeksler = 64 byte)
  // Hash 64 karakter, salt 32 karakter
  // Buffer'ı tam kullanıp null terminator eklememek doğru
  // verifyPassword fonksiyonunda strlen yerine sabit 64 kullanacağız
  memcpy(foundUser.passwordHash, storedHash, 64);
  memcpy(foundUser.salt, storedSalt, 32);
  // Null terminator eklemiyoruz çünkü buffer tam dolu
  // verifyPassword fonksiyonunda strlen yerine sabit 64 kullanılacak
  sqlite3_finalize(stmt);

  // Şifre doğrulama (SHA-256 hash kontrolü)
  // Hash ve salt uzunlukları zaten kontrol edildi ve doğru kopyalandı
  if (!Encryption::verifyPassword(password, foundUser.salt, foundUser.passwordHash)) {
    // Güvenli temizlik
    Security::secureMemoryCleanup(&foundUser, sizeof(User));
    return ErrorCode::InvalidUser;
  }

  // Last login'i güncelle
  time_t now = time(nullptr);
  const char *updateSql = "UPDATE users SET last_login = ? WHERE user_id = ?;";
  sqlite3_stmt *updateStmt = nullptr;
  rc = sqlite3_prepare_v2(db, updateSql, -1, &updateStmt, nullptr);

  if (rc == SQLITE_OK) {
    sqlite3_bind_int64(updateStmt, 1, static_cast<sqlite3_int64>(now));
    sqlite3_bind_int(updateStmt, 2, foundUser.userId);
    sqlite3_step(updateStmt);
    sqlite3_finalize(updateStmt);
    foundUser.lastLogin = now;
  }

  // Mevcut kullanıcıyı ayarla
  if (currentUser != nullptr) {
    Security::secureMemoryCleanup(currentUser, sizeof(User));
    delete currentUser;
  }

  currentUser = new User(foundUser);
  // Geçici değişkeni güvenli şekilde temizle
  Security::secureMemoryCleanup(&foundUser, sizeof(User));
  return ErrorCode::Success;
}

ErrorCode enableGuestMode() {
  if (currentUser != nullptr) {
    delete currentUser;
  }

  currentUser = new User();
  currentUser->userId = -1; // Guest kullanıcı ID'si
  currentUser->isGuest = true;
  strncpy(currentUser->username, "Guest", sizeof(currentUser->username) - 1);
  currentUser->createdAt = time(nullptr);
  return ErrorCode::Success;
}

void logoutUser() {
  if (currentUser != nullptr) {
    delete currentUser;
    currentUser = nullptr;
  }
}

User *getCurrentUser() {
  return currentUser;
}

ErrorCode getUserById(int32_t userId, User &user) {
  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = "SELECT user_id, username, password_hash, salt, is_guest, created_at, last_login FROM users WHERE user_id = ?;";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  // Parametreleri bağla
  sqlite3_bind_int(stmt, 1, userId);
  // Sorguyu çalıştır
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return ErrorCode::InvalidUser;
  }

  // Sonuçları al
  user.userId = sqlite3_column_int(stmt, 0);
  const char *dbUsername = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
  const char *storedHash = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
  const char *storedSalt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
  user.isGuest = (sqlite3_column_int(stmt, 4) != 0);
  user.createdAt = sqlite3_column_int64(stmt, 5);
  user.lastLogin = sqlite3_column_int64(stmt, 6);
  strncpy(user.username, dbUsername, sizeof(user.username) - 1);
  user.username[sizeof(user.username) - 1] = '\0';
  strncpy(user.passwordHash, storedHash, sizeof(user.passwordHash) - 1);
  user.passwordHash[sizeof(user.passwordHash) - 1] = '\0';
  strncpy(user.salt, storedSalt, sizeof(user.salt) - 1);
  user.salt[sizeof(user.salt) - 1] = '\0';
  sqlite3_finalize(stmt);
  return ErrorCode::Success;
}
}

} // namespace TravelExpense
