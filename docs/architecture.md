## Project layout:
../.  
├── assets  
│   ├── bmp_textures  
│   │   ├── bark1.bmp  
│   │   ├── bricks.bmp  
│   │   ├── grass2.bmp  
│   │   ├── grass.bmp  
│   │   ├── peat1.bmp  
│   │   ├── planks1.bmp  
│   │   ├── rock1.bmp  
│   │   ├── rockycoast.bmp  
│   │   ├── shingles1.bmp  
│   │   └── stone1.bmp  
│   ├── compressed.zip  
│   └── obj_models  
│       ├── barad_dur.obj  
│       ├── castle.obj  
│       ├── eye.obj  
│       ├── placeholder (Copy).obj  
│       ├── player.obj  
│       ├── roofs.obj  
│       ├── tower_body.obj  
│       └── tower_roof.obj  
├── CMakeLists.txt  
├── devtmux.conf  
├── docs  
│   ├── ai_summary.txt  
│   ├── arch_gen.sh  
│   ├── architecture.md  
│   ├── README.md  
│   └── rending formulas.md  
├── first-time-startup.sh  
├── launch.sh  
├── libs  
│   ├── CMakeLists.txt  
│   ├── gcurses  
│   │   ├── Cargo.lock  
│   │   ├── Cargo.toml  
│   │   ├── CMakeLists.txt  
│   │   ├── C-version_CMakeLists.txt  
│   │   ├── demo.c  
│   │   ├── include  
│   │   │   ├── Cimpl_gcurses.h  
│   │   │   └── gcurses.h  
│   │   ├── makefile  
│   │   ├── rust-build.sh  
│   │   └── src  
│   │       ├── c_exposure.rs  
│   │       ├── Cimpl_gcurses.c  
│   │       ├── gcurses.rs  
│   │       └── lib.rs  
│   ├── gio  
│   │   ├── CMakeLists.txt  
│   │   ├── include  
│   │   │   └── gio.h  
│   │   └── src  
│   │       └── cli_input.c  
│   ├── glad  
│   │   ├── CMakeLists.txt  
│   │   ├── include  
│   │   │   ├── glad  
│   │   │   │   └── glad.h  
│   │   │   └── KHR  
│   │   │       └── khrplatform.h  
│   │   └── src  
│   │       └── glad.c  
│   ├── logging  
│   │   ├── CMakeLists.txt  
│   │   ├── include  
│   │   │   └── logging.h  
│   │   ├── makefile  
│   │   └── src  
│   │       └── logging.c  
│   └── objects  
│       ├── CMakeLists.txt  
│       ├── include  
│       │   └── objects.h  
│       ├── makefile  
│       ├── README.md  
│       └── src  
│           └── oop.c  
├── scripts  
│   ├── build-boot.sh  
│   ├── build-editor.sh  
│   ├── build-game.sh  
│   ├── run-editor.sh  
│   └── run-game.sh  
├── src  
│   ├── CMakeLists.txt  
│   ├── devstudio  
│   │   ├── bootmenu.c  
│   │   ├── CMakeLists.txt  
│   │   └── dev.h  
│   ├── editor  
│   │   ├── chunk_operations.c  
│   │   ├── CMakeLists.txt  
│   │   ├── editor.c  
│   │   ├── editor.h  
│   │   ├── editor_menu.c  
│   │   ├── editor_window.c  
│   │   ├── legacy_functions.c  
│   │   ├── rendering_ops.c  
│   │   └── sdl2init.c  
│   ├── game  
│   │   ├── CMakeLists.txt  
│   │   ├── editor.h  
│   │   ├── editor_window.c  
│   │   ├── game.c  
│   │   ├── game.h  
│   │   ├── physics.c  
│   │   ├── physics.h  
│   │   ├── render.c  
│   │   ├── render.h  
│   │   ├── terminal_interface.c  
│   │   └── terminal_interface.h  
│   ├── legacy  
│   │   ├── game.c  
│   │   ├── game.h  
│   │   ├── render.c  
│   │   └── render.h  
│   └── render  
│       ├── basic.frag  
│       ├── basic.vert  
│       ├── bmp_parser  
│       │   ├── bmp_parser.c  
│       │   ├── bmp_parser.h  
│       │   └── parser  
│       ├── CMakeLists.txt  
│       ├── generation  
│       │   ├── generation.h  
│       │   ├── mesh_generation.c  
│       │   ├── shader_generation.c  
│       │   ├── terrain_generation.c  
│       │   └── texture_generation.c  
│       ├── main  
│       │   ├── game.c  
│       │   ├── game.h  
│       │   ├── player_controller.c  
│       │   ├── player_controller.h  
│       │   ├── simulation.c  
│       │   ├── simulation.h  
│       │   └── typing.h  
│       ├── math  
│       │   ├── matrix.c  
│       │   ├── matrix.h  
│       │   ├── vector.c  
│       │   └── vector.h  
│       ├── physics  
│       │   ├── physics.c  
│       │   └── physics.h  
│       ├── rendering  
│       │   ├── filehelper.c  
│       │   ├── filehelper.h  
│       │   ├── lighting.c  
│       │   ├── lighting.h  
│       │   ├── obj_parser.c  
│       │   ├── obj_parser.h  
│       │   ├── render.c  
│       │   └── render.h  
│       ├── sdl_windowing  
│       │   ├── sdl_ops.c  
│       │   └── sdl_ops.h  
│       └── tui_panel  
│           ├── terminal_interface.c  
│           └── terminal_interface.h  
└── world_data  
    ├── _0_0_.ck  
    ├── _0_-1_.ck  
    ├── _0_1_.ck  
    ├── _-1_0_.ck  
    ├── _1_0_.ck  
    ├── _-1_-1_.ck  
    ├── _-1_1_.ck  
    ├── _1_-1_.ck  
    └── _1_1_.ck  
  
39 directories, 136 files  
  
## Lines of code by file:  
    32 ../libs/logging/include/logging.h  
   128 ../libs/logging/src/logging.c  
    84 ../libs/gio/include/gio.h  
    33 ../libs/gio/src/cli_input.c  
  3611 ../libs/glad/include/glad/glad.h  
   311 ../libs/glad/include/KHR/khrplatform.h  
  1840 ../libs/glad/src/glad.c  
    62 ../libs/objects/include/objects.h  
   116 ../libs/objects/src/oop.c  
   121 ../libs/gcurses/include/gcurses.h  
   259 ../libs/gcurses/include/Cimpl_gcurses.h  
   362 ../libs/gcurses/demo.c  
   589 ../libs/gcurses/src/gcurses.rs  
   362 ../libs/gcurses/src/c_exposure.rs  
     2 ../libs/gcurses/src/lib.rs  
   593 ../libs/gcurses/src/Cimpl_gcurses.c  
   222 ../src/devstudio/bootmenu.c  
     5 ../src/devstudio/dev.h  
   457 ../src/legacy/render.c  
   558 ../src/legacy/render.h  
   493 ../src/legacy/game.c  
    30 ../src/legacy/game.h  
   457 ../src/game/render.c  
   122 ../src/game/editor.h  
   558 ../src/game/render.h  
    92 ../src/game/editor_window.c  
   493 ../src/game/game.c  
    63 ../src/game/physics.h  
    30 ../src/game/game.h  
    92 ../src/game/terminal_interface.c  
   350 ../src/game/physics.c  
    13 ../src/game/terminal_interface.h  
    36 ../src/render/rendering/lighting.h  
   599 ../src/render/rendering/render.c  
   124 ../src/render/rendering/render.h  
     9 ../src/render/rendering/filehelper.h  
    79 ../src/render/rendering/lighting.c  
    66 ../src/render/rendering/obj_parser.c  
    24 ../src/render/rendering/filehelper.c  
    15 ../src/render/rendering/obj_parser.h  
    33 ../src/render/generation/generation.h  
    86 ../src/render/generation/terrain_generation.c  
   137 ../src/render/generation/mesh_generation.c  
    29 ../src/render/generation/shader_generation.c  
    35 ../src/render/generation/texture_generation.c  
    95 ../src/render/bmp_parser/bmp_parser.c  
    43 ../src/render/bmp_parser/bmp_parser.h  
    63 ../src/render/sdl_windowing/sdl_ops.c  
    61 ../src/render/sdl_windowing/sdl_ops.h  
    18 ../src/render/main/player_controller.h  
   172 ../src/render/main/player_controller.c  
    31 ../src/render/main/typing.h  
    14 ../src/render/main/simulation.h  
   358 ../src/render/main/game.c  
   185 ../src/render/main/game.h  
    19 ../src/render/main/simulation.c  
    74 ../src/render/math/vector.h  
     2 ../src/render/math/vector.c  
    38 ../src/render/math/matrix.h  
   152 ../src/render/math/matrix.c  
   106 ../src/render/tui_panel/terminal_interface.c  
    11 ../src/render/tui_panel/terminal_interface.h  
    19 ../src/render/physics/physics.h  
   292 ../src/render/physics/physics.c  
   162 ../src/editor/chunk_operations.c  
    89 ../src/editor/editor_menu.c  
   122 ../src/editor/editor.h  
   287 ../src/editor/editor.c  
    17 ../src/editor/sdl2init.c  
    95 ../src/editor/editor_window.c  
    64 ../src/editor/rendering_ops.c  
   104 ../src/editor/legacy_functions.c  
 16505 total  
