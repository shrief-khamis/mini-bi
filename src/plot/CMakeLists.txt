add_library(minibi_plot
    core/PlotData.h
    core/PlotRenderer.h
    PlaceholderPlot.cpp
    PlaceholderPlot.h
    line/LinePlot.cpp
    line/LinePlot.h
    scatter/ScatterPlot.cpp
    scatter/ScatterPlot.h
)

target_link_libraries(minibi_plot
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        minibi_data
)

target_include_directories(minibi_plot
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)
