cmake_minimum_required(VERSION 3.0.0)
project(AdventOfCode2018 VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-fsanitize=address -fsanitize=leak")
set(CMAKE_BUILD_TYPE Debug)

include(CTest)
enable_testing()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

list(APPEND COMMON_SOURCES utils.hpp)

foreach(DAY_NUMBER RANGE 1 31)
    if(EXISTS ${CMAKE_SOURCE_DIR}/day${DAY_NUMBER}.cpp)
        add_executable(day${DAY_NUMBER} ${COMMON_SOURCES} day${DAY_NUMBER}.cpp)
        target_include_directories(
            day${DAY_NUMBER}
            PRIVATE
            ${CONAN_INCLUDE_DIRS}
            "third_party"
        )
        target_link_libraries(
            day${DAY_NUMBER}
            ${CONAN_LIBS}
        )
        target_compile_options(day${DAY_NUMBER} PRIVATE "-Wall")

    endif()
endforeach()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
