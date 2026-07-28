add_library(glad STATIC)

file(GLOB GLAD_SOURCES CONFIGURE_DEPENDS
    src/*.c
)

target_sources(glad
    PRIVATE
        ${GLAD_SOURCES}
)

target_include_directories(glad
    PUBLIC
        include
)

add_library(GQuest::glad ALIAS glad)