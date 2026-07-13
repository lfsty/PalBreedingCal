cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(PalBreeding LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "c:/Qt/Qt5.14.2/5.14.2/msvc2017_64") # Qt Kit Dir

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(Qt5 REQUIRED COMPONENTS Gui Core Widgets)

set(linkLibrariesList
    Qt5::Widgets
)

file(GLOB_RECURSE ${PROJECT_NAME}_SRC
    ${CMAKE_CURRENT_LIST_DIR}/code/src/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/code/src/*.h
    ${CMAKE_CURRENT_LIST_DIR}/code/inc/*.h
    ${CMAKE_CURRENT_LIST_DIR}/code/ui/*.ui
    ${CMAKE_CURRENT_LIST_DIR}/code/resources/resource.qrc
)

add_executable(${PROJECT_NAME} WIN32 ${${PROJECT_NAME}_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/code/ui"
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/code/inc)

target_link_libraries(${PROJECT_NAME} PRIVATE ${linkLibrariesList})