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

        sdl_windowing/sdl_ops.c
        sdl_windowing/sdl_ops.h

        tui_panel/terminal_interface.c
        tui_panel/terminal_interface.h

        main/typing.h
        main/main.c
        main/game.h
        rendering/lighting.c
        rendering/lighting.h
        main/player_controller.c
        main/player_controller.h
        generation/mesh_generation.c
        generation/shader_generation.c
        generation/terrain_generation.c
        generation/generation.h
        main/simulation.c
        main/simulation.h
        generation/texture_generation.c
        bmp_parser/bmp_parser.c
        bmp_parser/bmp_parser.h
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

target_compile_options(render PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -g
)

target_link_options(render PRIVATE
        -fsanitize=address
)

set_target_properties(render PROPERTIES
    OUTPUT_NAME "render.grg"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)