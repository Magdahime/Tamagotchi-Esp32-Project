# This is the project CMakeLists.txt file for the test subproject
cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra  -O3 ")
set(EXTRA_COMPONENT_DIRS ../components/esp ../components/generic ../components/app $ENV{IDF_PATH}/tools/unit-test-app/components)
set(TEST_COMPONENTS RecyclingContainer EspGL SPIFFSDriver BitmapLoader PetGenerator Pet Gomoku Game Serializer CACHE STRING "List of components to test")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(Tamagotchi_unit_tests)