/**
 * @file tripManager.h
 * @brief Seyahat Gideri Takibi - Seyahat Yönetimi Modülü
 * 
 * Bu dosya, seyahat yönetimi işlemlerinin fonksiyon bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef TRIP_MANAGER_H
#define TRIP_MANAGER_H

#include "commonTypes.h"
#include <vector>

namespace TravelExpense {

    /**
     * @namespace TripManager
     * @brief Seyahat yönetimi modülü
     */
    namespace TripManager {
        /**
         * @brief Yeni seyahat oluştur
         * @param trip Seyahat bilgileri
         * @param tripId Oluşturulan seyahatin ID'si
         * @return ErrorCode Başarı durumu
         */
        ErrorCode createTrip(const Trip& trip, int32_t& tripId);

        /**
         * @brief Kullanıcıya ait seyahatleri listele
         * @param userId Kullanıcı ID
         * @param trips Seyahat listesi
         * @return ErrorCode Başarı durumu
         */
        ErrorCode getTrips(int32_t userId, std::vector<Trip>& trips);

        /**
         * @brief Seyahat bilgisini güncelle
         * @param tripId Seyahat ID
         * @param trip Güncellenmiş seyahat bilgileri
         * @return ErrorCode Başarı durumu
         */
        ErrorCode updateTrip(int32_t tripId, const Trip& trip);

        /**
         * @brief Seyahati sil
         * @param tripId Seyahat ID
         * @return ErrorCode Başarı durumu
         */
        ErrorCode deleteTrip(int32_t tripId);

        /**
         * @brief Seyahat detayını al
         * @param tripId Seyahat ID
         * @param trip Seyahat bilgisi
         * @return ErrorCode Başarı durumu
         */
        ErrorCode getTrip(int32_t tripId, Trip& trip);
    }

} // namespace TravelExpense

#endif // TRIP_MANAGER_H

