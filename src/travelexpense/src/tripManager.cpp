/**
 * @file tripManager.cpp
 * @brief Seyahat Gideri Takibi - Seyahat Yönetimi Implementation
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/tripManager.h"
#include "../header/fileIO.h"
#include <fstream>
#include <ctime>

namespace TravelExpense {

    namespace TripManager {

        ErrorCode createTrip(const Trip& trip, int32_t& tripId) {
            if (trip.userId <= 0) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            Trip newTrip = trip;
            newTrip.tripId = FileIO::generateId("trip");
            newTrip.createdAt = time(nullptr);
            newTrip.updatedAt = time(nullptr);

            FileIO::ensureDataDirectory();
            std::string filename = "data/trips.dat";
            std::ofstream file(filename, std::ios::binary | std::ios::app);
            
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_IO;
            }

            file.write(reinterpret_cast<const char*>(&newTrip), sizeof(Trip));
            file.close();

            tripId = newTrip.tripId;
            return ErrorCode::SUCCESS;
        }

        ErrorCode getTrips(int32_t userId, std::vector<Trip>& trips) {
            trips.clear();
            
            std::string filename = "data/trips.dat";
            std::ifstream file(filename, std::ios::binary);
            
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            Trip trip;
            while (file.read(reinterpret_cast<char*>(&trip), sizeof(Trip))) {
                if (trip.userId == userId) {
                    trips.push_back(trip);
                }
            }
            
            file.close();
            return ErrorCode::SUCCESS;
        }

        ErrorCode updateTrip(int32_t tripId, const Trip& trip) {
            std::string filename = "data/trips.dat";
            std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
            
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            Trip tempTrip;
            bool found = false;
            std::streampos pos = 0;

            while (file.read(reinterpret_cast<char*>(&tempTrip), sizeof(Trip))) {
                if (tempTrip.tripId == tripId) {
                    found = true;
                    pos = file.tellg() - static_cast<std::streampos>(sizeof(Trip));
                    break;
                }
            }

            if (!found) {
                file.close();
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            Trip updatedTrip = trip;
            updatedTrip.tripId = tripId;
            updatedTrip.updatedAt = time(nullptr);

            file.seekp(pos);
            file.write(reinterpret_cast<const char*>(&updatedTrip), sizeof(Trip));
            file.close();

            return ErrorCode::SUCCESS;
        }

        ErrorCode deleteTrip(int32_t tripId) {
            // TODO: Soft delete veya gerçek silme implementasyonu
            // Şimdilik placeholder
            return ErrorCode::SUCCESS;
        }

        ErrorCode getTrip(int32_t tripId, Trip& trip) {
            std::string filename = "data/trips.dat";
            std::ifstream file(filename, std::ios::binary);
            
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            Trip tempTrip;
            bool found = false;
            
            while (file.read(reinterpret_cast<char*>(&tempTrip), sizeof(Trip))) {
                if (tempTrip.tripId == tripId) {
                    trip = tempTrip;
                    found = true;
                    break;
                }
            }
            
            file.close();

            return found ? ErrorCode::SUCCESS : ErrorCode::ERROR_INVALID_INPUT;
        }
    }

} // namespace TravelExpense

