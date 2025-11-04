# tests/travelexpense/CMakeLists.txt
set(ROOT src/tests)
set(TESTNAME travelexpense)
set(EXENAME ${TESTNAME}_tests)

message(STATUS "[${ROOT}/${TESTNAME}] Module Tests...")

# Collect files without having to explicitly list each header and source file
file(GLOB LIB_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

file(GLOB LIB_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

# Create named folders for the sources within the project
source_group("header" FILES ${LIB_HEADERS})
source_group("src" FILES ${LIB_SOURCES})

enable_testing()

# Add Google Test from project directory FIRST
# Google Test projede src/tests/googletest dizininde mevcut
# Test target'ından önce eklenmeli ki target'lar hazır olsun
set(GOOGLETEST_DIR "${CMAKE_SOURCE_DIR}/src/tests/googletest")
if(EXISTS "${GOOGLETEST_DIR}/CMakeLists.txt")
    # Google Test'i subdirectory olarak ekle
    # BUILD_GMOCK OFF yaparak sadece googletest'i kullan
    set(BUILD_GMOCK OFF CACHE BOOL "Build Google Mock" FORCE)
    # Google Test'in runtime library ayarını test target ile eşleştirmek için
    # gtest_force_shared_crt seçeneğini kullan
    # Hata mesajına göre test target MDd ile derleniyor, bu yüzden ON yapıyoruz
    # Bu sayede gtest de MD (MultiThreaded DLL) runtime library kullanacak
    set(gtest_force_shared_crt ON CACHE BOOL "Force shared CRT for gtest" FORCE)
    add_subdirectory(${GOOGLETEST_DIR} ${CMAKE_BINARY_DIR}/googletest EXCLUDE_FROM_ALL)
    message(STATUS "[${ROOT}/${TESTNAME}] Using Google Test from project directory")
else()
    # Fallback: Sistemde Google Test'i ara
    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        message(FATAL_ERROR "Google Test not found. Please ensure Google Test is available at ${GOOGLETEST_DIR}")
    endif()
endif()

# Define the target for travelexpense tests
add_executable(${EXENAME} ${LIB_HEADERS} ${LIB_SOURCES})

# Test target'ına da runtime library ayarını uygula
# Hata mesajına göre test target MDd ile derleniyor, bu yüzden MD ayarı kullanıyoruz
# Bu, travelexpense library ile uyumlu olmalı
if(MSVC)
    # Test target'ın runtime library ayarını MD (MultiThreaded DLL) olarak ayarla
    # Bu, Google Test ve diğer bağımlılıklarla uyumlu olmalı
    set_target_properties(${EXENAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>")
endif()

# Add included headers
# Use the travelexpense library's include directories (automatically inherited via target_link_libraries)
target_include_directories(${EXENAME} PUBLIC
						   ${CMAKE_CURRENT_SOURCE_DIR})

# Link travelexpense library
target_link_libraries(${EXENAME} PRIVATE travelexpense)

# Link Google Test libraries
if(TARGET gtest AND TARGET gtest_main)
    # Proje içindeki Google Test kullanılıyor
    # gtest_force_shared_crt ON yaptığımız için Google Test zaten MD ile derlenecek
    # Ekstra güvenlik için runtime library ayarını da kontrol ediyoruz
    if(MSVC)
        set_target_properties(gtest gtest_main PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>")
    endif()
    target_link_libraries(${EXENAME} PRIVATE gtest gtest_main)
    # Google Test include dizinlerini ekle
    target_include_directories(${EXENAME} PRIVATE ${GOOGLETEST_DIR}/googletest/include)
elseif(TARGET GTest::gtest AND TARGET GTest::gtest_main)
    # Sistemdeki Google Test kullanılıyor
    target_link_libraries(${EXENAME} PRIVATE GTest::gtest GTest::gtest_main)
else()
    message(FATAL_ERROR "Google Test libraries not found. Cannot link tests.")
endif()

# ENABLE_TRAVELEXPENSE_TEST makrosunu sadece test target'ı için tanımla
target_compile_definitions(${EXENAME} PRIVATE ENABLE_TRAVELEXPENSE_TEST)

# Register the test with CTest
# add_test(NAME ${EXENAME} COMMAND ${EXENAME})

install(TARGETS ${EXENAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin )
		
message(STATUS "[${ROOT}/${TESTNAME}] Added target: ${EXENAME}")
