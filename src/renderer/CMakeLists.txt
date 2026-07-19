add_executable(render)

file(GLOB RENDER_SOURCES CONFIGURE_DEPENDS
    *.c
)

target_sources(render
    PRIVATE
        ${RENDER_SOURCES}
)

target_link_libraries(render
    PRIVATE
        PkgConfig::SDL2
        GQuest::logging
        GQuest::gio
        GQuest::gcurses
        m
)

set_target_properties(render PROPERTIES
    OUTPUT_NAME "render.grg"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)