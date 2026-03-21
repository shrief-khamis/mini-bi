add_library(minibi_data
    table/DataTable.cpp
    table/DataTable.h
    ingestion/CSVReader.cpp
    ingestion/CSVReader.h
)

target_link_libraries(minibi_data
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        minibi_logging
)

target_include_directories(minibi_data
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)
