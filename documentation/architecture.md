#Project layout:
../.  
├── CMakeLists.txt  
├── documentation  
│   ├── arch_gen.sh  
│   ├── architecture.md  
│   ├── architecture.txt  
│   └── README.md  
├── first-time-startup.sh  
├── launcher.conf  
├── launch.sh  
├── libs  
│   ├── CMakeLists.txt  
│   ├── gcurses  
│   │   ├── CMakeLists.txt  
│   │   ├── demo  
│   │   ├── demo.c  
│   │   ├── include  
│   │   │   └── gcurses.h  
│   │   ├── makefile  
│   │   └── src  
│   │       ├── gcurses.c  
│   │       └── objective.c  
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
├── makefile  
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
│   │   ├── rendering_ops.c  
│   │   └── sdl2init.c  
│   └── game  
│       ├── CMakeLists.txt  
│       ├── game.c  
│       └── game.h  
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
  
21 directories, 60 files  
  
#Lines of code by file:  
  175 ../libs/gcurses/demo.c  
  257 ../libs/gcurses/include/gcurses.h  
  562 ../libs/gcurses/src/gcurses.c  
    0 ../libs/gcurses/src/objective.c  
   84 ../libs/gio/include/gio.h  
   35 ../libs/gio/src/cli_input.c  
   26 ../libs/logging/include/logging.h  
  111 ../libs/logging/src/logging.c  
   62 ../libs/objects/include/objects.h  
  117 ../libs/objects/src/oop.c  
  341 ../src/devstudio/bootmenu.c  
   57 ../src/devstudio/confparser.c  
   18 ../src/devstudio/dev.h  
   37 ../src/devstudio/launcher-wrapper.c  
  162 ../src/editor/chunk_operations.c  
  282 ../src/editor/editor.c  
   98 ../src/editor/editor.h  
   89 ../src/editor/editor_menu.c  
   90 ../src/editor/editor_window.c  
   61 ../src/editor/rendering_ops.c  
   17 ../src/editor/sdl2init.c  
  475 ../src/game/game.c  
   11 ../src/game/game.h  
 3167 total  
