set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCES_LIST
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mainwindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PalManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PalModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BreedingListView.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BreedingModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BreedingView.cpp
)

set(HEADERS_LIST
    ${CMAKE_CURRENT_LIST_DIR}/inc/mainwindow.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/PalManager.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/PalModel.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/BreedingListView.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/BreedingModel.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/BreedingView.h
)

set(RESOURCES_LIST
    ${CMAKE_CURRENT_LIST_DIR}/resources/resource.qrc
)

set(UI_LIST
    ${CMAKE_CURRENT_LIST_DIR}/ui/mainwindow.ui
)

add_executable(${PROJECT_NAME}
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${RESOURCES_LIST}
    ${UI_LIST}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE $<$<AND:$<PLATFORM_ID:Windows>,$<CONFIG:Release>>:TRUE>
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

find_package(Qt6 COMPONENTS Core QUIET)
if (NOT Qt6_FOUND)
    set(QT_VERSION_MAJOR 5)
elseif()
    set(QT_VERSION_MAJOR 6)
endif()
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Concurrent)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Concurrent
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)
