cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME eifaceprov)

project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES C)

set(src_files
	include/e_iface_prov.h
	include/e_iface_prov_platform.h
	include/e_net_iface.h
	iface_prov.h iface_prov.c
	net_iface.h net_iface.c
	common.h
	iface_prov_priv.h
)

if(UNIX AND NOT WIN32)
	list(APPEND src_files
		iface_prov_u.h iface_prov_u.c
	)
elseif(WIN32)
	list(APPEND src_files
		include/e_iface_prov_win.h
		iface_prov_w.c
		helper.h helper.c
	)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	list(APPEND src_files iface_prov_l.c)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin") # macOS
	list(APPEND src_files iface_prov_m.c)
endif()

add_library(${TARGET_NAME} STATIC ${src_files}
	include/e_iface_prov_platform.h
	include/e_iface_prov_win.h)

if(WIN32)
	set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER
		"include/e_iface_prov.h;include/e_iface_prov_platform.h;include/e_net_iface.h;include/e_iface_prov_win.h"
	)
	target_link_libraries(${TARGET_NAME}
		PUBLIC
			ws2_32
			Iphlpapi
	)
else()
	set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER
		"include/e_iface_prov.h;include/e_iface_prov_platform.h;include/e_net_iface.h"
	)
endif()

set(pch_files
	<stdlib.h>
	<string.h>
	"include/e_iface_prov_platform.h"
)

if(UNIX AND NOT WIN32)
	list(APPEND pch_files
		<unistd.h>
		<ifaddrs.h>
		<sys/ioctl.h>
		<sys/un.h>
		<net/ethernet.h>
		<net/if.h>
		<netinet/in.h>
	)
elseif(WIN32)
	list(APPEND pch_files
		<winsock2.h>
		<iphlpapi.h>
		<Ws2ipdef.h>
	)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	list(APPEND pch_files <sys/socket.h>)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin") # macOS
	list(APPEND pch_files
		<net/if_dl.h>
		<stdint.h>
	)
endif()

target_include_directories(${TARGET_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${TARGET_NAME}>
)

target_precompile_headers(${TARGET_NAME} PUBLIC ${pch_files})

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${TARGET_NAME}
)

enable_testing()
add_subdirectory(tests)
