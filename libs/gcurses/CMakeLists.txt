add_library(gcurses STATIC)

file(GLOB GCURSES_SOURCES CONFIGURE_DEPENDS
    src/*.c
)

target_sources(gcurses
    PRIVATE
        ${GCURSES_SOURCES}
)

target_include_directories(gcurses
    PUBLIC
        include
)

target_link_libraries(gcurses
    PUBLIC
        GQuest::objects
)

option(BUILD_GCURSES_DEMO "Build gcurses demo" OFF)

if(BUILD_GCURSES_DEMO)
    add_executable(gcurses-demo demo.c)

    target_link_libraries(gcurses-demo
        PRIVATE
            GQuest::gcurses
    )
endif()

add_library(GQuest::gcurses ALIAS gcurses)