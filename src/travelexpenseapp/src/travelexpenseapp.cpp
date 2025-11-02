/**
 * @file travelexpenseapp.cpp
 * @brief Seyahat Gideri Takibi - Konsol Uygulaması Main
 * 
 * Bu dosya, konsol menü sisteminin implementasyonunu ve main fonksiyonunu içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/travelexpenseapp.h"
#include <sqlite3.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstring>
#include <cstdio>
#include <algorithm>

#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX  // Prevent Windows.h from defining min/max macros
    #endif
    #include <windows.h>
    #include <io.h>
    #include <fcntl.h>
    
    // Undefine Windows ERROR_* macros that conflict with our ErrorCode enum class
    // Windows.h defines many ERROR_* macros that conflict with our enum class values
    #ifdef ERROR_FILE_NOT_FOUND
        #undef ERROR_FILE_NOT_FOUND
    #endif
    #ifdef ERROR_DECRYPTION_FAILED
        #undef ERROR_DECRYPTION_FAILED
    #endif
    #ifdef ERROR_CHECKSUM_MISMATCH
        #undef ERROR_CHECKSUM_MISMATCH
    #endif
    #ifdef ERROR_INVALID_USER
        #undef ERROR_INVALID_USER
    #endif
    #ifdef ERROR_BUDGET_EXCEEDED
        #undef ERROR_BUDGET_EXCEEDED
    #endif
    #ifdef ERROR_INVALID_INPUT
        #undef ERROR_INVALID_INPUT
    #endif
    #ifdef ERROR_MEMORY_ALLOCATION
        #undef ERROR_MEMORY_ALLOCATION
    #endif
    #ifdef ERROR_FILE_IO
        #undef ERROR_FILE_IO
    #endif
    #ifdef ERROR_ENCRYPTION_FAILED
        #undef ERROR_ENCRYPTION_FAILED
    #endif
    
    #define CLEAR_SCREEN() system("cls")
#else
    #include <cstdlib>
    #define CLEAR_SCREEN() system("clear")
#endif


using namespace TravelExpenseApp;

namespace TravelExpenseApp {

    // Import all types and namespaces from TravelExpense
    using namespace TravelExpense;
    
    // Explicit type imports for better IntelliSense support
    using TravelExpense::ErrorCode;
    using TravelExpense::User;
    using TravelExpense::Trip;
    using TravelExpense::Expense;
    using TravelExpense::Budget;
    using TravelExpense::ExpenseCategory;

    // ==================== Yardımcı Fonksiyonlar ====================

    void clearScreen() {
        CLEAR_SCREEN();
    }

    void printHeader(const char* title) {
        std::cout << "\n";
        std::cout << "========================================\n";
        std::cout << "  " << title << "\n";
        std::cout << "========================================\n\n";
    }

    void printSeparator() {
        std::cout << "----------------------------------------\n";
    }

    bool getStringInput(const char* prompt, char* output, size_t maxLength) {
        if (!prompt || !output || maxLength == 0) {
            return false;
        }

        std::cout << prompt;
        std::cin.getline(output, maxLength);
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            return false;
        }

        // Boş string kontrolü
        if (strlen(output) == 0) {
            return false;
        }

        return true;
    }

    bool getIntInput(const char* prompt, int& output) {
        if (!prompt) {
            return false;
        }

        std::cout << prompt;
        std::cin >> output;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            return false;
        }

        std::cin.ignore(); // \n karakterini temizle
        return true;
    }

    bool getDoubleInput(const char* prompt, double& output) {
        if (!prompt) {
            return false;
        }

        std::cout << prompt;
        std::cin >> output;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            return false;
        }

        std::cin.ignore(); // \n karakterini temizle
        return true;
    }

    void showError(ErrorCode errorCode) {
        std::cout << "\n[!] HATA: ";
        
        // Use if-else with full namespace path for better MSVC compatibility
        if (errorCode == TravelExpense::ErrorCode::ERROR_FILE_NOT_FOUND) {
            std::cout << "Dosya veya kaynak bulunamadı.\n";
        } else if (errorCode == TravelExpense::ErrorCode::ERROR_DECRYPTION_FAILED) {
            std::cout << "Şifre çözme işlemi başarısız oldu.\n";
        } else if (errorCode == TravelExpense::ErrorCode::ERROR_CHECKSUM_MISMATCH) {
            std::cout << "Veri bütünlüğü hatası tespit edildi.\n";
        } else if (errorCode == TravelExpense::ErrorCode::ERROR_INVALID_USER) {
            std::cout << "Geçersiz kullanıcı veya şifre.\n";
        } else if (errorCode == TravelExpense::ErrorCode::ERROR_BUDGET_EXCEEDED) {
            std::cout << "Bütçe limiti aşıldı!\n";
        } else if (errorCode == TravelExpense::ErrorCode::ERROR_INVALID_INPUT) {
            std::cout << "Geçersiz veya eksik girdi.\n";
        } else if (errorCode == TravelExpense::ErrorCode::ERROR_MEMORY_ALLOCATION) {
            std::cout << "Bellek ayırma hatası.\n";
        } else if (errorCode == TravelExpense::ErrorCode::ERROR_FILE_IO) {
            std::cout << "Dosya okuma/yazma hatası.\n";
        } else if (errorCode == TravelExpense::ErrorCode::ERROR_ENCRYPTION_FAILED) {
            std::cout << "Şifreleme işlemi başarısız oldu.\n";
        } else {
            std::cout << "Bilinmeyen bir hata oluştu.\n";
        }
    }

    void showSuccess(const char* message) {
        std::cout << "\n[+] " << message << "\n";
    }

    void showInfo(const char* message) {
        std::cout << "\n[i] " << message << "\n";
    }

    void waitForContinue() {
        std::cout << "\nDevam etmek için Enter'a basın...";
        std::cin.get();
    }

    bool requireLogin() {
        User* currentUser = UserAuth::getCurrentUser();
        if (!currentUser || currentUser->userId <= 0) {
            std::cout << "\n[!] Bu işlem için önce giriş yapmalısınız!\n";
            waitForContinue();
            return false;
        }
        return true;
    }

    // ==================== Kullanıcı İşlemleri Menüsü ====================

    void showUserMenu() {
        int choice;
        char username[50];
        char password[100];

        do {
            clearScreen();
            printHeader("KULLANICI İŞLEMLERİ");

            User* currentUser = UserAuth::getCurrentUser();
            if (currentUser && currentUser->userId > 0) {
                std::cout << "Mevcut Kullanıcı: " << currentUser->username;
                if (currentUser->isGuest) {
                    std::cout << " (Misafir)";
                }
                std::cout << "\n";
                printSeparator();
            }

            std::cout << "1. Giriş Yap\n";
            std::cout << "2. Yeni Hesap Oluştur\n";
            std::cout << "3. Misafir Modu (Giriş Yapmadan Devam Et)\n";
            if (currentUser && currentUser->userId > 0) {
                std::cout << "4. Çıkış Yap\n";
            }
            std::cout << "0. Ana Menüye Dön\n\n";
            std::cout << "Seçiminiz: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    std::cout << "\n";
                    if (!getStringInput("Kullanıcı Adı: ", username, sizeof(username))) {
                        std::cout << "\n[!] Kullanıcı adı boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getStringInput("Şifre: ", password, sizeof(password))) {
                        std::cout << "\n[!] Şifre boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    ErrorCode result = UserAuth::loginUser(username, password);
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        showSuccess("Giriş başarılı! Hoş geldiniz!");
                        waitForContinue();
                        return;
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 2: {
                    std::cout << "\n";
                    if (!getStringInput("Yeni Kullanıcı Adı: ", username, sizeof(username))) {
                        std::cout << "\n[!] Kullanıcı adı boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    if (strlen(username) < 3) {
                        std::cout << "\n[!] Kullanıcı adı en az 3 karakter olmalıdır!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getStringInput("Şifre: ", password, sizeof(password))) {
                        std::cout << "\n[!] Şifre boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    if (strlen(password) < 4) {
                        std::cout << "\n[!] Şifre en az 4 karakter olmalıdır!\n";
                        waitForContinue();
                        break;
                    }

                    ErrorCode result = UserAuth::registerUser(username, password);
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        showSuccess("Hesap başarıyla oluşturuldu! Giriş yapabilirsiniz.");
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 3: {
                    ErrorCode result = UserAuth::enableGuestMode();
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        showSuccess("Misafir modu etkinleştirildi!");
                        waitForContinue();
                        return;
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 4: {
                    if (currentUser && currentUser->userId > 0) {
                        UserAuth::logoutUser();
                        showSuccess("Başarıyla çıkış yapıldı.");
                        waitForContinue();
                        return;
                    }
                    break;
                }
                case 0:
                    return;
                default:
                    std::cout << "\n[!] Geçersiz seçim! Lütfen 0-4 arası bir sayı girin.\n";
                    waitForContinue();
                    break;
            }
        } while (choice != 0);
    }

    // ==================== Seyahat Planlama Menüsü ====================

    void showTripMenu() {
        int choice;

        do {
            clearScreen();
            printHeader("SEYAHAT PLANLAMA");

            if (!requireLogin()) {
                return;
            }

            std::cout << "1. Yeni Seyahat Oluştur\n";
            std::cout << "2. Seyahatlerimi Görüntüle\n";
            std::cout << "3. Seyahat Düzenle\n";
            std::cout << "4. Seyahat Sil\n";
            std::cout << "0. Ana Menüye Dön\n\n";
            std::cout << "Seçiminiz: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    User* currentUser = UserAuth::getCurrentUser();
                    Trip newTrip;
                    newTrip.userId = currentUser->userId;

                    std::cout << "\n--- Yeni Seyahat Bilgileri ---\n\n";

                    if (!getStringInput("Varış Noktası: ", newTrip.destination, sizeof(newTrip.destination))) {
                        std::cout << "\n[!] Varış noktası boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getStringInput("Başlangıç Tarihi (YYYY-MM-DD): ", newTrip.startDate, sizeof(newTrip.startDate))) {
                        std::cout << "\n[!] Başlangıç tarihi boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getStringInput("Bitiş Tarihi (YYYY-MM-DD): ", newTrip.endDate, sizeof(newTrip.endDate))) {
                        std::cout << "\n[!] Bitiş tarihi boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getStringInput("Konaklama Bilgisi: ", newTrip.accommodation, sizeof(newTrip.accommodation))) {
                        std::cout << "\n[!] Konaklama bilgisi boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getStringInput("Ulaşım Bilgisi: ", newTrip.transportation, sizeof(newTrip.transportation))) {
                        std::cout << "\n[!] Ulaşım bilgisi boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getDoubleInput("Toplam Bütçe: ", newTrip.budget)) {
                        std::cout << "\n[!] Geçersiz bütçe değeri!\n";
                        waitForContinue();
                        break;
                    }

                    if (newTrip.budget < 0) {
                        std::cout << "\n[!] Bütçe negatif olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    int32_t tripId;
                    ErrorCode result = TripManager::createTrip(newTrip, tripId);
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n[+] Seyahat başarıyla oluşturuldu!\n";
                        std::cout << "[i] Seyahat ID: " << tripId << "\n";
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 2: {
                    User* currentUser = UserAuth::getCurrentUser();
                    std::vector<Trip> trips;
                    ErrorCode result = TripManager::getTrips(currentUser->userId, trips);
                    
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n--- Seyahatlerim ---\n\n";
                        
                        if (trips.empty()) {
                            std::cout << "[i] Henüz seyahat eklenmemiş.\n";
                        } else {
                            std::cout << std::left << std::setw(6) << "ID" 
                                     << std::setw(25) << "Varış Noktası"
                                     << std::setw(15) << "Başlangıç"
                                     << std::setw(15) << "Bitiş"
                                     << std::setw(12) << "Bütçe"
                                     << "\n";
                            printSeparator();
                            
                            for (const auto& trip : trips) {
                                std::cout << std::left << std::setw(6) << trip.tripId
                                         << std::setw(25) << trip.destination
                                         << std::setw(15) << trip.startDate
                                         << std::setw(15) << trip.endDate
                                         << std::fixed << std::setprecision(2)
                                         << std::setw(12) << trip.budget
                                         << "\n";
                            }
                        }
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 3: {
                    int tripId;
                    if (!getIntInput("\nDüzenlenecek Seyahat ID: ", tripId)) {
                        std::cout << "\n[!] Geçersiz ID!\n";
                        waitForContinue();
                        break;
                    }

                    Trip existingTrip;
                    ErrorCode getResult = TripManager::getTrip(tripId, existingTrip);
                    if (getResult != TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n[!] Seyahat bulunamadı!\n";
                        waitForContinue();
                        break;
                    }

                    User* currentUser = UserAuth::getCurrentUser();
                    if (existingTrip.userId != currentUser->userId) {
                        std::cout << "\n[!] Bu seyahat size ait değil!\n";
                        waitForContinue();
                        break;
                    }

                    std::cout << "\n--- Seyahat Bilgilerini Güncelle ---\n";
                    std::cout << "[i] Değiştirmek istemediğiniz alanlar için Enter'a basın.\n\n";

                    Trip updatedTrip = existingTrip;
                    char prompt[200];
                    
                    snprintf(prompt, sizeof(prompt), "Varış Noktası [%s]: ", existingTrip.destination);
                    if (getStringInput(prompt, updatedTrip.destination, sizeof(updatedTrip.destination))) {
                        // Kullanıcı yeni değer girdi
                    }
                    
                    snprintf(prompt, sizeof(prompt), "Başlangıç Tarihi [%s]: ", existingTrip.startDate);
                    if (getStringInput(prompt, updatedTrip.startDate, sizeof(updatedTrip.startDate))) {
                        // Kullanıcı yeni değer girdi
                    }
                    
                    snprintf(prompt, sizeof(prompt), "Bitiş Tarihi [%s]: ", existingTrip.endDate);
                    if (getStringInput(prompt, updatedTrip.endDate, sizeof(updatedTrip.endDate))) {
                        // Kullanıcı yeni değer girdi
                    }
                    
                    snprintf(prompt, sizeof(prompt), "Konaklama [%s]: ", existingTrip.accommodation);
                    if (getStringInput(prompt, updatedTrip.accommodation, sizeof(updatedTrip.accommodation))) {
                        // Kullanıcı yeni değer girdi
                    }
                    
                    snprintf(prompt, sizeof(prompt), "Ulaşım [%s]: ", existingTrip.transportation);
                    if (getStringInput(prompt, updatedTrip.transportation, sizeof(updatedTrip.transportation))) {
                        // Kullanıcı yeni değer girdi
                    }
                    
                    double newBudget = existingTrip.budget;
                    char budgetStr[50];
                    snprintf(budgetStr, sizeof(budgetStr), "%.2f", existingTrip.budget);
                    snprintf(prompt, sizeof(prompt), "Bütçe [%.2f]: ", existingTrip.budget);
                    if (getDoubleInput(prompt, newBudget)) {
                        if (newBudget >= 0) {
                            updatedTrip.budget = newBudget;
                        }
                    }

                    ErrorCode result = TripManager::updateTrip(tripId, updatedTrip);
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        showSuccess("Seyahat başarıyla güncellendi!");
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 4: {
                    int tripId;
                    if (!getIntInput("\nSilinecek Seyahat ID: ", tripId)) {
                        std::cout << "\n[!] Geçersiz ID!\n";
                        waitForContinue();
                        break;
                    }

                    Trip existingTrip;
                    ErrorCode getResult = TripManager::getTrip(tripId, existingTrip);
                    if (getResult != TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n[!] Seyahat bulunamadı!\n";
                        waitForContinue();
                        break;
                    }

                    User* currentUser = UserAuth::getCurrentUser();
                    if (existingTrip.userId != currentUser->userId) {
                        std::cout << "\n[!] Bu seyahat size ait değil!\n";
                        waitForContinue();
                        break;
                    }

                    std::cout << "\n[?] '" << existingTrip.destination 
                              << "' seyahatini silmek istediğinize emin misiniz? (e/h): ";
                    char confirm;
                    std::cin >> confirm;
                    std::cin.ignore();

                    if (confirm == 'e' || confirm == 'E') {
                        ErrorCode result = TripManager::deleteTrip(tripId);
                        if (result == TravelExpense::ErrorCode::SUCCESS) {
                            showSuccess("Seyahat başarıyla silindi!");
                            waitForContinue();
                        } else {
                            showError(result);
                            waitForContinue();
                        }
                    } else {
                        showInfo("Silme işlemi iptal edildi.");
                        waitForContinue();
                    }
                    break;
                }
                case 0:
                    return;
                default:
                    std::cout << "\n[!] Geçersiz seçim! Lütfen 0-4 arası bir sayı girin.\n";
                    waitForContinue();
                    break;
            }
        } while (choice != 0);
    }

    // ==================== Gider Kaydı Menüsü ====================

    void showExpenseMenu() {
        int choice;

        do {
            clearScreen();
            printHeader("GİDER KAYDI");

            if (!requireLogin()) {
                return;
            }

            std::cout << "1. Yeni Gider Kaydet\n";
            std::cout << "2. Giderleri Görüntüle\n";
            std::cout << "3. Gider Düzenle\n";
            std::cout << "4. Gider Sil\n";
            std::cout << "0. Ana Menüye Dön\n\n";
            std::cout << "Seçiminiz: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    int tripId;
                    if (!getIntInput("\nSeyahat ID: ", tripId)) {
                        std::cout << "\n[!] Geçersiz ID!\n";
                        waitForContinue();
                        break;
                    }

                    Expense newExpense;
                    newExpense.tripId = tripId;

                    std::cout << "\n--- Yeni Gider Bilgileri ---\n\n";

                    int categoryChoice;
                    std::cout << "Kategori:\n";
                    std::cout << "  1. Konaklama\n";
                    std::cout << "  2. Ulaşım\n";
                    std::cout << "  3. Yemek\n";
                    std::cout << "  4. Eğlence\n";
                    std::cout << "Seçim (1-4): ";
                    std::cin >> categoryChoice;
                    std::cin.ignore();

                    // Use if-else instead of switch for better MSVC compatibility with enum class
                    if (categoryChoice == 1) {
                        newExpense.category = TravelExpense::ExpenseCategory::ACCOMMODATION;
                    } else if (categoryChoice == 2) {
                        newExpense.category = TravelExpense::ExpenseCategory::TRANSPORTATION;
                    } else if (categoryChoice == 3) {
                        newExpense.category = TravelExpense::ExpenseCategory::FOOD;
                    } else if (categoryChoice == 4) {
                        newExpense.category = TravelExpense::ExpenseCategory::ENTERTAINMENT;
                    } else {
                        std::cout << "\n[!] Geçersiz kategori! Varsayılan olarak Konaklama seçildi.\n";
                        newExpense.category = TravelExpense::ExpenseCategory::ACCOMMODATION;
                    }

                    if (!getDoubleInput("Tutar: ", newExpense.amount)) {
                        std::cout << "\n[!] Geçersiz tutar!\n";
                        waitForContinue();
                        break;
                    }

                    if (newExpense.amount <= 0) {
                        std::cout << "\n[!] Tutar pozitif olmalıdır!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getStringInput("Para Birimi (TRY/USD/EUR): ", newExpense.currency, sizeof(newExpense.currency))) {
                        std::cout << "\n[!] Para birimi boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getStringInput("Tarih (YYYY-MM-DD): ", newExpense.date, sizeof(newExpense.date))) {
                        std::cout << "\n[!] Tarih boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getStringInput("Ödeme Yöntemi: ", newExpense.paymentMethod, sizeof(newExpense.paymentMethod))) {
                        std::cout << "\n[!] Ödeme yöntemi boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getStringInput("Açıklama: ", newExpense.description, sizeof(newExpense.description))) {
                        std::cout << "\n[!] Açıklama boş olamaz!\n";
                        waitForContinue();
                        break;
                    }

                    int32_t expenseId;
                    ErrorCode result = ExpenseManager::logExpense(newExpense, expenseId);
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n[+] Gider başarıyla kaydedildi!\n";
                        std::cout << "[i] Gider ID: " << expenseId << "\n";
                        waitForContinue();
                    } else if (result == TravelExpense::ErrorCode::ERROR_BUDGET_EXCEEDED) {
                        std::cout << "\n[!] UYARI: Bütçe limiti aşıldı! Gider yine de kaydedildi.\n";
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 2: {
                    int tripId;
                    if (!getIntInput("\nSeyahat ID: ", tripId)) {
                        std::cout << "\n[!] Geçersiz ID!\n";
                        waitForContinue();
                        break;
                    }

                    std::vector<Expense> expenses;
                    ErrorCode result = ExpenseManager::getExpenses(tripId, expenses);
                    
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n--- Giderler ---\n\n";
                        
                        if (expenses.empty()) {
                            std::cout << "[i] Bu seyahat için henüz gider kaydedilmemiş.\n";
                        } else {
                            std::cout << std::left << std::setw(6) << "ID"
                                     << std::setw(15) << "Kategori"
                                     << std::setw(12) << "Tutar"
                                     << std::setw(8) << "Birim"
                                     << std::setw(15) << "Tarih"
                                     << "Açıklama"
                                     << "\n";
                            printSeparator();
                            
                            for (const auto& expense : expenses) {
                                std::cout << std::left << std::setw(6) << expense.expenseId
                                         << std::setw(15) << getCategoryString(expense.category)
                                         << std::fixed << std::setprecision(2)
                                         << std::setw(12) << expense.amount
                                         << std::setw(8) << expense.currency
                                         << std::setw(15) << expense.date
                                         << expense.description
                                         << "\n";
                            }
                        }
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 3: {
                    int expenseId;
                    if (!getIntInput("\nDüzenlenecek Gider ID: ", expenseId)) {
                        std::cout << "\n[!] Geçersiz ID!\n";
                        waitForContinue();
                        break;
                    }

                    Expense existingExpense;
                    ErrorCode getResult = ExpenseManager::getExpense(expenseId, existingExpense);
                    if (getResult != TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n[!] Gider bulunamadı!\n";
                        waitForContinue();
                        break;
                    }

                    std::cout << "\n--- Gider Bilgilerini Güncelle ---\n";
                    std::cout << "[i] Değiştirmek istemediğiniz alanlar için Enter'a basın.\n\n";

                    Expense updatedExpense = existingExpense;

                    int categoryChoice;
                    std::cout << "Kategori (1: Konaklama, 2: Ulaşım, 3: Yemek, 4: Eğlence) [Mevcut: "
                              << (static_cast<int>(existingExpense.category) + 1) << "]: ";
                    std::cin >> categoryChoice;
                    std::cin.ignore();

                    if (categoryChoice >= 1 && categoryChoice <= 4) {
                        // Use if-else instead of switch for better MSVC compatibility with enum class
                        if (categoryChoice == 1) {
                            updatedExpense.category = TravelExpense::ExpenseCategory::ACCOMMODATION;
                        } else if (categoryChoice == 2) {
                            updatedExpense.category = TravelExpense::ExpenseCategory::TRANSPORTATION;
                        } else if (categoryChoice == 3) {
                            updatedExpense.category = TravelExpense::ExpenseCategory::FOOD;
                        } else if (categoryChoice == 4) {
                            updatedExpense.category = TravelExpense::ExpenseCategory::ENTERTAINMENT;
                        }
                    }

                    double newAmount = existingExpense.amount;
                    char prompt[200];
                    snprintf(prompt, sizeof(prompt), "Tutar [%.2f]: ", existingExpense.amount);
                    if (getDoubleInput(prompt, newAmount)) {
                        if (newAmount > 0) {
                            updatedExpense.amount = newAmount;
                        }
                    }
                    
                    snprintf(prompt, sizeof(prompt), "Para Birimi [%s]: ", existingExpense.currency);
                    if (getStringInput(prompt, updatedExpense.currency, sizeof(updatedExpense.currency))) {
                        // Kullanıcı yeni değer girdi
                    }
                    
                    snprintf(prompt, sizeof(prompt), "Tarih [%s]: ", existingExpense.date);
                    if (getStringInput(prompt, updatedExpense.date, sizeof(updatedExpense.date))) {
                        // Kullanıcı yeni değer girdi
                    }
                    
                    snprintf(prompt, sizeof(prompt), "Ödeme Yöntemi [%s]: ", existingExpense.paymentMethod);
                    if (getStringInput(prompt, updatedExpense.paymentMethod, sizeof(updatedExpense.paymentMethod))) {
                        // Kullanıcı yeni değer girdi
                    }
                    
                    snprintf(prompt, sizeof(prompt), "Açıklama [%s]: ", existingExpense.description);
                    if (getStringInput(prompt, updatedExpense.description, sizeof(updatedExpense.description))) {
                        // Kullanıcı yeni değer girdi
                    }

                    ErrorCode result = ExpenseManager::updateExpense(expenseId, updatedExpense);
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        showSuccess("Gider başarıyla güncellendi!");
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 4: {
                    int expenseId;
                    if (!getIntInput("\nSilinecek Gider ID: ", expenseId)) {
                        std::cout << "\n[!] Geçersiz ID!\n";
                        waitForContinue();
                        break;
                    }

                    Expense existingExpense;
                    ErrorCode getResult = ExpenseManager::getExpense(expenseId, existingExpense);
                    if (getResult != TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n[!] Gider bulunamadı!\n";
                        waitForContinue();
                        break;
                    }

                    std::cout << "\n[?] " << getCategoryString(existingExpense.category)
                              << " kategorisindeki " << existingExpense.amount 
                              << " " << existingExpense.currency 
                              << " giderini silmek istediğinize emin misiniz? (e/h): ";
                    char confirm;
                    std::cin >> confirm;
                    std::cin.ignore();

                    if (confirm == 'e' || confirm == 'E') {
                        ErrorCode result = ExpenseManager::deleteExpense(expenseId);
                        if (result == TravelExpense::ErrorCode::SUCCESS) {
                            showSuccess("Gider başarıyla silindi!");
                            waitForContinue();
                        } else {
                            showError(result);
                            waitForContinue();
                        }
                    } else {
                        showInfo("Silme işlemi iptal edildi.");
                        waitForContinue();
                    }
                    break;
                }
                case 0:
                    return;
                default:
                    std::cout << "\n[!] Geçersiz seçim! Lütfen 0-4 arası bir sayı girin.\n";
                    waitForContinue();
                    break;
            }
        } while (choice != 0);
    }

    // ==================== Bütçe Yönetimi Menüsü ====================

    void showBudgetMenu() {
        int choice;

        do {
            clearScreen();
            printHeader("BÜTÇE YÖNETİMİ");

            if (!requireLogin()) {
                return;
            }

            std::cout << "1. Yeni Bütçe Belirle\n";
            std::cout << "2. Bütçeyi Görüntüle\n";
            std::cout << "3. Bütçeyi Güncelle\n";
            std::cout << "0. Ana Menüye Dön\n\n";
            std::cout << "Seçiminiz: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    int tripId;
                    double totalBudget;
                    double categoryBudgets[4] = {0, 0, 0, 0};

                    if (!getIntInput("\nSeyahat ID: ", tripId)) {
                        std::cout << "\n[!] Geçersiz ID!\n";
                        waitForContinue();
                        break;
                    }

                    if (!getDoubleInput("Toplam Bütçe: ", totalBudget)) {
                        std::cout << "\n[!] Geçersiz bütçe değeri!\n";
                        waitForContinue();
                        break;
                    }

                    if (totalBudget <= 0) {
                        std::cout << "\n[!] Bütçe pozitif olmalıdır!\n";
                        waitForContinue();
                        break;
                    }

                    std::cout << "\nKategori Bazlı Bütçeler (Opsiyonel):\n";
                    std::cout << "[i] Boş bırakırsanız 0 kabul edilir.\n\n";
                    
                    getDoubleInput("  Konaklama: ", categoryBudgets[0]);
                    getDoubleInput("  Ulaşım: ", categoryBudgets[1]);
                    getDoubleInput("  Yemek: ", categoryBudgets[2]);
                    getDoubleInput("  Eğlence: ", categoryBudgets[3]);

                    int32_t budgetId;
                    ErrorCode result = BudgetManager::setBudget(tripId, totalBudget, categoryBudgets, budgetId);
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n[+] Bütçe başarıyla belirlendi!\n";
                        std::cout << "[i] Bütçe ID: " << budgetId << "\n";
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 2: {
                    int tripId;
                    if (!getIntInput("\nSeyahat ID: ", tripId)) {
                        std::cout << "\n[!] Geçersiz ID!\n";
                        waitForContinue();
                        break;
                    }

                    Budget budget;
                    ErrorCode result = BudgetManager::getBudget(tripId, budget);
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n--- Bütçe Bilgileri ---\n\n";
                        std::cout << "Toplam Bütçe: " << std::fixed << std::setprecision(2) 
                                  << budget.totalBudget << "\n";
                        std::cout << "Harcanan: " << budget.spentAmount << "\n";
                        std::cout << "Kalan: " << (budget.totalBudget - budget.spentAmount) << "\n";
                        
                        double remaining = budget.totalBudget - budget.spentAmount;
                        if (remaining < 0) {
                            std::cout << "\n[!] UYARI: Bütçe limiti aşıldı! Aşım: " 
                                      << (-remaining) << "\n";
                        } else if (remaining < budget.totalBudget * 0.1) {
                            std::cout << "\n[i] UYARI: Bütçenin %90'ından fazlası harcanmış.\n";
                        }
                        
                        std::cout << "\n--- Kategori Bazlı Detaylar ---\n";
                        std::cout << std::left << std::setw(15) << "Kategori"
                                 << std::setw(15) << "Bütçe"
                                 << std::setw(15) << "Harcanan"
                                 << "Kalan"
                                 << "\n";
                        printSeparator();
                        
                        const char* categories[] = {"Konaklama", "Ulaşım", "Yemek", "Eğlence"};
                        for (int i = 0; i < 4; i++) {
                            double catRemaining = budget.categoryBudgets[i] - budget.categorySpent[i];
                            std::cout << std::left << std::setw(15) << categories[i]
                                     << std::fixed << std::setprecision(2)
                                     << std::setw(15) << budget.categoryBudgets[i]
                                     << std::setw(15) << budget.categorySpent[i]
                                     << catRemaining
                                     << "\n";
                        }
                        
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 3: {
                    int tripId;
                    if (!getIntInput("\nGüncellenecek Seyahat ID: ", tripId)) {
                        std::cout << "\n[!] Geçersiz ID!\n";
                        waitForContinue();
                        break;
                    }

                    Budget existingBudget;
                    ErrorCode getResult = BudgetManager::getBudget(tripId, existingBudget);
                    if (getResult != TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n[!] Bütçe bulunamadı! Önce bütçe belirleyin.\n";
                        waitForContinue();
                        break;
                    }

                    std::cout << "\n--- Bütçe Bilgilerini Güncelle ---\n";
                    std::cout << "[i] Değiştirmek istemediğiniz alanlar için Enter'a basın.\n\n";

                    Budget updatedBudget = existingBudget;
                    double newTotalBudget;
                    double newCategoryBudgets[4];

                    char prompt[200];
                    snprintf(prompt, sizeof(prompt), "Toplam Bütçe [%.2f]: ", existingBudget.totalBudget);
                    if (getDoubleInput(prompt, newTotalBudget)) {
                        if (newTotalBudget > 0) {
                            updatedBudget.totalBudget = newTotalBudget;
                        }
                    }

                    std::cout << "\nKategori Bazlı Bütçeler:\n";
                    const char* categories[] = {"Konaklama", "Ulaşım", "Yemek", "Eğlence"};
                    for (int i = 0; i < 4; i++) {
                        snprintf(prompt, sizeof(prompt), "  %s [%.2f]: ", categories[i], existingBudget.categoryBudgets[i]);
                        if (getDoubleInput(prompt, newCategoryBudgets[i])) {
                            if (newCategoryBudgets[i] >= 0) {
                                updatedBudget.categoryBudgets[i] = newCategoryBudgets[i];
                            }
                        }
                    }

                    ErrorCode result = BudgetManager::updateBudget(existingBudget.budgetId, updatedBudget);
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        showSuccess("Bütçe başarıyla güncellendi!");
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 0:
                    return;
                default:
                    std::cout << "\n[!] Geçersiz seçim! Lütfen 0-3 arası bir sayı girin.\n";
                    waitForContinue();
                    break;
            }
        } while (choice != 0);
    }

    // ==================== Özet Rapor Menüsü ====================

    void showReportMenu() {
        int choice;

        do {
            clearScreen();
            printHeader("ÖZET RAPOR");

            if (!requireLogin()) {
                return;
            }

            std::cout << "1. Seyahat Raporu Oluştur\n";
            std::cout << "2. Raporları Görüntüle\n";
            std::cout << "0. Ana Menüye Dön\n\n";
            std::cout << "Seçiminiz: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    int tripId;
                    if (!getIntInput("\nSeyahat ID: ", tripId)) {
                        std::cout << "\n[!] Geçersiz ID!\n";
                        waitForContinue();
                        break;
                    }

                    std::string report;
                    ErrorCode result = ReportGenerator::generateReport(tripId, report);
                    if (result == TravelExpense::ErrorCode::SUCCESS) {
                        std::cout << "\n";
                        std::cout << "========================================\n";
                        std::cout << "         SEYAHAT RAPORU\n";
                        std::cout << "========================================\n\n";
                        std::cout << report << "\n";
                        std::cout << "========================================\n";
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 2:
                    std::cout << "\n[i] Bu özellik yakında eklenecek!\n";
                    waitForContinue();
                    break;
                case 0:
                    return;
                default:
                    std::cout << "\n[!] Geçersiz seçim! Lütfen 0-2 arası bir sayı girin.\n";
                    waitForContinue();
                    break;
            }
        } while (choice != 0);
    }

    // ==================== Ana Menü ====================

    int showMainMenu() {
        int choice;

        do {
            clearScreen();
            printHeader("SEYAHAT GİDERİ TAKİBİ");

            User* currentUser = UserAuth::getCurrentUser();
            if (currentUser) {
                std::cout << "Kullanıcı: " << currentUser->username;
                if (currentUser->isGuest) {
                    std::cout << " (Misafir)";
                }
                std::cout << "\n";
            } else {
                std::cout << "Kullanıcı: Giriş yapılmamış\n";
            }
            std::cout << "\n";

            std::cout << "1. Kullanıcı İşlemleri\n";
            std::cout << "2. Seyahat Planlama\n";
            std::cout << "3. Gider Kaydı\n";
            std::cout << "4. Bütçe Yönetimi\n";
            std::cout << "5. Özet Rapor\n";
            std::cout << "0. Çıkış\n\n";
            std::cout << "Seçiminiz: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1:
                    showUserMenu();
                    break;
                case 2:
                    showTripMenu();
                    break;
                case 3:
                    showExpenseMenu();
                    break;
                case 4:
                    showBudgetMenu();
                    break;
                case 5:
                    showReportMenu();
                    break;
                case 0:
                    std::cout << "\n[i] Çıkılıyor...\n";
                    UserAuth::logoutUser();
                    return 0;
                default:
                    std::cout << "\n[!] Geçersiz seçim! Lütfen 0-5 arası bir sayı girin.\n";
                    waitForContinue();
                    break;
            }
        } while (choice != 0);

        return 0;
    }

} // namespace TravelExpenseApp

/**
 * @brief Main fonksiyon
 */
int main() {
    // Windows'ta Türkçe karakter desteği için
    #ifdef _WIN32
        SetConsoleOutputCP(65001); // UTF-8
        SetConsoleCP(65001);
    #endif

    // Veritabanını başlat
    sqlite3* db = TravelExpense::Database::getDatabase();
    if (!db) {
        std::cerr << "[!] KRITIK HATA: Veritabanı başlatılamadı!\n";
        std::cerr << "[!] Veritabanı dosyası oluşturulamıyor veya erişilemiyor.\n";
        std::cerr << "[!] Lütfen 'data' klasörünün yazılabilir olduğundan emin olun.\n";
        std::cout << "\nDevam etmek için Enter'a basın...";
        std::cin.get();
        return 1;
    }

    // Ana menüyü göster
    int exitCode = TravelExpenseApp::showMainMenu();

    return exitCode;
}
