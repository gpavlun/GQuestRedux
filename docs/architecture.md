## Project layout:
../.  
├── assets  
│   ├── barad_dur  
│   │   └── obj.mtl  
│   ├── barad_dur.obj  
│   ├── castle  
│   │   └── obj.mtl  
│   ├── castle.obj  
│   ├── eye  
│   │   └── obj.mtl  
│   ├── eye.obj  
│   ├── player  
│   │   └── obj.mtl  
│   ├── player.obj  
│   ├── roofs  
│   │   └── obj.mtl  
│   └── roofs.obj  
├── CMakeLists.txt  
├── devtmux.conf  
├── docs  
│   ├── ai_summary.txt  
│   ├── arch_gen.sh  
│   ├── architecture.md  
│   ├── README.md  
│   └── rending formulas.md  
├── first-time-startup.sh  
├── .idea  
│   ├── codeStyles  
│   │   ├── codeStyleConfig.xml  
│   │   └── Project.xml  
│   ├── editor.xml  
│   ├── GQuestRedux.iml  
│   ├── inspectionProfiles  
│   │   └── Project_Default.xml  
│   ├── misc.xml  
│   ├── modules.xml  
│   ├── .name  
│   ├── vcs.xml  
│   └── workspace.xml  
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
│   └── render  
│       ├── basic.frag  
│       ├── basic.vert  
│       ├── CMakeLists.txt  
│       ├── main  
│       │   ├── game.c  
│       │   ├── game.h  
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
│       │   ├── editor.h  
│       │   └── editor_window.c  
│       └── tui_panel  
│           ├── terminal_interface.c  
│           └── terminal_interface.h  
├── tmp  
├── tmux-client-2994278.log  
├── tmux-server-2994280.log  
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
  
43 directories, 123 files  
  
## Lines of code by file:  
   287 ../src/editor/editor.c  
    17 ../src/editor/sdl2init.c  
    89 ../src/editor/editor_menu.c  
   162 ../src/editor/chunk_operations.c  
   104 ../src/editor/legacy_functions.c  
    64 ../src/editor/rendering_ops.c  
   122 ../src/editor/editor.h  
    95 ../src/editor/editor_window.c  
    63 ../src/game/physics.h  
   457 ../src/game/render.c  
   350 ../src/game/physics.c  
    92 ../src/game/terminal_interface.c  
    13 ../src/game/terminal_interface.h  
   493 ../src/game/game.c  
   558 ../src/game/render.h  
    30 ../src/game/game.h  
   122 ../src/game/editor.h  
    92 ../src/game/editor_window.c  
   222 ../src/devstudio/bootmenu.c  
     5 ../src/devstudio/dev.h  
    73 ../src/render/physics/physics.h  
   440 ../src/render/physics/physics.c  
    27 ../src/render/main/typing.h  
    24 ../src/render/main/game.c  
    15 ../src/render/main/game.h  
    65 ../src/render/rendering/obj_parser.c  
    77 ../src/render/rendering/lighting.c  
   784 ../src/render/rendering/render.c  
    34 ../src/render/rendering/lighting.h  
   478 ../src/render/rendering/render.h  
    24 ../src/render/rendering/filehelper.c  
    15 ../src/render/rendering/obj_parser.h  
     9 ../src/render/rendering/filehelper.h  
   122 ../src/render/sdl_windowing/editor.h  
   100 ../src/render/sdl_windowing/editor_window.c  
    38 ../src/render/math/matrix.h  
    76 ../src/render/math/vector.h  
   152 ../src/render/math/matrix.c  
     2 ../src/render/math/vector.c  
    99 ../src/render/tui_panel/terminal_interface.c  
    19 ../src/render/tui_panel/terminal_interface.h  
    33 ../libs/gio/src/cli_input.c  
    84 ../libs/gio/include/gio.h  
  1840 ../libs/glad/src/glad.c  
   311 ../libs/glad/include/KHR/khrplatform.h  
  3611 ../libs/glad/include/glad/glad.h  
   116 ../libs/objects/src/oop.c  
    62 ../libs/objects/include/objects.h  
   128 ../libs/logging/src/logging.c  
    32 ../libs/logging/include/logging.h  
   362 ../libs/gcurses/demo.c  
     2 ../libs/gcurses/src/lib.rs  
   589 ../libs/gcurses/src/gcurses.rs  
   593 ../libs/gcurses/src/Cimpl_gcurses.c  
   362 ../libs/gcurses/src/c_exposure.rs  
   121 ../libs/gcurses/include/gcurses.h  
   259 ../libs/gcurses/include/Cimpl_gcurses.h  
 14615 total  
