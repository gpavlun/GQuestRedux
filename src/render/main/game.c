#include <pthread.h>

#include "game.h"
#include "../rendering/render.h"
#include "../tui_panel/terminal_interface.h"

boolean_t modes;

int main(int argc, char **argv){


    modes.RUNNING = 1;

    pthread_t tui_thread;
    pthread_create(&tui_thread, NULL, tui, NULL);

    start_render();

    modes.RUNNING = 0;
    pthread_join(tui_thread, NULL);

  return 0;

}
