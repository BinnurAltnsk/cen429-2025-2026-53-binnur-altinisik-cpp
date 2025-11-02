/**
 * @file userAuth.cpp
 * @brief Seyahat Gideri Takibi - Kullanıcı Kimlik Doğrulama Implementation
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/userAuth.h"
#include "../header/fileIO.h"
#include <cstring>
#include <fstream>
#include <ctime>

namespace TravelExpense {

    namespace UserAuth {
        
        // Global mevcut kullanıcı pointer'ı
        static User* currentUser = nullptr;

        ErrorCode registerUser(const char* username, const char* password) {
            if (!username || !password || strlen(username) == 0 || strlen(password) == 0) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            // TODO: Şifre hash'leme (SHA-256) implementasyonu eklenecek
            // TODO: Salt oluşturma implementasyonu eklenecek
            
            // Şimdilik basit bir kontrol
            if (strlen(username) >= 50) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            User newUser;
            newUser.userId = FileIO::generateId("user");
            strncpy(newUser.username, username, sizeof(newUser.username) - 1);
            newUser.username[sizeof(newUser.username) - 1] = '\0';
            
            // TODO: passwordHash ve salt gerçek implementasyon ile doldurulacak
            // Şimdilik placeholder
            strncpy(newUser.passwordHash, "placeholder_hash", sizeof(newUser.passwordHash) - 1);
            strncpy(newUser.salt, "placeholder_salt", sizeof(newUser.salt) - 1);
            
            newUser.isGuest = false;
            newUser.createdAt = time(nullptr);
            newUser.lastLogin = 0;

            // Dosyaya kaydet (şimdilik binary, ileride şifrelenmiş)
            FileIO::ensureDataDirectory();
            std::string filepath = "data/users.dat";
            std::ofstream outFile(filepath, std::ios::binary | std::ios::app);
            
            if (!outFile.is_open()) {
                return ErrorCode::ERROR_FILE_IO;
            }

            outFile.write(reinterpret_cast<const char*>(&newUser), sizeof(User));
            outFile.close();

            return ErrorCode::SUCCESS;
        }

        ErrorCode loginUser(const char* username, const char* password) {
            if (!username || !password) {
                return ErrorCode::ERROR_INVALID_INPUT;
            }

            // TODO: Gerçek authentication implementasyonu
            // Şimdilik basit bir kontrol
            
            std::string filename = "data/users.dat";
            std::ifstream file(filename, std::ios::binary);
            
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            User user;
            bool found = false;
            
            while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
                if (strcmp(user.username, username) == 0) {
                    // TODO: Şifre hash kontrolü yapılacak
                    // Şimdilik basit kontrol
                    found = true;
                    break;
                }
            }
            
            file.close();

            if (!found) {
                return ErrorCode::ERROR_INVALID_USER;
            }

            // Mevcut kullanıcıyı ayarla
            if (currentUser != nullptr) {
                delete currentUser;
            }
            
            currentUser = new User(user);
            currentUser->lastLogin = time(nullptr);

            return ErrorCode::SUCCESS;
        }

        ErrorCode enableGuestMode() {
            if (currentUser != nullptr) {
                delete currentUser;
            }

            currentUser = new User();
            currentUser->userId = -1; // Guest kullanıcı ID'si
            currentUser->isGuest = true;
            strncpy(currentUser->username, "Guest", sizeof(currentUser->username) - 1);
            currentUser->createdAt = time(nullptr);

            return ErrorCode::SUCCESS;
        }

        void logoutUser() {
            if (currentUser != nullptr) {
                delete currentUser;
                currentUser = nullptr;
            }
        }

        User* getCurrentUser() {
            return currentUser;
        }

        ErrorCode getUserById(int32_t userId, User& user) {
            std::string filename = "data/users.dat";
            std::ifstream file(filename, std::ios::binary);
            
            if (!file.is_open()) {
                return ErrorCode::ERROR_FILE_NOT_FOUND;
            }

            User tempUser;
            bool found = false;
            
            while (file.read(reinterpret_cast<char*>(&tempUser), sizeof(User))) {
                if (tempUser.userId == userId) {
                    user = tempUser;
                    found = true;
                    break;
                }
            }
            
            file.close();

            return found ? ErrorCode::SUCCESS : ErrorCode::ERROR_INVALID_USER;
        }
    }

} // namespace TravelExpense

