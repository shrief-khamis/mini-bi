add_library(minibi_logging
    Log.cpp
    Log.h
)

target_link_libraries(minibi_logging PUBLIC Qt${QT_VERSION_MAJOR}::Core)

target_include_directories(minibi_logging
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)
