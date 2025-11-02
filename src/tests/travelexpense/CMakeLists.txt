# tests/aka5g/CMakeLists.txt
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

# Define the target for aka5g tests
add_executable(${EXENAME} ${LIB_HEADERS} ${LIB_SOURCES})

# Add included headers
# Use the travelexpense library's include directories (automatically inherited via target_link_libraries)
target_include_directories(${EXENAME} PUBLIC
						   ${CMAKE_CURRENT_SOURCE_DIR})

# Link travelexpense library and gtest
target_link_libraries(${EXENAME} PRIVATE travelexpense gtest gtest_main)

# Register the test with CTest
# add_test(NAME ${EXENAME} COMMAND ${EXENAME})

install(TARGETS ${EXENAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin )

# Discover tests using CTest
include(GoogleTest)
gtest_discover_tests(${EXENAME})
		
message(STATUS "[${ROOT}/${TESTNAME}] Added target: ${EXENAME}")