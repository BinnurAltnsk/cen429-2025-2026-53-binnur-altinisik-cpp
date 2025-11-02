/**
 * @file budgetManager.cpp
 * @brief Seyahat Gideri Takibi - Bütçe Yönetimi Implementation
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/budgetManager.h"
#include "../header/fileIO.h"
#include <fstream>
#include <ctime>

namespace TravelExpense {

    namespace BudgetManager {

        ErrorCode setBudget(int32_t tripId, double totalBudget, 
                           const double* categoryBudgets, int32_t& budgetId) {
            if (tripId <= 0 || totalBudget <= 0 || !categoryBudgets) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            Budget newBudget;
            newBudget.budgetId = FileIO::generateId("budget");
            newBudget.tripId = tripId;
            newBudget.totalBudget = totalBudget;
            newBudget.spentAmount = 0.0;
            
            for (int i = 0; i < 4; i++) {
                newBudget.categoryBudgets[i] = categoryBudgets[i];
                newBudget.categorySpent[i] = 0.0;
            }
            
            newBudget.createdAt = time(nullptr);
            newBudget.updatedAt = time(nullptr);

            FileIO::ensureDataDirectory();
            std::string filename = "data/budgets.dat";
            std::ofstream file(filename, std::ios::binary | std::ios::app);
            
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_IO;
            }

            file.write(reinterpret_cast<const char*>(&newBudget), sizeof(Budget));
            file.close();

            budgetId = newBudget.budgetId;
            return ErrorCode::SUCCESS;
        }

        ErrorCode getBudget(int32_t tripId, Budget& budget) {
            std::string filename = "data/budgets.dat";
            std::ifstream file(filename, std::ios::binary);
            
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            Budget tempBudget;
            bool found = false;
            
            while (file.read(reinterpret_cast<char*>(&tempBudget), sizeof(Budget))) {
                if (tempBudget.tripId == tripId) {
                    budget = tempBudget;
                    found = true;
                    break;
                }
            }
            
            file.close();

            return found ? ErrorCode::SUCCESS : ErrorCode::ERROR_INVALID_INPUT;
        }

        ErrorCode updateBudget(int32_t budgetId, const Budget& budget) {
            // TODO: TripManager::updateTrip benzeri implementasyon
            return ErrorCode::SUCCESS;
        }

        bool checkBudgetLimit(int32_t tripId, ExpenseCategory category, double amount) {
            Budget budget;
            if (getBudget(tripId, budget) != ErrorCode::SUCCESS) {
                return false; // Bütçe bulunamazsa izin verilmez
            }

            int categoryIndex = static_cast<int>(category);
            if (categoryIndex < 0 || categoryIndex >= 4) {
                return false;
            }

            return (budget.categorySpent[categoryIndex] + amount) <= budget.categoryBudgets[categoryIndex];
        }
    }

} // namespace TravelExpense

