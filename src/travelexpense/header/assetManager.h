/**
 * @file assetManager.h
 * @brief Seyahat Gideri Takibi - Varlık Yönetimi
 * 
 * Bu dosya, Varlık Yönetimi gereksinimini karşılar.
 * Her varlık için aşağıdaki özellikler belirlenmeli ve dokümante edilmelidir:
 * - Adı
 * - Açıklaması
 * - Konumu (ör. veritabanı, tablo, sütun)
 * - Kaynağı
 * - Boyutu
 * - Oluşturulma Zamanı
 * - Silinme Zamanı
 * - Varsayılan Değeri
 * - Varlık Koruma Şeması (Gizlilik, Bütünlük, Kimlik Doğrulama)
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "commonTypes.h"
#include "export.h"
#include <cstdint>
#include <cstddef>
#include <ctime>
#include <cstring>
#include <vector>

namespace TravelExpense {

    /**
     * @namespace AssetManager
     * @brief Varlık yönetimi modülü
     * 
     * Bu modül, uygulamadaki varlıkların (veriler, anahtarlar, konfigürasyonlar)
     * kayıt, güncelleme, silme ve listeleme işlemlerini yönetir. Her varlık için
     * ad, açıklama, konum, kaynak, boyut, koruma şeması gibi özellikler takip edilir.
     * Varlık yönetimi gereksinimlerini karşılar.
     */
    namespace AssetManager {

        /**
         * @enum AssetType
         * @brief Varlık tipleri
         */
        enum class AssetType {
            USER_DATA = 0,        /**< @brief Kullanıcı verileri */
            TRIP_DATA = 1,        /**< @brief Seyahat verileri */
            EXPENSE_DATA = 2,     /**< @brief Gider verileri */
            BUDGET_DATA = 3,      /**< @brief Bütçe verileri */
            SESSION_DATA = 4,     /**< @brief Oturum verileri */
            ENCRYPTION_KEY = 5,   /**< @brief Şifreleme anahtarları */
            CONFIG_DATA = 6       /**< @brief Yapılandırma verileri */
        };

        /**
         * @enum ProtectionScheme
         * @brief Varlık koruma şemaları
         */
        enum class ProtectionScheme {
            CONFIDENTIALITY = 0x01,    /**< @brief Gizlilik */
            INTEGRITY = 0x02,          /**< @brief Bütünlük */
            AUTHENTICATION = 0x04      /**< @brief Kimlik Doğrulama */
        };

        /**
         * @struct Asset
         * @brief Varlık veri modeli
         */
        struct Asset {
            int32_t assetId;              /**< @brief Varlık kimliği */
            AssetType assetType;          /**< @brief Varlık tipi */
            char name[100];               /**< @brief Varlık adı */
            char description[500];        /**< @brief Varlık açıklaması */
            char location[200];           /**< @brief Konum (veritabanı, tablo, sütun) */
            char source[200];             /**< @brief Kaynak */
            int64_t size;                 /**< @brief Boyut (byte) */
            time_t createdAt;             /**< @brief Oluşturulma zamanı */
            time_t deletedAt;             /**< @brief Silinme zamanı (0 = silinmemiş) */
            char defaultValue[500];       /**< @brief Varsayılan değer */
            uint8_t protectionScheme;     /**< @brief Koruma şeması (bit flags) */
            bool isEncrypted;             /**< @brief Şifrelenmiş mi? */
            bool isActive;                /**< @brief Aktif mi? */

            /**
             * @brief Asset constructor
             * 
             * Varsayılan değerlerle yeni bir Asset oluşturur.
             */
            Asset() : assetId(0), assetType(AssetType::USER_DATA),
                     size(0), createdAt(0), deletedAt(0),
                     protectionScheme(0), isEncrypted(false), isActive(true) {
                memset(name, 0, sizeof(name));
                memset(description, 0, sizeof(description));
                memset(location, 0, sizeof(location));
                memset(source, 0, sizeof(source));
                memset(defaultValue, 0, sizeof(defaultValue));
            }
        };

        // ============================================
        // VARLIK YÖNETİMİ
        // ============================================

        /**
         * @brief Varlık kaydet
         * 
         * @param asset Varlık bilgileri
         * @param assetId Oluşturulan varlık ID'si (çıktı)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode registerAsset(const Asset& asset, int32_t& assetId);

        /**
         * @brief Varlık bilgilerini al
         * 
         * @param assetId Varlık ID
         * @param asset Varlık bilgileri (çıktı)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode getAsset(int32_t assetId, Asset& asset);

        /**
         * @brief Varlık bilgilerini güncelle
         * 
         * @param assetId Varlık ID
         * @param asset Güncellenmiş varlık bilgileri
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode updateAsset(int32_t assetId, const Asset& asset);

        /**
         * @brief Varlık sil (soft delete)
         * 
         * @param assetId Varlık ID
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode deleteAsset(int32_t assetId);

        /**
         * @brief Tüm varlıkları listele
         * 
         * @param assets Varlık listesi (çıktı)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode listAssets(std::vector<Asset>& assets);

        /**
         * @brief Tipe göre varlıkları listele
         * 
         * @param assetType Varlık tipi
         * @param assets Varlık listesi (çıktı)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode listAssetsByType(AssetType assetType, std::vector<Asset>& assets);

        // ============================================
        // VARLIK KORUMA ŞEMALARI
        // ============================================

        /**
         * @brief Varlık koruma şemasını ayarla
         * 
         * @param assetId Varlık ID
         * @param protectionScheme Koruma şeması (bit flags)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode setProtectionScheme(int32_t assetId, uint8_t protectionScheme);

        /**
         * @brief Varlık koruma şemasını al
         * 
         * @param assetId Varlık ID
         * @param protectionScheme Koruma şeması (çıktı)
         * @return ErrorCode Başarı durumu
         */
        TRAVELEXPENSE_API ErrorCode getProtectionScheme(int32_t assetId, uint8_t& protectionScheme);

        /**
         * @brief Varlık koruma şeması doğrula
         * 
         * @param assetId Varlık ID
         * @param requiredScheme Gerekli koruma şeması
         * @return true Koruma şeması uygun, false Uygun değil
         */
        TRAVELEXPENSE_API bool verifyProtectionScheme(int32_t assetId, uint8_t requiredScheme);

    } // namespace AssetManager

} // namespace TravelExpense

#endif // ASSET_MANAGER_H

