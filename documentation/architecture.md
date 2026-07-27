## Project layout:
../.  
├── CMakeLists.txt  
├── devtmux.conf  
├── documentation  
│   ├── ai_summary.txt  
│   ├── arch_gen.sh  
│   ├── architecture.md  
│   ├── README.md  
│   └── rending formulas.md  
├── first-time-startup.sh  
├── launcher.conf  
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
│   │   ├── confparser.c  
│   │   ├── dev.h  
│   │   └── launcher-wrapper.c  
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
│   └── game  
│       ├── CMakeLists.txt  
│       ├── editor.h  
│       ├── editor_window.c  
│       ├── game.c  
│       ├── game.h  
│       ├── physics.c  
│       ├── physics.h  
│       ├── render.c  
│       ├── render.h  
│       ├── terminal_interface.c  
│       └── terminal_interface.h  
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
  
21 directories, 76 files  
  
## Lines of code by file:  
    26 ../libs/logging/include/logging.h
   111 ../libs/logging/src/logging.c
    84 ../libs/gio/include/gio.h
    44 ../libs/gio/src/cli_input.c
    62 ../libs/objects/include/objects.h
   116 ../libs/objects/src/oop.c
   119 ../libs/gcurses/include/gcurses.h
   259 ../libs/gcurses/include/Cimpl_gcurses.h
   362 ../libs/gcurses/demo.c
   542 ../libs/gcurses/src/gcurses.rs
   346 ../libs/gcurses/src/c_exposure.rs
     2 ../libs/gcurses/src/lib.rs
   593 ../libs/gcurses/src/Cimpl_gcurses.c
    57 ../src/devstudio/confparser.c
    37 ../src/devstudio/launcher-wrapper.c
   216 ../src/devstudio/bootmenu.c
    18 ../src/devstudio/dev.h
   460 ../src/game/render.c
   122 ../src/game/editor.h
   558 ../src/game/render.h
    90 ../src/game/editor_window.c
   493 ../src/game/game.c
    63 ../src/game/physics.h
    30 ../src/game/game.h
    87 ../src/game/terminal_interface.c
   350 ../src/game/physics.c
    13 ../src/game/terminal_interface.h
   162 ../src/editor/chunk_operations.c
    89 ../src/editor/editor_menu.c
   122 ../src/editor/editor.h
   287 ../src/editor/editor.c
    17 ../src/editor/sdl2init.c
    95 ../src/editor/editor_window.c
    64 ../src/editor/rendering_ops.c
   104 ../src/editor/legacy_functions.c
  6200 total
