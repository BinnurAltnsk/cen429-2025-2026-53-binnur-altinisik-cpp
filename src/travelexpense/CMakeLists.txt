# crypto/CMakeLists.txt
set(ROOT src)
set(LIBNAME travelexpense)

message(STATUS "[${ROOT}/${LIBNAME}] Module Processing...")

# Collect files without having to explicitly list each header and source file
file(GLOB LIB_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/header/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/header/*.hpp")

file(GLOB LIB_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")

# Create named folders for the sources within the project
source_group("header" FILES ${LIB_HEADERS})
source_group("src" FILES ${LIB_SOURCES})

# Set Properties->General->Configuration Type to Dynamic Library (.dll/.so/.dylib)
add_library(${LIBNAME} SHARED ${LIB_HEADERS} ${LIB_SOURCES}) # Dynamic library (DLL/SO)

target_include_directories(${LIBNAME} PUBLIC
						   ${CMAKE_CURRENT_SOURCE_DIR}/header
						   ${SQLite3_INCLUDE_DIRS})

# Link SQLite3
if(SQLITE3_AMALGAMATION)
    # Amalgamation dosyasını direkt ekle (statik link)
    target_sources(${LIBNAME} PRIVATE 
        ${SQLITE3_AMALGAMATION_C}
    )
    # SQLite3'ü C dosyası olarak derle
    set_source_files_properties(${SQLITE3_AMALGAMATION_C} PROPERTIES LANGUAGE C)
    
    # Windows'ta SQLite3 için runtime library ayarını eşleştir
    # SQLite3 C dosyası da aynı runtime library'yi kullanmalı
    if(MSVC)
        # SQLite3 C dosyası için de aynı runtime library'yi kullan
        # Multi-configuration build için genel ayar
        set_source_files_properties(${SQLITE3_AMALGAMATION_C} PROPERTIES 
            COMPILE_OPTIONS "$<$<CONFIG:Debug>:/MTd>;$<$<CONFIG:Release>:/MT>"
        )
    endif()
else()
    # Sistem SQLite3 kütüphanesini link et
    target_link_libraries(${LIBNAME} PRIVATE ${SQLite3_LIBRARIES})
endif()

# Platform specific settings for DLL/SO
if(WIN32)
    # Windows: DLL export definitions
    set_target_properties(${LIBNAME} PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
else()
    # Linux/Unix: Set visibility for exported symbols
    set_target_properties(${LIBNAME} PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )
endif()

# creates preprocessor definition used for library exports
target_compile_definitions(${LIBNAME} PRIVATE "TRAVELEXPENSE_LIB_EXPORTS")
target_compile_definitions(${LIBNAME} INTERFACE "TRAVELEXPENSE_LIB_EXPORTS")

install(TARGETS ${LIBNAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin )
		
# Copy required header to the installation include folder		
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/header/travelexpense.h
        DESTINATION include)

# Export the crypto target so other modules can use it
# export(TARGETS ${LIBNAME} FILE ${LIBNAME}Targets.cmake)

message(STATUS "[${ROOT}/${LIBNAME}] Added library target: ${LIBNAME}")
