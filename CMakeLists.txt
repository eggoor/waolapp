cmake_minimum_required(VERSION 3.16)

project(Waola++ VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
	message(STATUS "CMAKE_BUILD_TYPE not specified, defaulting to ${CMAKE_BUILD_TYPE}")
endif()

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>)

add_subdirectory(eifaceprov)

set(TARGET_WAOLA++ Waola++)
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	string(TOLOWER ${TARGET_WAOLA++} TARGET_WAOLA++)
endif()
add_subdirectory(Waola++)

add_subdirectory(waolac++)

find_package(Qt6 COMPONENTS Quick Svg QUIET)

if (Qt6_FOUND)
	set(TARGET_WAOLAQT WaolaQt)
	if (CMAKE_SYSTEM_NAME MATCHES "Linux")
		string(TOLOWER ${TARGET_WAOLAQT} TARGET_WAOLAQT)
	endif()
	add_subdirectory(WaolaQt)

	set(TARGET_WAOLAQ WaolaQ)
	if (CMAKE_SYSTEM_NAME MATCHES "Linux")
		string(TOLOWER ${TARGET_WAOLAQ} TARGET_WAOLAQ)
	endif()
	add_subdirectory(WaolaQ)
else()
	message(STATUS "Skipping waolaq GUI frontend since Qt6 not found")
endif()

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY
)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

enable_testing()
