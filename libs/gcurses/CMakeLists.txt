find_program(CARGO cargo REQUIRED)

add_custom_target(gcurses_build ALL
        COMMAND ${CARGO} build --release
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(gcurses STATIC IMPORTED GLOBAL)

set_target_properties(gcurses PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/target/release/libgcurses.a

        INTERFACE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_property(TARGET gcurses PROPERTY
        INTERFACE_LINK_LIBRARIES
        pthread
        dl
)

add_dependencies(gcurses gcurses_build)

add_library(GQuest::gcurses ALIAS gcurses)
