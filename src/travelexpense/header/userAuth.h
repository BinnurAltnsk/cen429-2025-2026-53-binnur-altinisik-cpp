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
#include "export.h"

namespace TravelExpense {

    /**
     * @namespace UserAuth
     * @brief Kullanıcı kimlik doğrulama modülü
     * 
     * Bu modül, kullanıcı kaydı, giriş, çıkış ve kimlik doğrulama
     * işlemlerini yönetir. Şifre hash'leme ve doğrulama işlemlerini
     * sağlar. Misafir modu desteği sunar.
     */
    namespace UserAuth {
        /**
         * @brief Kullanıcı kaydı
         * 
         * Yeni bir kullanıcı kaydı oluşturur. Şifre hash'lenir ve salt ile birlikte
         * veritabanına kaydedilir. Kullanıcı adı benzersiz olmalıdır.
         * 
         * @note Bu fonksiyon, şifreyi SHA-256 ile hash'ler ve salt ekler.
         * Eğer kullanıcı adı zaten varsa, InvalidInput hatası döner.
         * 
         * @param username Kullanıcı adı (nullptr ise InvalidInput döner, benzersiz olmalı)
         * @param password Şifre (nullptr ise InvalidInput döner, minimum uzunluk kontrolü yapılabilir)
         * @return ErrorCode Başarı durumu (Success, InvalidInput, FileNotFound vb.)
         */
        TRAVELEXPENSE_API ErrorCode registerUser(const char* username, const char* password);

        /**
         * @brief Kullanıcı girişi
         * 
         * Belirtilen kullanıcı adı ve şifre ile giriş yapar. Şifre hash'i doğrulanır
         * ve doğruysa kullanıcı oturumu başlatılır.
         * 
         * @note Bu fonksiyon, şifreyi hash'ler ve veritabanındaki hash ile karşılaştırır.
         * Constant-time comparison kullanarak timing attack'lara karşı koruma sağlar.
         * Başarılı giriş sonrası, getCurrentUser() ile kullanıcı bilgisi alınabilir.
         * 
         * @param username Kullanıcı adı (nullptr ise InvalidInput döner)
         * @param password Şifre (nullptr ise InvalidInput döner)
         * @return ErrorCode Başarı durumu (Success, InvalidInput, InvalidUser vb.)
         */
        TRAVELEXPENSE_API ErrorCode loginUser(const char* username, const char* password);

        /**
         * @brief Misafir modunu etkinleştir
         * 
         * Misafir kullanıcı modunu etkinleştirir. Misafir modunda, kullanıcı kaydı
         * veya girişi gerekmez, ancak bazı özellikler sınırlı olabilir.
         * 
         * @note Bu fonksiyon, geçici bir misafir kullanıcısı oluşturur.
         * Misafir modu, oturum sonlandığında veya logoutUser() çağrıldığında sonlanır.
         * 
         * @return ErrorCode Başarı durumu (Success, InvalidInput vb.)
         */
        TRAVELEXPENSE_API ErrorCode enableGuestMode();

        /**
         * @brief Kullanıcı çıkışı
         * 
         * Mevcut kullanıcı oturumunu sonlandırır. Kullanıcı bilgileri temizlenir
         * ve getCurrentUser() nullptr döner.
         * 
         * @note Bu fonksiyon, misafir modu dahil tüm oturumları sonlandırır.
         * Oturum sonlandıktan sonra, yeni giriş veya misafir modu gerekir.
         */
        TRAVELEXPENSE_API void logoutUser();

        /**
         * @brief Mevcut kullanıcı bilgisini al
         * 
         * Aktif oturumdaki kullanıcı bilgisini döndürür. Eğer oturum yoksa
         * (giriş yapılmamış veya logoutUser() çağrılmış), nullptr döner.
         * 
         * @note Bu fonksiyon, getCurrentUser() ile alınan pointer, logoutUser()
         * çağrılana kadar geçerlidir. Pointer'ı saklamak güvenli değildir.
         * 
         * @return User* Mevcut kullanıcı pointer'ı (nullptr ise giriş yapılmamış veya oturum sonlandırılmış)
         */
        TRAVELEXPENSE_API User* getCurrentUser();

        /**
         * @brief Kullanıcı ID'ye göre kullanıcı bilgisini al
         * 
         * Belirtilen userId'ye sahip kullanıcının bilgilerini veritabanından alır
         * ve user parametresine doldurur.
         * 
         * @note Bu fonksiyon, kullanıcı ID'sine göre kullanıcı bilgilerini getirir.
         * Eğer kullanıcı bulunamazsa, FileNotFound hatası döner.
         * 
         * @param userId Kullanıcı ID (geçerli bir userId olmalı, 0'dan büyük)
         * @param user Kullanıcı bilgisinin yazılacağı struct (çıktı parametresi, User struct'ı)
         *             İçerik: userId, username, email, registrationDate vb.
         * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
         */
        TRAVELEXPENSE_API ErrorCode getUserById(int32_t userId, User& user);
    }

} // namespace TravelExpense

#endif // USER_AUTH_H

