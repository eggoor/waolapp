cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME ${TARGET_WAOLAQ})

project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Qt6 REQUIRED COMPONENTS Quick Svg)
qt_standard_project_setup(REQUIRES 6.8)

qt_policy(SET QTP0001 NEW)

qt_add_executable(${TARGET_NAME}
	main.cpp
	res.qrc
)

qt_add_qml_module(${TARGET_NAME}
	URI ${TARGET_NAME}
	VERSION 1.0
	RESOURCES
	Converters.js
	QML_FILES
	About.qml
	AboutView.qml
	AboutViewForm.ui.qml
	CellRect.qml
	CellText.qml
	CellGeneric.qml
	CellLso.qml
	Credits.qml
	EditView.qml
	EditViewForm.ui.qml
	LabeledTextField.qml
	License.qml
	Main.qml
	MainViewForm.ui.qml
	WDelegateChooser.qml
	WTabBar.qml
	WTabButton.qml
	WToolBar.qml
	WToolButton.qml
	WToolSeparator.qml
	NetworkModelMoc.qml
	OverlayLoader.qml
	WShortcuts.qml
	MainView.qml
)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin") #macOS
	set(ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/icons/Waola.icns)
	set_source_files_properties(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	target_sources(${TARGET_NAME} PRIVATE ${ICON_FILE})
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/icons/01wakeup.svg)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/icons/Waola.ico)
	set(RESOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/res.rc)
	target_sources(${TARGET_NAME} PRIVATE ${ICON_FILE} ${RESOURCE_FILE})
endif()

set_target_properties(${TARGET_NAME}
	PROPERTIES
		MACOSX_BUNDLE_GUI_IDENTIFIER com.asuscomm.smartnclever.${TARGET_NAME}
		MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
		MACOSX_BUNDLE_ICON_FILE Waola.icns
		MACOSX_BUNDLE TRUE
		WIN32_EXECUTABLE TRUE
)

target_link_libraries(${TARGET_NAME}
	PRIVATE
		Qt${QT_VERSION_MAJOR}::Quick
		Qt${QT_VERSION_MAJOR}::Svg
		${TARGET_WAOLAQT}
)

# set_target_properties (${TARGET_NAME} PROPERTIES
# 	QT_QML_IMPORT_PATH "${CMAKE_BINARY_DIR}/${WaolaQt}"
# )

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	if(ICON_FILE)
		install(FILES ${ICON_FILE}
			DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps
			RENAME ${TARGET_NAME}.svg
		)
	endif()

	configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.desktop.in
		${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.desktop
		@ONLY
	)

	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.desktop
		DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
	)

	find_program(SETCAP_COMMAND setcap)
	if(SETCAP_COMMAND)
		add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND sudo ${SETCAP_COMMAND} cap_net_raw+eip $<TARGET_FILE:${TARGET_NAME}>
			COMMENT "Setting cap_net_raw+eip capabilities on ${TARGET_NAME}"
		)
	else()
		message(WARNING "setcap not found, skipping setting capabilities.")
	endif()
endif()

include(GNUInstallDirs)
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
	install(TARGETS ${TARGET_NAME}
		BUNDLE DESTINATION /Applications
	)
else()
	install(TARGETS ${TARGET_NAME}
		BUNDLE DESTINATION .
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	find_program(SETCAP_COMMAND setcap)
	if (SETCAP_COMMAND)
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
		message(WARNING "setcap not found, skipping setting capabilities.")
	endif()
endif()
