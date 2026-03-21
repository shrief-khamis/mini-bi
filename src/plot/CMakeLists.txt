add_library(minibi_plot
    PlotRenderer.cpp
    PlotRenderer.h
)

target_link_libraries(minibi_plot PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(minibi_plot
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)
