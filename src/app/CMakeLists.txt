add_library(minibi_app
    AppController.cpp
    AppController.h
    MainWindow.cpp
    MainWindow.h
)

target_link_libraries(minibi_app
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        minibi_ui
        minibi_data
)

target_include_directories(minibi_app
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)
