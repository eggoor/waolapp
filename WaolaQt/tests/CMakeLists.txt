cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME ${TARGET_WAOLAQT}Tests)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

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
	MockINetwork.h
	MockIPersistor.h
	NetworkModelTestFixture.h
	NetworkModelTest.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_precompile_headers(${TARGET_NAME} PRIVATE
	<gmock/gmock.h>
	<gtest/gtest.h>
)
target_link_libraries(${TARGET_NAME} PRIVATE gmock_main ${TARGET_WAOLAQT})

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})
