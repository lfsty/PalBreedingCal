set(CMAKE_AUTORCC ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG  v1.17.0
    GIT_SHALLOW    ON
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(SOURCES_LIST
    ${CMAKE_CURRENT_LIST_DIR}/src/test_BreedingModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/test_DisplayPalManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/test_DisplayPalModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/test_PlayerManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/test_PlayerModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/test_PlayerPalModel.cpp
)

set(PROJECT_SOURCE_LIST
    ${CMAKE_SOURCE_DIR}/code/resources/resource.qrc
)

set(TEST_TARGET_NAME TestsPalBreedingCal)

add_executable(${TEST_TARGET_NAME} ${SOURCES_LIST} ${PROJECT_SOURCE_LIST})

find_package(Qt5 REQUIRED COMPONENTS Core)

target_link_libraries(${TEST_TARGET_NAME} PRIVATE
    gtest_main

    models
)

include(GoogleTest)
gtest_discover_tests(${TEST_TARGET_NAME} DISCOVERY_MODE PRE_TEST)