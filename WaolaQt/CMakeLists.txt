cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME ${TARGET_WAOLAQT})

project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.8)

qt_policy(SET QTP0001 NEW)

qt_add_library(${TARGET_NAME} STATIC)
qt_add_qml_module(${TARGET_NAME}
	URI WaolaQt
	VERSION 1.0
	SOURCES
		ENetworkModel.h ENetworkModel.cpp
		IPersistor.h
		Persistor.h Persistor.cpp
		EClipboard.h EClipboard.cpp
		ETextReader.h ETextReader.cpp
)

set_target_properties(${TARGET_NAME} PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER com.asuscomm.smartnclever.${TARGET_NAME}
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

target_compile_definitions(${TARGET_NAME}
	PRIVATE
		$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_precompile_headers(${TARGET_NAME}
	PUBLIC
		<QQmlApplicationEngine>
		<QQmlEngine>
		<QQmlContext>
		<QJsonObject>
		<QJsonArray>
		<QJsonDocument>
		<QFile>
		<QDir>
		<QStandardPaths>
		<QGuiApplication>
		<QClipboard>
		<QAbstractTableModel>
		<QQmlExtensionPlugin>
)

target_link_libraries(${TARGET_NAME}
	PUBLIC
		Qt${QT_VERSION_MAJOR}::Quick
		${TARGET_WAOLA++}
)

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qmldir
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/com/asuscomm/smartnclever/${TARGET_NAME}
)

enable_testing()
add_subdirectory(tests)
