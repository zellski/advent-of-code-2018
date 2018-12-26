cmake_minimum_required(VERSION 3.0.0)
project(AdventOfCode2018 VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
link_libraries(${CONAN_LIBS})
include_directories(${CONAN_INCLUDE_DIRS})

list(APPEND COMMON_SOURCES utils.hpp)

add_executable(day1 ${COMMON_SOURCES} day1.cpp)
add_executable(day2 ${COMMON_SOURCES} day2.cpp)
add_executable(day3 ${COMMON_SOURCES} day3.cpp)
add_executable(day4 ${COMMON_SOURCES} day4.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
