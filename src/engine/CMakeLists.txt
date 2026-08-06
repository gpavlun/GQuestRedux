add_executable(engine
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

file(GLOB ENGINE_SOURCES CONFIGURE_DEPENDS
    *.c
)

target_sources(engine
    PRIVATE
        ${ENGINE_SOURCES}
)

target_link_libraries(engine
    PRIVATE
        PkgConfig::SDL2
        GQuest::logging
        GQuest::gio
        GQuest::gcurses
        GQuest::glad
        m
)

target_compile_options(engine PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
)

target_link_options(engine PRIVATE
        -fsanitize=address,undefined
)

set_target_properties(engine PROPERTIES
    OUTPUT_NAME "engine.grg"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

set_target_properties(engine PROPERTIES
    OUTPUT_NAME "GQuestRedux"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/release"
)