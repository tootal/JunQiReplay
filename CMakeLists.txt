cmake_minimum_required(VERSION 3.20)

project(JunQiReplay 
    VERSION 0.1.0
    LANGUAGES C CXX)
set(MODULE_NAME ${CMAKE_PROJECT_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 依赖的Qt模块
set(QT_COMPONENTS
    Widgets
    Core5Compat
)
find_package(Qt6 COMPONENTS ${QT_COMPONENTS} REQUIRED)

file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE QRC_FILES res/*.qrc)
file(GLOB_RECURSE TS_FILES res/*.ts)
file(GLOB_RECURSE UI_FILES src/*.ui)

add_executable(${MODULE_NAME} WIN32)
target_sources(${MODULE_NAME}
    PRIVATE
        ${SRC_FILES}
        ${HEADER_FILES}
        ${QRC_FILES}
        ${UI_FILES}
        ${TS_FILES})

list(TRANSFORM QT_COMPONENTS PREPEND Qt6::)
target_link_libraries(${MODULE_NAME}
    PRIVATE
        ${QT_COMPONENTS})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
    PROPERTY 
        VS_STARTUP_PROJECT ${MODULE_NAME})
