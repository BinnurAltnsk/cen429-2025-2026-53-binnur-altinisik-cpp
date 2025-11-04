# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.12)
project(travelexpenseapp)

set(ROOT src)

# Print useful information
message(STATUS "[${ROOT}] CMake version ${CMAKE_VERSION}")
message(STATUS "[${ROOT}] System ${CMAKE_SYSTEM_NAME}")
message(STATUS "[${ROOT}] Processor ${CMAKE_SYSTEM_PROCESSOR}")

# GoogleTest requires at least C++11
if(NOT "${CMAKE_CXX_STANDARD}")
  set(CMAKE_CXX_STANDARD 11)
  message(STATUS "[${ROOT}] Default C++ Standard Selected: ${CMAKE_CXX_STANDARD}")
endif()

message(STATUS "[${ROOT}] C++ standard version: ${CMAKE_CXX_STANDARD}")

# Set the default installation directory
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" CACHE PATH "Default installation directory" FORCE)
endif()

message(STATUS "[${ROOT}] Folder is set to : ${CMAKE_INSTALL_PREFIX}")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Set build configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

#crypto++ settings, disable testing, documentation generation, install and DLL generation
set(CRYPTOPP_BUILD_TESTING OFF)
set(CRYPTOPP_BUILD_DOCUMENTATION OFF)
set(CRYPTOPP_INSTALL ON)
set(CRYPTOPP_BUILD_SHARED OFF)
set(CRYPTOPP_USE_MASTER_BRANCH FALSE)


option(ENABLE_TRAVELEXPENSE "Enable Travelexpense Module" ON)
option(ENABLE_TRAVELEXPENSE_APP "Enable Travelexpense Application" ON)
option(ENABLE_TESTS "Enable All Tests" ON)

# Configure tests
# ENABLE_TRAVELEXPENSE_TEST artık sadece test target'ında tanımlı (test/CMakeLists.txt)
# Global tanımlama kaldırıldı - her test target'ı kendi tanımını ekler


# Configure logs

add_compile_definitions(ENABLE_TRAVELEXPENSE_LOGGER)

# Set the output directories for Debug and Release configurations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/Release)

# Custom definitions based on environment
if(MSVC)
  add_definitions(-DMSVC_ENV)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-DGCC_ENV)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_definitions(-DMAC_ENV)
endif()

# Set Debug and Release configuration flags
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  message(STATUS "[${ROOT}] Setting Ubuntu/MacOS C/C++ Flags")

  set(COMP_WARNINGS "-Wall -Wextra -Wunused-function -Wno-unknown-pragmas")
  set(COMP_PROFILE "-fprofile-arcs -ftest-coverage")
  set(COMP_OPTIMIZATION "-O0") #for debugging
  set(COMP_SHARED "-shared -fPIC")

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g ${COMP_OPTIMIZATION} ${COMP_WARNINGS} ${COMP_PROFILE}")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g ${COMP_OPTIMIZATION} ${COMP_WARNINGS} ${COMP_PROFILE}")

  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${COMP_WARNINGS} ${COMP_PROFILE}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMP_WARNINGS} ${COMP_PROFILE}")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  message(STATUS "[${ROOT}] Setting Windows C/C++ Flags")
  
  # Runtime library çakışmasını önlemek için CMake'in modern API'sini kullan
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  
  # Paralel derleme sırasında PDB dosyasına yazma çakışmasını önlemek için /FS flag'ini ekle
  # /FS flag'i, birden fazla CL.EXE'nin aynı PDB dosyasına yazmasına izin verir
  # Global olarak tüm target'lara uygula
  add_compile_options(/FS)
  
  # Ayrıca Debug ve Release modları için de ayrı ayrı ekle (güvenlik için)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FS")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /FS")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /FS")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /FS")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /FS")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /FS")
  
  # Geriye dönük uyumluluk için eski yöntem (CMAKE_MSVC_RUNTIME_LIBRARY yeterli olmalı)
  # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  # set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
  # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  # set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
else()
  message(WARNING "[${ROOT}] Not supported with the current compiler.")
endif()




# Find SQLite3
# Önce proje içindeki amalgamation dosyalarını kontrol et
set(SQLITE3_AMALGAMATION_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${ROOT}/travelexpense/third_party/sqlite3")
set(SQLITE3_AMALGAMATION_C "${SQLITE3_AMALGAMATION_DIR}/sqlite3.c")
set(SQLITE3_AMALGAMATION_H "${SQLITE3_AMALGAMATION_DIR}/sqlite3.h")

if(EXISTS "${SQLITE3_AMALGAMATION_C}" AND EXISTS "${SQLITE3_AMALGAMATION_H}")
    # Amalgamation dosyaları bulundu - proje içinde kullan
    message(STATUS "[${ROOT}] Using SQLite3 amalgamation from project")
    set(SQLite3_FOUND TRUE)
    set(SQLite3_INCLUDE_DIRS "${SQLITE3_AMALGAMATION_DIR}")
    set(SQLite3_LIBRARIES "") # Statik link edilecek
    set(SQLITE3_AMALGAMATION TRUE)
else()
    # Sistem SQLite3'ü ara
    message(STATUS "[${ROOT}] SQLite3 amalgamation not found, searching system SQLite3...")
    find_package(SQLite3 QUIET)
    if(SQLite3_FOUND)
        message(STATUS "[${ROOT}] SQLite3 found: ${SQLite3_VERSION}")
        message(STATUS "[${ROOT}] SQLite3 libraries: ${SQLite3_LIBRARIES}")
        message(STATUS "[${ROOT}] SQLite3 include dirs: ${SQLite3_INCLUDE_DIRS}")
        set(SQLITE3_AMALGAMATION FALSE)
    else()
        message(FATAL_ERROR "[${ROOT}] SQLite3 not found! Please:")
        message(FATAL_ERROR "  Option 1: Download SQLite3 amalgamation:")
        message(FATAL_ERROR "    https://www.sqlite.org/download.html")
        message(FATAL_ERROR "    Copy sqlite3.c and sqlite3.h to:")
        message(FATAL_ERROR "    ${SQLITE3_AMALGAMATION_DIR}/")
        message(FATAL_ERROR "  Option 2: Install SQLite3 system-wide or via vcpkg")
    endif()
endif()

# Logger submodule
if(ENABLE_TRAVELEXPENSE)
	add_subdirectory(${ROOT}/travelexpense)
endif()

# BigInteger for Crypto submodule
if(ENABLE_TRAVELEXPENSE_APP)
	add_subdirectory(${ROOT}/travelexpenseapp)
endif()

# Tests
if(ENABLE_TESTS)
	add_subdirectory(${ROOT}/tests)
endif()

# Include the Google Test framework
# add_subdirectory(src/tests/googletest)


