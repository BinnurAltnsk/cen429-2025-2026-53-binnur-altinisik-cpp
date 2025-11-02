/**
 * @file fileIO.cpp
 * @brief Seyahat Gideri Takibi - Dosya I/O Yardımcı Fonksiyonlar Implementation
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#include "../header/fileIO.h"
#include <fstream>
#include <string>

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
    #define MKDIR(dir) _mkdir(dir.c_str())
    #define ACCESS(path, mode) _access(path, mode)
#else
    #include <sys/stat.h>
    #include <unistd.h>
    #define MKDIR(dir) mkdir(dir.c_str(), 0755)
    #define ACCESS(path, mode) access(path, mode)
#endif

namespace TravelExpense {

    namespace FileIO {

        bool ensureDataDirectory() {
            std::string dataDir = "data";
            
            #ifdef _WIN32
                if (ACCESS(dataDir.c_str(), 0) != 0) {
                    return MKDIR(dataDir) == 0;
                }
            #else
                struct stat info;
                if (stat(dataDir.c_str(), &info) != 0) {
                    return MKDIR(dataDir) == 0;
                }
            #endif
            
            return true;
        }

        int32_t getLastId(const char* filename) {
            std::string idFile = std::string("data/") + filename + ".id";
            std::ifstream file(idFile, std::ios::binary);
            
            if (!file.is_open()) {
                return 0;
            }
            
            int32_t lastId = 0;
            file.read(reinterpret_cast<char*>(&lastId), sizeof(int32_t));
            file.close();
            
            return lastId;
        }

        bool saveLastId(const char* filename, int32_t id) {
            ensureDataDirectory();
            std::string idFile = std::string("data/") + filename + ".id";
            std::ofstream file(idFile, std::ios::binary);
            
            if (!file.is_open()) {
                return false;
            }
            
            file.write(reinterpret_cast<const char*>(&id), sizeof(int32_t));
            file.close();
            
            return true;
        }

        int32_t generateId(const char* entityName) {
            int32_t lastId = getLastId(entityName);
            int32_t newId = lastId + 1;
            saveLastId(entityName, newId);
            return newId;
        }

    } // namespace FileIO

} // namespace TravelExpense

