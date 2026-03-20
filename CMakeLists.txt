cmake_minimum_required(VERSION 3.16)

project(MiniBIQtApp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_subdirectory(src/ui)
add_subdirectory(src/app)

add_executable(minibi
    src/main.cpp
)

target_link_libraries(minibi PRIVATE minibi_app)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(minibi)
endif()
