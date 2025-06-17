cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME ${TARGET_WAOLA++}Tests)
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
	MockIInterfaceProvider.h
	MockIScanner.h
	MockIVault.h
	MockIKnockerUpper.h
	NetworkTestFixture.h
	TestIpAddress.cpp
	TestMacAddress.cpp
	TestSubnet.cpp
	TestNetwork.cpp
	TestNetworkInterface.cpp
	TestArper.cpp
	TestHostEditable.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_precompile_headers(${TARGET_NAME} PRIVATE
	<memory>
	<gmock/gmock.h>
	<gtest/gtest.h>
)
target_link_libraries(${TARGET_NAME} PRIVATE gmock_main ${TARGET_WAOLA++})

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})

target_compile_definitions(${TARGET_NAME} PRIVATE CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD})