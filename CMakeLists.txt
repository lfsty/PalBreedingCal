cmake_minimum_required(VERSION 3.5)
project(PalBreeding LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options("/utf-8")
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/code)
