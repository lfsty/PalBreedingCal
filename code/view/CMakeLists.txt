set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(MODULE_NAME view)

set(SOURCES_LIST
    ${CMAKE_CURRENT_LIST_DIR}/src/mainwindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BreedingListView.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BreedingView.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/playermanagerview.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PlayerView.cpp
)

set(HEADERS_LIST
    ${CMAKE_CURRENT_LIST_DIR}/inc/mainwindow.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/BreedingListView.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/BreedingView.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/playermanagerview.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/PlayerView.h
)

set(UI_LIST
    ${CMAKE_CURRENT_LIST_DIR}/ui/mainwindow.ui
    ${CMAKE_CURRENT_LIST_DIR}/ui/playermanagerview.ui
)

add_library(${MODULE_NAME} STATIC
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${UI_LIST}
)

target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Concurrent)

target_link_libraries(${MODULE_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Concurrent

    models
)

set_target_properties(${MODULE_NAME} PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)