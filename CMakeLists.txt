cmake_minimum_required(VERSION 3.5)
project(PalBreeding LANGUAGES CXX)

if(BUILD_TESTS)
    enable_testing()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_VERSION_MAJOR 5)

if(MSVC)
    add_compile_options("/utf-8")
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/code)
