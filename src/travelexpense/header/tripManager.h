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
#include "export.h"
#include <vector>

namespace TravelExpense { // LCOV_EXCL_LINE

    /**
     * @namespace TripManager
     * @brief Seyahat yönetimi modülü
     * 
     * Bu modül, seyahatlerin oluşturulması, güncellenmesi, silinmesi
     * ve listelenmesi işlemlerini yönetir. Kullanıcı bazlı seyahat
     * takibi sağlar.
     */
    namespace TripManager {
        /**
         * @brief Yeni seyahat oluştur
         * 
         * Belirtilen seyahat bilgilerini veritabanına kaydeder ve benzersiz bir tripId
         * oluşturur. Seyahat, kullanıcı ID'si, başlangıç/bitiş tarihi, destinasyon
         * ve açıklama bilgilerini içerir.
         * 
         * @note Bu fonksiyon, yeni bir seyahat kaydı oluşturur.
         * Seyahat ID otomatik olarak oluşturulur ve tripId parametresine yazılır.
         * 
         * @param trip Seyahat bilgileri (const referans, Trip struct'ı)
         *             İçerik: userId, startDate, endDate, destination, description vb.
         *             Tüm alanlar geçerli olmalı (userId > 0, tarih geçerliliği vb.).
         * @param tripId Oluşturulan seyahatin benzersiz ID'si (çıktı parametresi)
         * @return ErrorCode Başarı durumu (Success, InvalidInput, FileNotFound vb.)
         */
        TRAVELEXPENSE_API ErrorCode createTrip(const Trip& trip, int32_t& tripId);

        /**
         * @brief Kullanıcıya ait seyahatleri listele
         * 
         * Belirtilen kullanıcı ID'sine ait tüm seyahatleri veritabanından alır ve
         * trips vektörüne doldurur. Seyahatler tarih sırasına göre sıralanabilir.
         * 
         * @note Bu fonksiyon, trips vektörünü temizler ve yeni seyahatlerle doldurur.
         * Eğer kullanıcı için seyahat bulunamazsa, trips vektörü boş kalır.
         * 
         * @param userId Kullanıcı ID (geçerli bir userId olmalı, 0'dan büyük)
         * @param trips Seyahat listesi (çıktı parametresi, std::vector<Trip>)
         *              Fonksiyon çağrıldığında temizlenir ve yeni seyahatlerle doldurulur.
         * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
         */
        TRAVELEXPENSE_API ErrorCode getTrips(int32_t userId, std::vector<Trip>& trips);

        /**
         * @brief Seyahat bilgisini güncelle
         * 
         * Belirtilen tripId'ye sahip seyahati, yeni seyahat bilgileriyle günceller.
         * Tarih, destinasyon ve açıklama gibi alanlar değiştirilebilir.
         * 
         * @note Bu fonksiyon, mevcut seyahat kaydını günceller. Eğer tripId
         * geçersizse veya seyahat bulunamazsa, FileNotFound hatası döner.
         * 
         * @param tripId Güncellenecek seyahatin benzersiz ID'si (geçerli bir tripId olmalı, 0'dan büyük)
         * @param trip Güncellenmiş seyahat bilgileri (const referans, Trip struct'ı)
         *             İçerik: userId, startDate, endDate, destination, description vb.
         *             Tüm alanlar geçerli olmalı.
         * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
         */
        TRAVELEXPENSE_API ErrorCode updateTrip(int32_t tripId, const Trip& trip);

        /**
         * @brief Seyahati sil
         * 
         * Belirtilen tripId'ye sahip seyahati veritabanından kalıcı olarak siler.
         * Seyahate ait giderler de silinebilir (cascade delete).
         * 
         * @note Bu fonksiyon, seyahati kalıcı olarak siler. Silinen seyahat geri getirilemez.
         * Eğer tripId geçersizse veya seyahat bulunamazsa, FileNotFound hatası döner.
         * 
         * @param tripId Silinecek seyahatin benzersiz ID'si (geçerli bir tripId olmalı, 0'dan büyük)
         * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
         */
        TRAVELEXPENSE_API ErrorCode deleteTrip(int32_t tripId);

        /**
         * @brief Seyahat detayını al
         * 
         * Belirtilen tripId'ye sahip seyahatin detaylı bilgilerini veritabanından alır
         * ve trip parametresine doldurur.
         * 
         * @note Bu fonksiyon, tek bir seyahatin detaylarını getirir.
         * Eğer seyahat bulunamazsa, FileNotFound hatası döner.
         * 
         * @param tripId Seyahat ID (geçerli bir tripId olmalı, 0'dan büyük)
         * @param trip Seyahat bilgisi (çıktı parametresi, Trip struct'ı)
         *             İçerik: tripId, userId, startDate, endDate, destination, description vb.
         * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
         */
        TRAVELEXPENSE_API ErrorCode getTrip(int32_t tripId, Trip& trip);
    }

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // TRIP_MANAGER_H

