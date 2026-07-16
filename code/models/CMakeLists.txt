set(CMAKE_AUTOMOC ON)

set(MODULE_NAME models)

set(SOURCES_LIST
    ${CMAKE_CURRENT_LIST_DIR}/src/PalManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PalModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BreedingModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PlayerModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PlayerPalModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PlayerListModel.cpp
)

set(HEADERS_LIST
    ${CMAKE_CURRENT_LIST_DIR}/inc/PalManager.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/PalModel.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/BreedingModel.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/PlayerModel.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/PlayerPalModel.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/PlayerListModel.h
)

add_library(${MODULE_NAME} STATIC ${SOURCES_LIST})

target_sources(${MODULE_NAME} PRIVATE ${HEADERS_LIST})

target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Concurrent)

target_link_libraries(${MODULE_NAME} PUBLIC 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Concurrent
)