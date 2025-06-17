if(NOT DEFINED TARGET_WAOLA++)
	set(TARGET_WAOLA++ Waola++)
	if (CMAKE_SYSTEM_NAME MATCHES "Linux")
		string(TOLOWER ${TARGET_WAOLA++} TARGET_WAOLA++)
	endif()
endif()

cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME ${TARGET_WAOLA++})

project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES CXX)

set(src_files
	include/HostEditable.h
	include/HostSaved.h
	include/MacAddress.h
	include/MacAddress.hpp
	include/INetwork.h
	include/IInterfaceProvider.h
	include/IKnockerUpper.h
	include/IpAddress.h
	include/IpAddress.hpp
	include/IScanner.h
	include/IVault.h
	include/OpResult.h
	include/Platform.h
	include/Host.h
	include/HostField.h
	include/Waola++.h
	include/NetworkParams.h
	include/ScannerParams.h
	include/VaultParams.h
	include/VaultOpResult.h
	include/L7Result.h
	ArpResult.h
	Arper.h Arper.cpp
	ArperImpl.h ArperImpl.cpp
	Common.h
	HostEditable.cpp
	InterfaceProvider.h InterfaceProvider.cpp
	IpAddrIterator.h
	IpAddress.cpp
	IpAddrGenerator.h
	KnockerUpper.h KnockerUpper.cpp
	MacAddress.cpp
	Network.h Network.cpp
	NetworkInterface.h NetworkInterface.cpp
	Scanner.h Scanner.cpp
	Subnet.h Subnet.cpp
	SubnetGenerator.h SubnetGenerator.cpp
	Vault.cpp
)

if (UNIX)
	list(APPEND src_files
		include/DepsX.h
		ArperImplX.h ArperImplX.cpp
		ArpRequest.cpp
		KnockerUpperX.cpp
	)
elseif (WIN32)
	list(APPEND src_files
		include/DepsWin.h
		NetworkInterfaceWin.cpp
		ArperImplWin.h ArperImplWin.cpp
		KnockerUpperWin.cpp
	)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	list(APPEND src_files
		include/DepsLin.h
		NetworkInterfaceLin.cpp
		ArperImplLin.cpp
		KnockerUpperLin.cpp
	)
elseif( CMAKE_SYSTEM_NAME STREQUAL "Darwin") # macOS
	list(APPEND src_files
		include/DepsMac.h
		NetworkInterfaceMac.cpp
		ArperImplMac.cpp
		KnockerUpperMac.cpp
	)
endif()

add_library(${TARGET_NAME} STATIC ${src_files})

if (WIN32)
	set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER
		"include/HostEditable.h;include/HostSaved.h;include/MacAddress.h;include/MacAddress.hpp;include/INetwork.h;include/IpAddress.h;include/IpAddress.hpp;include/OpResult.h;include/Platform.h;include/Host.h;include/HostField.h;include/Waola++.h;include/NetworkParams.h;include/IScanner.h;include/IVault.h;include/ScannerParams.h;include/VaultParams.h;include/VaultOpResult.h;include/L7Result.h;include/DepsWin.h"
	)
elseif (UNIX AND NOT WIN32)
	if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
		set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER
			"include/HostEditable.h;include/HostSaved.h;include/MacAddress.h;include/MacAddress.hpp;include/INetwork.h;include/IpAddress.h;include/IpAddress.hpp;include/OpResult.h;include/Platform.h;include/Host.h;include/HostField.h;include/Waola++.h;include/NetworkParams.h;include/IScanner.h;include/IVault.h;include/ScannerParams.h;include/VaultParams.h;include/VaultOpResult.h;include/L7Result.h;include/DepsX.h;include/DepsLin.h"
		)
	elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin") # macOS
		set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER
			"include/HostEditable.h;include/HostSaved.h;include/MacAddress.h;include/MacAddress.hpp;include/INetwork.h;include/IpAddress.h;include/IpAddress.hpp;include/OpResult.h;include/Platform.h;include/Host.h;include/HostField.h;include/Waola++.h;include/NetworkParams.h;include/IScanner.h;include/IVault.h;include/ScannerParams.h;include/VaultParams.h;include/VaultOpResult.h;include/L7Result.h;include/DepsX.h;include/DepsMac.h"
		)
	endif()
endif()

target_compile_definitions(${TARGET_NAME} PRIVATE WAOLAPP_LIBRARY)

target_precompile_headers(${TARGET_NAME}
	PUBLIC
		<cstdint>
		<cstring>
		<memory>
		<string>
		<vector>
		<queue>
		<unordered_set>
		<unordered_map>
		<map>
		<sstream>
		<iostream>
		<iomanip>
		<functional>
		<mutex>
		<thread>
		<algorithm>
		<exception>
		<chrono>
		include/Platform.h
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_precompile_headers(${TARGET_NAME}
			PUBLIC
				<atomic>
	)
endif()

if (CMAKE_CXX_STANDARD GREATER_EQUAL 23)
	target_precompile_headers(${TARGET_NAME}
		PUBLIC
			<ranges>
	)
endif()

target_include_directories(${TARGET_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${TARGET_NAME}>
)

target_link_libraries(${TARGET_NAME}
	PUBLIC
		eifaceprov
)

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${TARGET_NAME}
)

enable_testing()
add_subdirectory(tests)
