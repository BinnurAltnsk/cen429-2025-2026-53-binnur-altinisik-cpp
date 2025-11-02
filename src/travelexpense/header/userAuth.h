/**
 * @file userAuth.h
 * @brief Seyahat Gideri Takibi - Kullanıcı Kimlik Doğrulama Modülü
 * 
 * Bu dosya, kullanıcı kimlik doğrulama işlemlerinin fonksiyon bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef USER_AUTH_H
#define USER_AUTH_H

#include "commonTypes.h"

namespace TravelExpense {

    /**
     * @namespace UserAuth
     * @brief Kullanıcı kimlik doğrulama modülü
     */
    namespace UserAuth {
        /**
         * @brief Kullanıcı kaydı
         * @param username Kullanıcı adı
         * @param password Şifre
         * @return ErrorCode Başarı durumu
         */
        ErrorCode registerUser(const char* username, const char* password);

        /**
         * @brief Kullanıcı girişi
         * @param username Kullanıcı adı
         * @param password Şifre
         * @return ErrorCode Başarı durumu
         */
        ErrorCode loginUser(const char* username, const char* password);

        /**
         * @brief Misafir modunu etkinleştir
         * @return ErrorCode Başarı durumu
         */
        ErrorCode enableGuestMode();

        /**
         * @brief Kullanıcı çıkışı
         */
        void logoutUser();

        /**
         * @brief Mevcut kullanıcı bilgisini al
         * @return User* Mevcut kullanıcı pointer'ı (nullptr ise giriş yapılmamış)
         */
        User* getCurrentUser();

        /**
         * @brief Kullanıcı ID'ye göre kullanıcı bilgisini al
         * @param userId Kullanıcı ID
         * @param user Kullanıcı bilgisinin yazılacağı struct
         * @return ErrorCode Başarı durumu
         */
        ErrorCode getUserById(int32_t userId, User& user);
    }

} // namespace TravelExpense

#endif // USER_AUTH_H

