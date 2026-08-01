add_executable(render
        math/vector.c
        math/vector.h
        math/matrix.c
        math/matrix.h

        physics/physics.c
        physics/physics.h

        rendering/obj_parser.c
        rendering/obj_parser.h
        rendering/filehelper.c
        rendering/filehelper.h
        rendering/render.c
        rendering/render.h

        sdl_windowing/editor_window.c
        sdl_windowing/editor.h

        tui_panel/terminal_interface.c
        tui_panel/terminal_interface.h

        main/typing.h
        main/game.c
        main/game.h
        rendering/lighting.c
        rendering/lighting.h
)

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
        GQuest::glad
        m
)

set_target_properties(render PROPERTIES
    OUTPUT_NAME "render.grg"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)