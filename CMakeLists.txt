cmake_minimum_required(VERSION 3.5)
project(PalBreeding LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCES_LIST
    ${CMAKE_CURRENT_LIST_DIR}/code/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/code/src/mainwindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/code/src/PalManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/code/src/PalModel.cpp
)

set(HEADERS_LIST
    ${CMAKE_CURRENT_LIST_DIR}/code/inc/mainwindow.h
    ${CMAKE_CURRENT_LIST_DIR}/code/inc/PalManager.h
    ${CMAKE_CURRENT_LIST_DIR}/code/inc/PalModel.h
)

set(RESOURCES_LIST
    ${CMAKE_CURRENT_LIST_DIR}/code/resources/resource.qrc
)

set(UI_LIST
    ${CMAKE_CURRENT_LIST_DIR}/code/ui/mainwindow.ui
)

set(EXE_TYPE)
if(WIN32)
    set(EXE_TYPE WIN32)
endif()

add_executable(${PROJECT_NAME}
    ${EXE_TYPE}
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${RESOURCES_LIST}
    ${UI_LIST}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/code/inc)

find_package(Qt6 COMPONENTS Core QUIET)
if (NOT Qt6_FOUND)
    set(QT_VERSION_MAJOR 5)
elseif()
    set(QT_VERSION_MAJOR 6)
endif()
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/code/ui"
)
