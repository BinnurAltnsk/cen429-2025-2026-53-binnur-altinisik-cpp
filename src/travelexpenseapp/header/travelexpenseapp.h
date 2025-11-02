/**
 * @file travelexpenseapp.h
 * @brief Seyahat Gideri Takibi - Konsol Uygulaması Header Dosyası
 * 
 * Bu dosya, konsol menü sisteminin fonksiyon bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef TRAVELEXPENSEAPP_H
#define TRAVELEXPENSEAPP_H

#include "../travelexpense/header/travelexpense.h"

/**
 * @namespace TravelExpenseApp
 * @brief Konsol uygulaması menü sistemi
 */
namespace TravelExpenseApp {

    /**
     * @brief Ana menüyü göster ve yönet
     * @return int Çıkış kodu
     */
    int showMainMenu();

    /**
     * @brief Kullanıcı işlemleri menüsü
     */
    void showUserMenu();

    /**
     * @brief Seyahat planlama menüsü
     */
    void showTripMenu();

    /**
     * @brief Gider kaydı menüsü
     */
    void showExpenseMenu();

    /**
     * @brief Bütçe yönetimi menüsü
     */
    void showBudgetMenu();

    /**
     * @brief Özet rapor menüsü
     */
    void showReportMenu();

    // ==================== Yardımcı Menü Fonksiyonları ====================

    /**
     * @brief Ekranı temizle
     */
    void clearScreen();

    /**
     * @brief Kullanıcıdan string input al
     * @param prompt Prompt mesajı
     * @param output Alınan string
     * @param maxLength Maksimum uzunluk
     * @return true Başarılı, false İptal
     */
    bool getStringInput(const char* prompt, char* output, size_t maxLength);

    /**
     * @brief Kullanıcıdan integer input al
     * @param prompt Prompt mesajı
     * @param output Alınan integer
     * @return true Başarılı, false İptal
     */
    bool getIntInput(const char* prompt, int& output);

    /**
     * @brief Kullanıcıdan double input al
     * @param prompt Prompt mesajı
     * @param output Alınan double
     * @return true Başarılı, false İptal
     */
    bool getDoubleInput(const char* prompt, double& output);

    /**
     * @brief Hata mesajını göster
     * @param errorCode Hata kodu
     */
    void showError(TravelExpense::ErrorCode errorCode);

    /**
     * @brief Başarı mesajını göster
     * @param message Mesaj
     */
    void showSuccess(const char* message);

    /**
     * @brief Devam etmek için beklet
     */
    void waitForContinue();

} // namespace TravelExpenseApp

#endif // TRAVELEXPENSEAPP_H

