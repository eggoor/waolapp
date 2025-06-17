cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME eifaceprov_tests)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG v1.17.0
		DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME}
	mock_e_iface_prov.h
    test_iface_prov.cpp
    test_enet_iface.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_precompile_headers(${TARGET_NAME} PRIVATE
	<gmock/gmock.h>
	<gtest/gtest.h>
)
target_link_libraries(${TARGET_NAME} gmock_main eifaceprov)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})
