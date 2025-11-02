/**
 * @file expenseManager.cpp
 * @brief Seyahat Gideri Takibi - Gider Yönetimi Implementation
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/expenseManager.h"
#include "../header/fileIO.h"
#include <fstream>
#include <ctime>

namespace TravelExpense {

    namespace ExpenseManager {

        ErrorCode logExpense(const Expense& expense, int32_t& expenseId) {
            if (expense.tripId <= 0 || expense.amount <= 0) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            Expense newExpense = expense;
            newExpense.expenseId = FileIO::generateId("expense");
            newExpense.createdAt = time(nullptr);

            FileIO::ensureDataDirectory();
            std::string filename = "data/expenses.dat";
            std::ofstream file(filename, std::ios::binary | std::ios::app);
            
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_IO;
            }

            file.write(reinterpret_cast<const char*>(&newExpense), sizeof(Expense));
            file.close();

            expenseId = newExpense.expenseId;
            return ErrorCode::SUCCESS;
        }

        ErrorCode getExpenses(int32_t tripId, std::vector<Expense>& expenses) {
            expenses.clear();
            
            std::string filename = "data/expenses.dat";
            std::ifstream file(filename, std::ios::binary);
            
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            Expense expense;
            while (file.read(reinterpret_cast<char*>(&expense), sizeof(Expense))) {
                if (expense.tripId == tripId) {
                    expenses.push_back(expense);
                }
            }
            
            file.close();
            return ErrorCode::SUCCESS;
        }

        ErrorCode updateExpense(int32_t expenseId, const Expense& expense) {
            // TODO: TripManager::updateTrip benzeri implementasyon
            return ErrorCode::SUCCESS;
        }

        ErrorCode deleteExpense(int32_t expenseId) {
            // TODO: Soft delete veya gerçek silme implementasyonu
            return ErrorCode::SUCCESS;
        }

        ErrorCode getExpense(int32_t expenseId, Expense& expense) {
            std::string filename = "data/expenses.dat";
            std::ifstream file(filename, std::ios::binary);
            
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            Expense tempExpense;
            bool found = false;
            
            while (file.read(reinterpret_cast<char*>(&tempExpense), sizeof(Expense))) {
                if (tempExpense.expenseId == expenseId) {
                    expense = tempExpense;
                    found = true;
                    break;
                }
            }
            
            file.close();

            return found ? ErrorCode::SUCCESS : ErrorCode::ERROR_INVALID_INPUT;
        }
    }

} // namespace TravelExpense

