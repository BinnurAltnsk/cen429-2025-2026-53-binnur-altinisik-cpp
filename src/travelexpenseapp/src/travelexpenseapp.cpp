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
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN() system("cls")
#else
    #include <cstdlib>
    #define CLEAR_SCREEN() system("clear")
#endif

using namespace TravelExpense;
using namespace TravelExpenseApp;

namespace TravelExpenseApp {

    void clearScreen() {
        CLEAR_SCREEN();
    }

    bool getStringInput(const char* prompt, char* output, size_t maxLength) {
        if (!prompt || !output || maxLength == 0) {
            return false;
        }

        std::cout << prompt;
        std::cin.getline(output, maxLength);
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        }

        std::cin.ignore(); // \n karakterini temizle
        return true;
    }

    void showError(ErrorCode errorCode) {
        std::cout << "\n[!] HATA: ";
        
        switch (errorCode) {
            case ErrorCode::ERROR_FILE_NOT_FOUND:
                std::cout << "Dosya bulunamadı.\n";
                break;
            case ErrorCode::ERROR_DECRYPTION_FAILED:
                std::cout << "Şifre çözme hatası.\n";
                break;
            case ErrorCode::ERROR_CHECKSUM_MISMATCH:
                std::cout << "Veri bütünlüğü hatası.\n";
                break;
            case ErrorCode::ERROR_INVALID_USER:
                std::cout << "Geçersiz kullanıcı.\n";
                break;
            case ErrorCode::ERROR_BUDGET_EXCEEDED:
                std::cout << "Bütçe limiti aşıldı.\n";
                break;
            case ErrorCode::ERROR_INVALID_INPUT:
                std::cout << "Geçersiz girdi.\n";
                break;
            case ErrorCode::ERROR_MEMORY_ALLOCATION:
                std::cout << "Bellek ayırma hatası.\n";
                break;
            case ErrorCode::ERROR_FILE_IO:
                std::cout << "Dosya okuma/yazma hatası.\n";
                break;
            case ErrorCode::ERROR_ENCRYPTION_FAILED:
                std::cout << "Şifreleme hatası.\n";
                break;
            default:
                std::cout << "Bilinmeyen hata.\n";
                break;
        }
    }

    void showSuccess(const char* message) {
        std::cout << "\n[+] " << message << "\n";
    }

    void waitForContinue() {
        std::cout << "\nDevam etmek için Enter'a basın...";
        std::cin.get();
    }

    void showUserMenu() {
        int choice;
        char username[50];
        char password[100];

        do {
            clearScreen();
            std::cout << "=== KULLANICI İŞLEMLERİ ===\n\n";
            std::cout << "1. Giriş Yap\n";
            std::cout << "2. Kaydol\n";
            std::cout << "3. Misafir Modu\n";
            std::cout << "0. Ana Menü\n\n";
            std::cout << "Seçiminiz: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    if (getStringInput("Kullanıcı Adı: ", username, sizeof(username)) &&
                        getStringInput("Şifre: ", password, sizeof(password))) {
                        ErrorCode result = UserAuth::loginUser(username, password);
                        if (result == ErrorCode::SUCCESS) {
                            showSuccess("Giriş başarılı!");
                            waitForContinue();
                            return;
                        } else {
                            showError(result);
                            waitForContinue();
                        }
                    }
                    break;
                }
                case 2: {
                    if (getStringInput("Kullanıcı Adı: ", username, sizeof(username)) &&
                        getStringInput("Şifre: ", password, sizeof(password))) {
                        ErrorCode result = UserAuth::registerUser(username, password);
                        if (result == ErrorCode::SUCCESS) {
                            showSuccess("Kayıt başarılı!");
                            waitForContinue();
                        } else {
                            showError(result);
                            waitForContinue();
                        }
                    }
                    break;
                }
                case 3: {
                    ErrorCode result = UserAuth::enableGuestMode();
                    if (result == ErrorCode::SUCCESS) {
                        showSuccess("Misafir modu etkinleştirildi!");
                        waitForContinue();
                        return;
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 0:
                    return;
                default:
                    std::cout << "\nGeçersiz seçim!\n";
                    waitForContinue();
                    break;
            }
        } while (choice != 0);
    }

    void showTripMenu() {
        int choice;

        do {
            clearScreen();
            std::cout << "=== SEYAHAT PLANLAMA ===\n\n";
            std::cout << "1. Seyahat Oluştur\n";
            std::cout << "2. Seyahatleri Görüntüle\n";
            std::cout << "3. Seyahati Düzenle\n";
            std::cout << "4. Seyahati Sil\n";
            std::cout << "0. Ana Menü\n\n";
            std::cout << "Seçiminiz: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    User* currentUser = UserAuth::getCurrentUser();
                    if (!currentUser || currentUser->userId <= 0) {
                        std::cout << "\n[!] Önce giriş yapmalısınız!\n";
                        waitForContinue();
                        break;
                    }

                    Trip newTrip;
                    newTrip.userId = currentUser->userId;

                    if (getStringInput("Varış Noktası: ", newTrip.destination, sizeof(newTrip.destination)) &&
                        getStringInput("Başlangıç Tarihi (YYYY-MM-DD): ", newTrip.startDate, sizeof(newTrip.startDate)) &&
                        getStringInput("Bitiş Tarihi (YYYY-MM-DD): ", newTrip.endDate, sizeof(newTrip.endDate)) &&
                        getStringInput("Konaklama: ", newTrip.accommodation, sizeof(newTrip.accommodation)) &&
                        getStringInput("Ulaşım: ", newTrip.transportation, sizeof(newTrip.transportation)) &&
                        getDoubleInput("Bütçe: ", newTrip.budget)) {

                        int32_t tripId;
                        ErrorCode result = TripManager::createTrip(newTrip, tripId);
                        if (result == ErrorCode::SUCCESS) {
                            std::cout << "\n[+] Seyahat başarıyla oluşturuldu! ID: " << tripId << "\n";
                            waitForContinue();
                        } else {
                            showError(result);
                            waitForContinue();
                        }
                    }
                    break;
                }
                case 2: {
                    User* currentUser = UserAuth::getCurrentUser();
                    if (!currentUser || currentUser->userId <= 0) {
                        std::cout << "\n[!] Önce giriş yapmalısınız!\n";
                        waitForContinue();
                        break;
                    }

                    std::vector<Trip> trips;
                    ErrorCode result = TripManager::getTrips(currentUser->userId, trips);
                    if (result == ErrorCode::SUCCESS) {
                        std::cout << "\n=== SEYAHATLER ===\n\n";
                        for (const auto& trip : trips) {
                            std::cout << "ID: " << trip.tripId << " - " << trip.destination 
                                      << " (" << trip.startDate << " - " << trip.endDate << ")\n";
                        }
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 3:
                case 4:
                    std::cout << "\n[!] Bu özellik yakında eklenecek!\n";
                    waitForContinue();
                    break;
                case 0:
                    return;
                default:
                    std::cout << "\nGeçersiz seçim!\n";
                    waitForContinue();
                    break;
            }
        } while (choice != 0);
    }

    void showExpenseMenu() {
        int choice;

        do {
            clearScreen();
            std::cout << "=== GİDER KAYDI ===\n\n";
            std::cout << "1. Gider Kaydet\n";
            std::cout << "2. Giderleri Görüntüle\n";
            std::cout << "3. Gideri Düzenle\n";
            std::cout << "4. Gideri Sil\n";
            std::cout << "0. Ana Menü\n\n";
            std::cout << "Seçiminiz: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    int tripId;
                    if (!getIntInput("Seyahat ID: ", tripId)) {
                        break;
                    }

                    Expense newExpense;
                    newExpense.tripId = tripId;

                    int categoryChoice;
                    std::cout << "Kategori (1: Konaklama, 2: Ulaşım, 3: Yemek, 4: Eğlence): ";
                    std::cin >> categoryChoice;
                    std::cin.ignore();

                    switch (categoryChoice) {
                        case 1: newExpense.category = ExpenseCategory::ACCOMMODATION; break;
                        case 2: newExpense.category = ExpenseCategory::TRANSPORTATION; break;
                        case 3: newExpense.category = ExpenseCategory::FOOD; break;
                        case 4: newExpense.category = ExpenseCategory::ENTERTAINMENT; break;
                        default: newExpense.category = ExpenseCategory::ACCOMMODATION; break;
                    }

                    if (getDoubleInput("Tutar: ", newExpense.amount) &&
                        getStringInput("Para Birimi (TRY/USD/EUR): ", newExpense.currency, sizeof(newExpense.currency)) &&
                        getStringInput("Tarih (YYYY-MM-DD): ", newExpense.date, sizeof(newExpense.date)) &&
                        getStringInput("Ödeme Yöntemi: ", newExpense.paymentMethod, sizeof(newExpense.paymentMethod)) &&
                        getStringInput("Açıklama: ", newExpense.description, sizeof(newExpense.description))) {

                        int32_t expenseId;
                        ErrorCode result = ExpenseManager::logExpense(newExpense, expenseId);
                        if (result == ErrorCode::SUCCESS) {
                            std::cout << "\n[+] Gider başarıyla kaydedildi! ID: " << expenseId << "\n";
                            waitForContinue();
                        } else {
                            showError(result);
                            waitForContinue();
                        }
                    }
                    break;
                }
                case 2: {
                    int tripId;
                    if (!getIntInput("Seyahat ID: ", tripId)) {
                        break;
                    }

                    std::vector<Expense> expenses;
                    ErrorCode result = ExpenseManager::getExpenses(tripId, expenses);
                    if (result == ErrorCode::SUCCESS) {
                        std::cout << "\n=== GİDERLER ===\n\n";
                        for (const auto& expense : expenses) {
                            std::cout << "ID: " << expense.expenseId << " - " 
                                      << getCategoryString(expense.category)
                                      << ": " << expense.amount << " " << expense.currency
                                      << " (" << expense.date << ")\n";
                        }
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 3:
                case 4:
                    std::cout << "\n[!] Bu özellik yakında eklenecek!\n";
                    waitForContinue();
                    break;
                case 0:
                    return;
                default:
                    std::cout << "\nGeçersiz seçim!\n";
                    waitForContinue();
                    break;
            }
        } while (choice != 0);
    }

    void showBudgetMenu() {
        int choice;

        do {
            clearScreen();
            std::cout << "=== BÜTÇE YÖNETİMİ ===\n\n";
            std::cout << "1. Bütçe Belirle\n";
            std::cout << "2. Bütçeyi Görüntüle\n";
            std::cout << "3. Bütçeyi Düzenle\n";
            std::cout << "0. Ana Menü\n\n";
            std::cout << "Seçiminiz: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    int tripId;
                    double totalBudget;
                    double categoryBudgets[4];

                    if (!getIntInput("Seyahat ID: ", tripId) ||
                        !getDoubleInput("Toplam Bütçe: ", totalBudget)) {
                        break;
                    }

                    std::cout << "Kategori Bazlı Bütçeler:\n";
                    getDoubleInput("  Konaklama: ", categoryBudgets[0]);
                    getDoubleInput("  Ulaşım: ", categoryBudgets[1]);
                    getDoubleInput("  Yemek: ", categoryBudgets[2]);
                    getDoubleInput("  Eğlence: ", categoryBudgets[3]);

                    int32_t budgetId;
                    ErrorCode result = BudgetManager::setBudget(tripId, totalBudget, categoryBudgets, budgetId);
                    if (result == ErrorCode::SUCCESS) {
                        std::cout << "\n[+] Bütçe başarıyla belirlendi! ID: " << budgetId << "\n";
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 2: {
                    int tripId;
                    if (!getIntInput("Seyahat ID: ", tripId)) {
                        break;
                    }

                    Budget budget;
                    ErrorCode result = BudgetManager::getBudget(tripId, budget);
                    if (result == ErrorCode::SUCCESS) {
                        std::cout << "\n=== BÜTÇE BİLGİLERİ ===\n\n";
                        std::cout << "Toplam Bütçe: " << budget.totalBudget << "\n";
                        std::cout << "Harcanan: " << budget.spentAmount << "\n";
                        std::cout << "Kalan: " << (budget.totalBudget - budget.spentAmount) << "\n";
                        std::cout << "\nKategori Bazlı:\n";
                        std::cout << "  Konaklama: " << budget.categorySpent[0] << " / " << budget.categoryBudgets[0] << "\n";
                        std::cout << "  Ulaşım: " << budget.categorySpent[1] << " / " << budget.categoryBudgets[1] << "\n";
                        std::cout << "  Yemek: " << budget.categorySpent[2] << " / " << budget.categoryBudgets[2] << "\n";
                        std::cout << "  Eğlence: " << budget.categorySpent[3] << " / " << budget.categoryBudgets[3] << "\n";
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 3:
                    std::cout << "\n[!] Bu özellik yakında eklenecek!\n";
                    waitForContinue();
                    break;
                case 0:
                    return;
                default:
                    std::cout << "\nGeçersiz seçim!\n";
                    waitForContinue();
                    break;
            }
        } while (choice != 0);
    }

    void showReportMenu() {
        int choice;

        do {
            clearScreen();
            std::cout << "=== ÖZET RAPOR ===\n\n";
            std::cout << "1. Rapor Oluştur\n";
            std::cout << "2. Raporları Görüntüle\n";
            std::cout << "0. Ana Menü\n\n";
            std::cout << "Seçiminiz: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    int tripId;
                    if (!getIntInput("Seyahat ID: ", tripId)) {
                        break;
                    }

                    std::string report;
                    ErrorCode result = ReportGenerator::generateReport(tripId, report);
                    if (result == ErrorCode::SUCCESS) {
                        std::cout << "\n" << report << "\n";
                        waitForContinue();
                    } else {
                        showError(result);
                        waitForContinue();
                    }
                    break;
                }
                case 2:
                    std::cout << "\n[!] Bu özellik yakında eklenecek!\n";
                    waitForContinue();
                    break;
                case 0:
                    return;
                default:
                    std::cout << "\nGeçersiz seçim!\n";
                    waitForContinue();
                    break;
            }
        } while (choice != 0);
    }

    int showMainMenu() {
        int choice;

        do {
            clearScreen();
            std::cout << "========================================\n";
            std::cout << "    SEYAHAT GİDERİ TAKİBİ\n";
            std::cout << "========================================\n\n";

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
                    std::cout << "\nÇıkılıyor...\n";
                    UserAuth::logoutUser();
                    return 0;
                default:
                    std::cout << "\nGeçersiz seçim!\n";
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
    return TravelExpenseApp::showMainMenu();
}

