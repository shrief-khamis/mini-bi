add_library(minibi_ui
    widgets/Canvas.cpp
    widgets/Canvas.h
    widgets/ControlPanel.cpp
    widgets/ControlPanel.h
)

target_link_libraries(minibi_ui
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        minibi_logging
        minibi_plot
)

target_include_directories(minibi_ui
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)
