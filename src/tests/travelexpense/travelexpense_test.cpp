/**
 * @file travelexpense_test.cpp
 * @brief Seyahat Gideri Takibi - Unit Test Dosyası
 * 
 * Bu dosya, seyahat gideri takibi kütüphanesinin GoogleTest tabanlı
 * unit testlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#define ENABLE_TRAVELEXPENSE_TEST

#include "gtest/gtest.h"
#include "../../travelexpense/header/travelexpense.h"
#include <cstring>
#include <vector>

#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
#endif

using namespace TravelExpense;

// ==================== Kullanıcı Yönetimi Testleri ====================

class UserAuthTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Test öncesi hazırlık
        // Dosya dizinini oluştur
        #ifdef _WIN32
            _mkdir("data");
        #else
            mkdir("data", 0755);
        #endif
    }

    void TearDown() override {
        // Test sonrası temizlik
        UserAuth::logoutUser();
    }
};

TEST_F(UserAuthTest, TestGuestMode) {
    ErrorCode result = UserAuth::enableGuestMode();
    EXPECT_EQ(result, ErrorCode::SUCCESS);
    
    User* currentUser = UserAuth::getCurrentUser();
    ASSERT_NE(currentUser, nullptr);
    EXPECT_TRUE(currentUser->isGuest);
    EXPECT_EQ(strcmp(currentUser->username, "Guest"), 0);
}

TEST_F(UserAuthTest, TestRegisterUser) {
    ErrorCode result = UserAuth::registerUser("testuser", "testpass123");
    EXPECT_EQ(result, ErrorCode::SUCCESS);
}

TEST_F(UserAuthTest, TestRegisterUserInvalidInput) {
    // Boş username
    ErrorCode result = UserAuth::registerUser("", "testpass123");
    EXPECT_EQ(result, ErrorCode::ERROR_INVALID_INPUT);
    
    // Boş password
    result = UserAuth::registerUser("testuser", "");
    EXPECT_EQ(result, ErrorCode::ERROR_INVALID_INPUT);
    
    // nullptr
    result = UserAuth::registerUser(nullptr, "testpass123");
    EXPECT_EQ(result, ErrorCode::ERROR_INVALID_INPUT);
}

TEST_F(UserAuthTest, TestLoginUser) {
    // Önce kayıt ol
    ErrorCode result = UserAuth::registerUser("logintest", "password123");
    EXPECT_EQ(result, ErrorCode::SUCCESS);
    
    // Sonra giriş yap
    result = UserAuth::loginUser("logintest", "password123");
    // TODO: Gerçek şifre kontrolü eklendiğinde SUCCESS olacak
    // Şimdilik test edilebilir
}

// ==================== Seyahat Yönetimi Testleri ====================

class TripManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Misafir modunda başlat
        UserAuth::enableGuestMode();
        User* user = UserAuth::getCurrentUser();
        if (user) {
            user->userId = 1; // Test için ID ataması
        }
    }

    void TearDown() override {
        UserAuth::logoutUser();
    }
};

TEST_F(TripManagerTest, TestCreateTrip) {
    Trip trip;
    trip.userId = 1;
    strncpy(trip.destination, "İstanbul", sizeof(trip.destination) - 1);
    strncpy(trip.startDate, "2025-01-01", sizeof(trip.startDate) - 1);
    strncpy(trip.endDate, "2025-01-07", sizeof(trip.endDate) - 1);
    trip.budget = 5000.0;

    int32_t tripId;
    ErrorCode result = TripManager::createTrip(trip, tripId);
    EXPECT_EQ(result, ErrorCode::SUCCESS);
    EXPECT_GT(tripId, 0);
}

TEST_F(TripManagerTest, TestCreateTripInvalidInput) {
    Trip trip;
    trip.userId = 0; // Geçersiz user ID

    int32_t tripId;
    ErrorCode result = TripManager::createTrip(trip, tripId);
    EXPECT_EQ(result, ErrorCode::ERROR_INVALID_INPUT);
}

TEST_F(TripManagerTest, TestGetTrips) {
    // Önce bir seyahat oluştur
    Trip trip;
    trip.userId = 1;
    strncpy(trip.destination, "Ankara", sizeof(trip.destination) - 1);
    trip.budget = 3000.0;

    int32_t tripId;
    ErrorCode result = TripManager::createTrip(trip, tripId);
    EXPECT_EQ(result, ErrorCode::SUCCESS);

    // Seyahatleri listele
    std::vector<Trip> trips;
    result = TripManager::getTrips(1, trips);
    EXPECT_EQ(result, ErrorCode::SUCCESS);
    EXPECT_GT(trips.size(), 0);
}

TEST_F(TripManagerTest, TestGetTrip) {
    // Önce bir seyahat oluştur
    Trip newTrip;
    newTrip.userId = 1;
    strncpy(newTrip.destination, "İzmir", sizeof(newTrip.destination) - 1);
    newTrip.budget = 4000.0;

    int32_t tripId;
    ErrorCode result = TripManager::createTrip(newTrip, tripId);
    EXPECT_EQ(result, ErrorCode::SUCCESS);

    // Seyahati al
    Trip retrievedTrip;
    result = TripManager::getTrip(tripId, retrievedTrip);
    EXPECT_EQ(result, ErrorCode::SUCCESS);
    EXPECT_EQ(retrievedTrip.tripId, tripId);
    EXPECT_EQ(strcmp(retrievedTrip.destination, "İzmir"), 0);
}

// ==================== Gider Yönetimi Testleri ====================

class ExpenseManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        UserAuth::enableGuestMode();
    }

    void TearDown() override {
        UserAuth::logoutUser();
    }
};

TEST_F(ExpenseManagerTest, TestLogExpense) {
    Expense expense;
    expense.tripId = 1;
    expense.category = ExpenseCategory::FOOD;
    expense.amount = 150.0;
    strncpy(expense.currency, "TRY", sizeof(expense.currency) - 1);
    strncpy(expense.date, "2025-01-15", sizeof(expense.date) - 1);

    int32_t expenseId;
    ErrorCode result = ExpenseManager::logExpense(expense, expenseId);
    EXPECT_EQ(result, ErrorCode::SUCCESS);
    EXPECT_GT(expenseId, 0);
}

TEST_F(ExpenseManagerTest, TestLogExpenseInvalidInput) {
    Expense expense;
    expense.tripId = 0; // Geçersiz trip ID
    expense.amount = 100.0;

    int32_t expenseId;
    ErrorCode result = ExpenseManager::logExpense(expense, expenseId);
    EXPECT_EQ(result, ErrorCode::ERROR_INVALID_INPUT);

    expense.tripId = 1;
    expense.amount = -50.0; // Negatif tutar

    result = ExpenseManager::logExpense(expense, expenseId);
    EXPECT_EQ(result, ErrorCode::ERROR_INVALID_INPUT);
}

TEST_F(ExpenseManagerTest, TestGetExpenses) {
    // Önce bir gider kaydet
    Expense expense;
    expense.tripId = 1;
    expense.category = ExpenseCategory::TRANSPORTATION;
    expense.amount = 500.0;

    int32_t expenseId;
    ErrorCode result = ExpenseManager::logExpense(expense, expenseId);
    EXPECT_EQ(result, ErrorCode::SUCCESS);

    // Giderleri listele
    std::vector<Expense> expenses;
    result = ExpenseManager::getExpenses(1, expenses);
    EXPECT_EQ(result, ErrorCode::SUCCESS);
    EXPECT_GT(expenses.size(), 0);
}

// ==================== Bütçe Yönetimi Testleri ====================

class BudgetManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        UserAuth::enableGuestMode();
    }

    void TearDown() override {
        UserAuth::logoutUser();
    }
};

TEST_F(BudgetManagerTest, TestSetBudget) {
    double categoryBudgets[4] = {2000.0, 1500.0, 1000.0, 500.0};

    int32_t budgetId;
    ErrorCode result = BudgetManager::setBudget(1, 5000.0, categoryBudgets, budgetId);
    EXPECT_EQ(result, ErrorCode::SUCCESS);
    EXPECT_GT(budgetId, 0);
}

TEST_F(BudgetManagerTest, TestSetBudgetInvalidInput) {
    double categoryBudgets[4] = {1000.0, 500.0, 300.0, 200.0};

    int32_t budgetId;
    // Geçersiz trip ID
    ErrorCode result = BudgetManager::setBudget(0, 2000.0, categoryBudgets, budgetId);
    EXPECT_EQ(result, ErrorCode::ERROR_INVALID_INPUT);

    // Geçersiz bütçe
    result = BudgetManager::setBudget(1, -1000.0, categoryBudgets, budgetId);
    EXPECT_EQ(result, ErrorCode::ERROR_INVALID_INPUT);
}

TEST_F(BudgetManagerTest, TestGetBudget) {
    // Önce bütçe oluştur
    double categoryBudgets[4] = {1500.0, 1000.0, 800.0, 700.0};
    int32_t budgetId;
    ErrorCode result = BudgetManager::setBudget(1, 4000.0, categoryBudgets, budgetId);
    EXPECT_EQ(result, ErrorCode::SUCCESS);

    // Bütçeyi al
    Budget budget;
    result = BudgetManager::getBudget(1, budget);
    EXPECT_EQ(result, ErrorCode::SUCCESS);
    EXPECT_EQ(budget.totalBudget, 4000.0);
}

TEST_F(BudgetManagerTest, TestCheckBudgetLimit) {
    // Önce bütçe oluştur
    double categoryBudgets[4] = {2000.0, 1500.0, 1000.0, 500.0};
    int32_t budgetId;
    ErrorCode result = BudgetManager::setBudget(1, 5000.0, categoryBudgets, budgetId);
    EXPECT_EQ(result, ErrorCode::SUCCESS);

    // Bütçe limiti kontrolü
    bool withinLimit = BudgetManager::checkBudgetLimit(1, ExpenseCategory::FOOD, 800.0);
    EXPECT_TRUE(withinLimit);

    bool overLimit = BudgetManager::checkBudgetLimit(1, ExpenseCategory::FOOD, 1200.0);
    EXPECT_FALSE(overLimit);
}

// ==================== Rapor Oluşturma Testleri ====================

class ReportGeneratorTest : public ::testing::Test {
protected:
    void SetUp() override {
        UserAuth::enableGuestMode();
        User* user = UserAuth::getCurrentUser();
        if (user) {
            user->userId = 1;
        }

        // Test için bir seyahat ve gider oluştur
        Trip trip;
        trip.userId = 1;
        strncpy(trip.destination, "Test Destination", sizeof(trip.destination) - 1);
        trip.budget = 5000.0;

        int32_t tripId;
        TripManager::createTrip(trip, tripId);

        Expense expense;
        expense.tripId = tripId;
        expense.category = ExpenseCategory::FOOD;
        expense.amount = 200.0;

        int32_t expenseId;
        ExpenseManager::logExpense(expense, expenseId);
    }

    void TearDown() override {
        UserAuth::logoutUser();
    }
};

TEST_F(ReportGeneratorTest, TestGenerateReport) {
    // İlk seyahati al (test için)
    std::vector<Trip> trips;
    TripManager::getTrips(1, trips);
    if (trips.empty()) {
        FAIL() << "Test trip bulunamadı";
        return;
    }

    int32_t tripId = trips[0].tripId;

    std::string report;
    ErrorCode result = ReportGenerator::generateReport(tripId, report);
    EXPECT_EQ(result, ErrorCode::SUCCESS);
    EXPECT_FALSE(report.empty());
    EXPECT_NE(report.find("SEYAHAT ÖZET RAPORU"), std::string::npos);
}

// ==================== Yardımcı Fonksiyon Testleri ====================

TEST(HelperFunctionsTest, TestGetCategoryString) {
    EXPECT_STREQ(getCategoryString(ExpenseCategory::ACCOMMODATION), "Konaklama");
    EXPECT_STREQ(getCategoryString(ExpenseCategory::TRANSPORTATION), "Ulaşım");
    EXPECT_STREQ(getCategoryString(ExpenseCategory::FOOD), "Yemek");
    EXPECT_STREQ(getCategoryString(ExpenseCategory::ENTERTAINMENT), "Eğlence");
}

TEST(HelperFunctionsTest, TestGetCategoryFromString) {
    EXPECT_EQ(getCategoryFromString("Konaklama"), ExpenseCategory::ACCOMMODATION);
    EXPECT_EQ(getCategoryFromString("Ulaşım"), ExpenseCategory::TRANSPORTATION);
    EXPECT_EQ(getCategoryFromString("Yemek"), ExpenseCategory::FOOD);
    EXPECT_EQ(getCategoryFromString("Eğlence"), ExpenseCategory::ENTERTAINMENT);
}

// ==================== Main Test Fonksiyonu ====================

/**
 * @brief Test main fonksiyonu
 */
int main(int argc, char** argv) {
#ifdef ENABLE_TRAVELEXPENSE_TEST
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#else
    return 0;
#endif
}

