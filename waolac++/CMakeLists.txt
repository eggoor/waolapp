cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME "waolac++")

project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES CXX)

add_executable(${TARGET_NAME}
	main.cpp
	Waolac.h
	Waolac.cpp
)

target_precompile_headers(${TARGET_NAME} PRIVATE
	<condition_variable>
)

target_link_libraries(${TARGET_NAME}
	PRIVATE
		${TARGET_WAOLA++}
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	find_program(SETCAP_COMMAND setcap)
	if(SETCAP_COMMAND)
		add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND sudo ${SETCAP_COMMAND} cap_net_raw+eip $<TARGET_FILE:${TARGET_NAME}>
			COMMAND_EXPAND_LISTS
			COMMENT "Setting cap_net_raw+eip capabilities on ${TARGET_NAME}"
			VERBATIM
		)
	else()
		message(WARNING "setcap not found, skipping post-build capabilities.")
	endif()
endif()

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	find_program(SETCAP_COMMAND setcap)
	if(SETCAP_COMMAND)
		install(CODE "
			message(STATUS \"Setting cap_net_raw+eip capabilities on \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/${TARGET_NAME}\")
			execute_process(
				COMMAND ${SETCAP_COMMAND} cap_net_raw+eip \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/${TARGET_NAME}
				RESULT_VARIABLE SETCAP_RESULT
			)
			if(NOT SETCAP_RESULT EQUAL 0)
				message(WARNING \"Failed to set cap_net_raw+eip capabilities on installed ${TARGET_NAME}\")
			endif()
		")
	else()
		message(WARNING "setcap not found, skipping install capabilities.")
	endif()
endif()
