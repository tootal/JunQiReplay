cmake_minimum_required(VERSION 3.20)

project(JunQiReplay 
    VERSION 0.1.0.0
    DESCRIPTION "An application for QQGame JunQi Replay Manager."
    LANGUAGES C CXX)
set(PROJECT_AUTHOR "tootal")
set(CMAKE_PROJECT_AUTHOR "${PROJECT_AUTHOR}")
set(MODULE_NAME ${CMAKE_PROJECT_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 依赖的Qt模块
set(QT_COMPONENTS
    Core
    Widgets
    Quick
    Core5Compat
)
find_package(Qt6 COMPONENTS ${QT_COMPONENTS} REQUIRED)
find_package(Qt6 COMPONENTS LinguistTools REQUIRED)

file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE QRC_FILES res/*.qrc)
file(GLOB_RECURSE TS_FILES res/*.ts)

add_executable(${MODULE_NAME} WIN32)
target_sources(${MODULE_NAME}
    PRIVATE
        ${SRC_FILES}
        ${HEADER_FILES}
        ${QRC_FILES}
        ${TS_FILES}
        mainwindowviewmodel.h
        mainwindowviewmodel.cpp)

list(TRANSFORM QT_COMPONENTS PREPEND Qt6::)
target_link_libraries(${MODULE_NAME}
    PRIVATE
        ${QT_COMPONENTS})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
    PROPERTY 
        VS_STARTUP_PROJECT ${MODULE_NAME})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/res/${MODULE_NAME}.rc.in 
    ${PROJECT_BINARY_DIR}/${MODULE_NAME}.rc)
target_sources(${MODULE_NAME}
    PRIVATE 
        ${PROJECT_BINARY_DIR}/${MODULE_NAME}.rc)

qt_add_translations(${MODULE_NAME} TS_FILES ${TS_FILES})

execute_process(
    COMMAND ${LUPDATE_EXECUTABLE} -recursive ${CMAKE_CURRENT_SOURCE_DIR}/src -ts ${TS_FILES})

qt6_add_qml_module(${MODULE_NAME}
    URI JunQiReplay
    VERSION 1.0
    NO_RESOURCE_TARGET_PATH
)

list(APPEND QML_IMPORT_PATH "${PROJECT_BINARY_DIR}")
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
    CACHE STRING "Qt Creator 4.1 extra qml import paths"
    FORCE
)
