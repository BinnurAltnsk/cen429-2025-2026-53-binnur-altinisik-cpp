/**
 * @file reportGenerator.cpp
 * @brief Seyahat Gideri Takibi - Rapor Oluşturma Implementation
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/reportGenerator.h"
#include "../header/tripManager.h"
#include "../header/expenseManager.h"
#include "../header/budgetManager.h"
#include "../header/helpers.h"
#include <fstream>
#include <sstream>

namespace TravelExpense {

    namespace ReportGenerator {

        ErrorCode generateReport(int32_t tripId, std::string& report) {
            Trip trip;
            if (TripManager::getTrip(tripId, trip) != ErrorCode::SUCCESS) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            std::vector<Expense> expenses;
            if (ExpenseManager::getExpenses(tripId, expenses) != ErrorCode::SUCCESS) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            Budget budget;
            BudgetManager::getBudget(tripId, budget);

            std::ostringstream oss;
            
            oss << "=== SEYAHAT ÖZET RAPORU ===\n\n";
            oss << "Seyahat ID: " << trip.tripId << "\n";
            oss << "Varış Noktası: " << trip.destination << "\n";
            oss << "Başlangıç Tarihi: " << trip.startDate << "\n";
            oss << "Bitiş Tarihi: " << trip.endDate << "\n";
            oss << "Konaklama: " << trip.accommodation << "\n";
            oss << "Ulaşım: " << trip.transportation << "\n";
            oss << "\n=== GİDERLER ===\n";
            
            double totalExpenses = 0.0;
            for (const auto& expense : expenses) {
                oss << "- " << getCategoryString(expense.category) 
                    << ": " << expense.amount << " " << expense.currency
                    << " (" << expense.date << ")\n";
                totalExpenses += expense.amount;
            }
            
            oss << "\nToplam Gider: " << totalExpenses << "\n";
            oss << "Bütçe: " << budget.totalBudget << "\n";
            oss << "Kalan: " << (budget.totalBudget - totalExpenses) << "\n";

            report = oss.str();
            return ErrorCode::SUCCESS;
        }

        ErrorCode getReports(int32_t userId, std::vector<std::string>& reports) {
            // TODO: Kullanıcıya ait tüm seyahatlerin raporlarını oluştur
            reports.clear();
            return ErrorCode::SUCCESS;
        }

        ErrorCode saveReport(int32_t tripId, const char* filepath) {
            std::string report;
            if (generateReport(tripId, report) != ErrorCode::SUCCESS) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            std::ofstream file(filepath);
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_IO;
            }

            file << report;
            file.close();

            return ErrorCode::SUCCESS;
        }
    }

} // namespace TravelExpense

