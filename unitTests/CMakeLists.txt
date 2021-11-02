cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER clang++.exe)
set(CMAKE_C_COMPILER clang.exe)
# set(CMAKE_CXX_FLAGS " -Wall -Wextra  -Wpedantic")
set(CMAKE_CXX_FLAGS "/std:c++17")
set(CXX_STANDARD 17)

project(Tamagotchi_unit_tests LANGUAGES CXX)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build"
)

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include"
    )
endif()

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
set(ESP_PROJECT ${PROJECT_SOURCE_DIR}/../components/)

set(SOURCES
  ${ESP_PROJECT}/esp/ST7789Driver/ST7789Utils.cpp
  RecyclingContainerTests.cpp
  ST7789UtilsTests.cpp
)

add_executable(tests ${SOURCES})
target_link_libraries(tests gtest gtest_main)

target_include_directories(tests
  PRIVATE
  ${ESP_PROJECT}/esp/ST7789Driver/include
  ${ESP_PROJECT}/generic/RecyclingContainer/include
)
