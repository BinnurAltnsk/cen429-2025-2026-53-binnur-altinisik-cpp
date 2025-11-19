/**
 * @file assetManager.cpp
 * @brief Seyahat Gideri Takibi - Varlık Yönetimi Implementation
 *
 * Bu dosya, Varlık Yönetimi gereksinimlerinin implementasyonlarını içerir.
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/assetManager.h"
#include "../header/database.h"
#include "../header/safe_string.h"
#include <sqlite3.h>
#include <cstring>
#include <ctime>
#include <vector>

namespace TravelExpense {

namespace AssetManager {

// ============================================
// VARLIK YÖNETİMİ
// ============================================

ErrorCode registerAsset(const Asset &asset, int32_t &assetId) {
  if (asset.assetId != 0 || asset.name[0] == '\0') {
    return ErrorCode::InvalidInput;
  }

  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                INSERT INTO assets (
                    asset_type, name, description, location, source,
                    size, created_at, deleted_at, default_value,
                    protection_scheme, is_encrypted, is_active
                )
                VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  // Parametreleri bağla
  time_t now = time(nullptr);
  sqlite3_bind_int(stmt, 1, static_cast<int>(asset.assetType));
  sqlite3_bind_text(stmt, 2, asset.name, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, asset.description, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, asset.location, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, asset.source, -1, SQLITE_STATIC);
  sqlite3_bind_int64(stmt, 6, asset.size);
  sqlite3_bind_int64(stmt, 7, static_cast<sqlite3_int64>(now));
  sqlite3_bind_int64(stmt, 8, asset.deletedAt > 0 ? static_cast<sqlite3_int64>(asset.deletedAt) : 0);
  sqlite3_bind_text(stmt, 9, asset.defaultValue, -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 10, asset.protectionScheme);
  sqlite3_bind_int(stmt, 11, asset.isEncrypted ? 1 : 0);
  sqlite3_bind_int(stmt, 12, asset.isActive ? 1 : 0);
  // Sorguyu çalıştır
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return ErrorCode::FileIO;
  }

  assetId = static_cast<int32_t>(sqlite3_last_insert_rowid(db));
  sqlite3_finalize(stmt);
  return ErrorCode::Success;
}

ErrorCode getAsset(int32_t assetId, Asset &asset) {
  if (assetId <= 0) {
    return ErrorCode::InvalidInput;
  }

  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                SELECT asset_type, name, description, location, source,
                       size, created_at, deleted_at, default_value,
                       protection_scheme, is_encrypted, is_active
                FROM assets
                WHERE asset_id = ? AND deleted_at = 0;
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  sqlite3_bind_int(stmt, 1, assetId);
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return ErrorCode::FileNotFound;
  }

  // Sonuçları asset yapısına kopyala
  asset.assetId = assetId;
  asset.assetType = static_cast<AssetType>(sqlite3_column_int(stmt, 0));
  const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
  const char *desc = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
  const char *loc = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
  const char *src = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
  SafeString::safeCopy(asset.name, sizeof(asset.name), name ? name : "");
  SafeString::safeCopy(asset.description, sizeof(asset.description), desc ? desc : "");
  SafeString::safeCopy(asset.location, sizeof(asset.location), loc ? loc : "");
  SafeString::safeCopy(asset.source, sizeof(asset.source), src ? src : "");
  asset.size = sqlite3_column_int64(stmt, 5);
  asset.createdAt = static_cast<time_t>(sqlite3_column_int64(stmt, 6));
  asset.deletedAt = static_cast<time_t>(sqlite3_column_int64(stmt, 7));
  const char *defVal = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 8));
  SafeString::safeCopy(asset.defaultValue, sizeof(asset.defaultValue), defVal ? defVal : "");
  asset.protectionScheme = static_cast<uint8_t>(sqlite3_column_int(stmt, 9));
  asset.isEncrypted = sqlite3_column_int(stmt, 10) != 0;
  asset.isActive = sqlite3_column_int(stmt, 11) != 0;
  sqlite3_finalize(stmt);
  return ErrorCode::Success;
}

ErrorCode updateAsset(int32_t assetId, const Asset &asset) {
  if (assetId <= 0) {
    return ErrorCode::InvalidInput;
  }

  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                UPDATE assets
                SET asset_type = ?, name = ?, description = ?, location = ?, source = ?,
                    size = ?, default_value = ?, protection_scheme = ?,
                    is_encrypted = ?, is_active = ?
                WHERE asset_id = ? AND deleted_at = 0;
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  // Parametreleri bağla
  sqlite3_bind_int(stmt, 1, static_cast<int>(asset.assetType));
  sqlite3_bind_text(stmt, 2, asset.name, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, asset.description, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, asset.location, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, asset.source, -1, SQLITE_STATIC);
  sqlite3_bind_int64(stmt, 6, asset.size);
  sqlite3_bind_text(stmt, 7, asset.defaultValue, -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 8, asset.protectionScheme);
  sqlite3_bind_int(stmt, 9, asset.isEncrypted ? 1 : 0);
  sqlite3_bind_int(stmt, 10, asset.isActive ? 1 : 0);
  sqlite3_bind_int(stmt, 11, assetId);
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    return ErrorCode::FileIO;
  }

  return ErrorCode::Success;
}

ErrorCode deleteAsset(int32_t assetId) {
  if (assetId <= 0) {
    return ErrorCode::InvalidInput;
  }

  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // Soft delete: deleted_at zamanını güncelle
  const char *sql = R"(
                UPDATE assets
                SET deleted_at = ?, is_active = 0
                WHERE asset_id = ? AND deleted_at = 0;
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  time_t now = time(nullptr);
  sqlite3_bind_int64(stmt, 1, static_cast<sqlite3_int64>(now));
  sqlite3_bind_int(stmt, 2, assetId);
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    return ErrorCode::FileIO;
  }

  return ErrorCode::Success;
}

ErrorCode listAssets(std::vector<Asset> &assets) {
  assets.clear();
  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                SELECT asset_id, asset_type, name, description, location, source,
                       size, created_at, deleted_at, default_value,
                       protection_scheme, is_encrypted, is_active
                FROM assets
                WHERE deleted_at = 0
                ORDER BY created_at DESC;
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Asset asset;
    asset.assetId = sqlite3_column_int(stmt, 0);
    asset.assetType = static_cast<AssetType>(sqlite3_column_int(stmt, 1));
    const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    const char *desc = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
    const char *loc = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    const char *src = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
    SafeString::safeCopy(asset.name, sizeof(asset.name), name ? name : "");
    SafeString::safeCopy(asset.description, sizeof(asset.description), desc ? desc : "");
    SafeString::safeCopy(asset.location, sizeof(asset.location), loc ? loc : "");
    SafeString::safeCopy(asset.source, sizeof(asset.source), src ? src : "");
    asset.size = sqlite3_column_int64(stmt, 6);
    asset.createdAt = static_cast<time_t>(sqlite3_column_int64(stmt, 7));
    asset.deletedAt = static_cast<time_t>(sqlite3_column_int64(stmt, 8));
    const char *defVal = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 9));
    SafeString::safeCopy(asset.defaultValue, sizeof(asset.defaultValue), defVal ? defVal : "");
    asset.protectionScheme = static_cast<uint8_t>(sqlite3_column_int(stmt, 10));
    asset.isEncrypted = sqlite3_column_int(stmt, 11) != 0;
    asset.isActive = sqlite3_column_int(stmt, 12) != 0;
    assets.push_back(asset);
  }

  sqlite3_finalize(stmt);
  return ErrorCode::Success;
}

ErrorCode listAssetsByType(AssetType assetType, std::vector<Asset> &assets) {
  assets.clear();
  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                SELECT asset_id, asset_type, name, description, location, source,
                       size, created_at, deleted_at, default_value,
                       protection_scheme, is_encrypted, is_active
                FROM assets
                WHERE asset_type = ? AND deleted_at = 0
                ORDER BY created_at DESC;
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  sqlite3_bind_int(stmt, 1, static_cast<int>(assetType));

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Asset asset;
    asset.assetId = sqlite3_column_int(stmt, 0);
    asset.assetType = static_cast<AssetType>(sqlite3_column_int(stmt, 1));
    const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    const char *desc = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
    const char *loc = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    const char *src = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
    SafeString::safeCopy(asset.name, sizeof(asset.name), name ? name : "");
    SafeString::safeCopy(asset.description, sizeof(asset.description), desc ? desc : "");
    SafeString::safeCopy(asset.location, sizeof(asset.location), loc ? loc : "");
    SafeString::safeCopy(asset.source, sizeof(asset.source), src ? src : "");
    asset.size = sqlite3_column_int64(stmt, 6);
    asset.createdAt = static_cast<time_t>(sqlite3_column_int64(stmt, 7));
    asset.deletedAt = static_cast<time_t>(sqlite3_column_int64(stmt, 8));
    const char *defVal = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 9));
    SafeString::safeCopy(asset.defaultValue, sizeof(asset.defaultValue), defVal ? defVal : "");
    asset.protectionScheme = static_cast<uint8_t>(sqlite3_column_int(stmt, 10));
    asset.isEncrypted = sqlite3_column_int(stmt, 11) != 0;
    asset.isActive = sqlite3_column_int(stmt, 12) != 0;
    assets.push_back(asset);
  }

  sqlite3_finalize(stmt);
  return ErrorCode::Success;
}

// ============================================
// VARLIK KORUMA ŞEMALARI
// ============================================

ErrorCode setProtectionScheme(int32_t assetId, uint8_t protectionScheme) {
  if (assetId <= 0) {
    return ErrorCode::InvalidInput;
  }

  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                UPDATE assets
                SET protection_scheme = ?
                WHERE asset_id = ? AND deleted_at = 0;
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  sqlite3_bind_int(stmt, 1, protectionScheme);
  sqlite3_bind_int(stmt, 2, assetId);
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    return ErrorCode::FileIO;
  }

  return ErrorCode::Success;
}

ErrorCode getProtectionScheme(int32_t assetId, uint8_t &protectionScheme) {
  if (assetId <= 0) {
    return ErrorCode::InvalidInput;
  }

  // SQLite veritabanını al
  sqlite3 *db = Database::getDatabase();

  if (!db) {
    return ErrorCode::FileNotFound;
  }

  // SQL sorgusu hazırla
  const char *sql = R"(
                SELECT protection_scheme
                FROM assets
                WHERE asset_id = ? AND deleted_at = 0;
            )";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    return ErrorCode::FileIO;
  }

  sqlite3_bind_int(stmt, 1, assetId);
  rc = sqlite3_step(stmt);

  if (rc != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return ErrorCode::FileNotFound;
  }

  protectionScheme = static_cast<uint8_t>(sqlite3_column_int(stmt, 0));
  sqlite3_finalize(stmt);
  return ErrorCode::Success;
}

bool verifyProtectionScheme(int32_t assetId, uint8_t requiredScheme) {
  uint8_t currentScheme = 0;

  if (getProtectionScheme(assetId, currentScheme) != ErrorCode::Success) {
    return false;
  }

  // Gerekli koruma şemasının tüm bitlerinin mevcut olup olmadığını kontrol et
  return (currentScheme & requiredScheme) == requiredScheme;
}

} // namespace AssetManager

} // namespace TravelExpense
