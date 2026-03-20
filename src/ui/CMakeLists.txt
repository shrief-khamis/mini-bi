add_library(minibi_ui
    widgets/Canvas.cpp
    widgets/Canvas.h
    widgets/Controlpanel.cpp
    widgets/Controlpanel.h
)

target_link_libraries(minibi_ui PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(minibi_ui
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)
