add_executable(game
        terminal_interface.c
        terminal_interface.h)

file(GLOB GAME_SOURCES CONFIGURE_DEPENDS
    *.c
)

target_sources(game
    PRIVATE
        ${GAME_SOURCES}
)

target_link_libraries(game
    PRIVATE
        PkgConfig::SDL2
        GQuest::logging
        GQuest::gio
        GQuest::gcurses
        m
)

set_target_properties(game PROPERTIES
    OUTPUT_NAME "game.grg"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)