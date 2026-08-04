//
// Created by berserker on 7/24/26.
//




#include "terminal_interface.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <logging.h>

#include "gcurses.h"
#include "../main/game.h"


char glob_error[64];

void *tui(void *arg) {

  engine_t *engine = (engine_t *)arg;
  logging.info("console_thread: console waiting");
  logging.detail("console_thread: barrier at %p",&engine->barrier);
  pthread_barrier_wait(&engine->barrier);
  logging.info("console_thread: console started");

  player_t *player = &engine->simulation.player_table[0];
  
  gc_term_t *terminal;
  terminal = gc_new();
  gc_io_block(terminal, 0);
  gc_show_cursor(terminal, 0);
  gc_echo(terminal, 0);
  gc_canon(terminal, 0);
  
  gc_clear(terminal);
  gc_present(terminal);


  strcpy(glob_error, "no error");
  char source[128];


  while (engine->modes.running) {

    gc_frame_resize(terminal);

    gc_clear(terminal);


    sprintf(source, "player");
    gc_horz_str_disp(terminal, 1, 1, source);

    sprintf(source, "    [s] : (%.2f m, %.2fm, %.2fm)", player->entity.transform.pos.x,
            player->entity.transform.pos.y, player->entity.transform.pos.z);
    gc_horz_str_disp(terminal, 2, 1, source);

    sprintf(source, "    [v] : (%.2f ms, %.2fms, %.2fms)", player->entity.physics_actor.vel.x,
            player->entity.physics_actor.vel.y, player->entity.physics_actor.vel.z);
    gc_horz_str_disp(terminal, 3, 1, source);

    sprintf(source, "    [a] : (%.2f ms^2, %.2fms^2, %.2fms^2)", player->entity.physics_actor.acc.x,
            player->entity.physics_actor.acc.y, player->entity.physics_actor.acc.z);
    gc_horz_str_disp(terminal, 4, 1, source);

    sprintf(source, "    desired velocity : (%.2f N, %.2fN, %.2fN)",
            player->entity.physics_actor.desired_velocity.x, player->entity.physics_actor.desired_velocity.y,
            player->entity.physics_actor.desired_velocity.z);
    gc_horz_str_disp(terminal, 6, 1, source);

    sprintf(source, "    applied force    : (%.2f N, %.2fN, %.2fN)",
            player->entity.physics_actor.applied_force.x, player->entity.physics_actor.applied_force.y,
            player->entity.physics_actor.applied_force.z);
    gc_horz_str_disp(terminal, 7, 1, source);

    sprintf(source, "    net force        : (%.2f N, %.2fN, %.2fN)",
            player->entity.physics_actor.net_force.x, player->entity.physics_actor.net_force.y,
            player->entity.physics_actor.net_force.z);
    gc_horz_str_disp(terminal, 8, 1, source);

    sprintf(source, "    rotation (%.2f, %.2f, %.2f)", player->entity.transform.rot.x, player->entity.transform.rot.y, player->entity.transform.rot.z);
    gc_horz_str_disp(terminal, 9, 1, source);

    sprintf(source, "    flying : %s", player->flying?"true":"false");
    gc_horz_str_disp(terminal, 10, 1, source);



    sprintf(source, "camera");
    gc_horz_str_disp(terminal, 11, 1, source);

    sprintf(source, "    rotation (%.2f, %.2f, %.2f)", player->camera.rot.x, player->camera.rot.y, player->camera.rot.z);
    gc_horz_str_disp(terminal, 12, 1, source);

    sprintf(source, "    position (%.2f, %.2f, %.2f)", player->camera.pos.x, player->camera.pos.y, player->camera.pos.z);
    gc_horz_str_disp(terminal, 13, 1, source);

    sprintf(source, "error message: %s", glob_error);
    gc_horz_str_disp(terminal, 14, 1, source);

    gc_present(terminal);

    usleep(10000);
  }

  gc_io_block(terminal, 1);
  gc_show_cursor(terminal, 1);
  gc_echo(terminal, 1);
  gc_canon(terminal, 1);
  gc_move_cursor(terminal, gc_nrows(terminal), 0);

  gc_drop(terminal);
  pthread_exit(0);
  return 0;
}