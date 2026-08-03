//
// Created by berserker on 7/24/26.
//




#include "terminal_interface.h"
#include "../physics/physics.h"

char glob_error[64];

void *tui(void *arg) {

  engine_t *engine = (engine_t *)arg;
  while (!engine->modes.running) usleep(10000);

  player_t *player = engine->physics.player;
  
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

    sprintf(source, "    [s] : (%.2f m, %.2fm, %.2fm)", player->actor.pos.x,
            player->actor.pos.y, player->actor.pos.z);
    gc_horz_str_disp(terminal, 2, 1, source);

    sprintf(source, "    [v] : (%.2f ms, %.2fms, %.2fms)", player->actor.vel.x,
            player->actor.vel.y, player->actor.vel.z);
    gc_horz_str_disp(terminal, 3, 1, source);

    sprintf(source, "    [a] : (%.2f ms^2, %.2fms^2, %.2fms^2)", player->actor.acc.x,
            player->actor.acc.y, player->actor.acc.z);
    gc_horz_str_disp(terminal, 4, 1, source);

    sprintf(source, "    desired velocity : (%.2f N, %.2fN, %.2fN)",
            player->actor.desired_velocity.x, player->actor.desired_velocity.y,
            player->actor.desired_velocity.z);
    gc_horz_str_disp(terminal, 6, 1, source);

    sprintf(source, "    applied force    : (%.2f N, %.2fN, %.2fN)",
            player->actor.applied_force.x, player->actor.applied_force.y,
            player->actor.applied_force.z);
    gc_horz_str_disp(terminal, 7, 1, source);

    sprintf(source, "    net force        : (%.2f N, %.2fN, %.2fN)",
            player->actor.net_force.x, player->actor.net_force.y,
            player->actor.net_force.z);
    gc_horz_str_disp(terminal, 8, 1, source);

    sprintf(source, "    flying : %s", player->flying?"true":"false");
    gc_horz_str_disp(terminal, 9, 1, source);



    sprintf(source, "camera");
    gc_horz_str_disp(terminal, 11, 1, source);

    sprintf(source, "    psi %.2f :: theta %.2f :: phi %.2f", player->camera.psi, player->camera.theta, player->camera.phi);
    gc_horz_str_disp(terminal, 12, 1, source);

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