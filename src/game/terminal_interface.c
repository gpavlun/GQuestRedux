//
// Created by berserker on 7/24/26.
//


#include <gcurses.h>
#include <pthread.h>

#include "terminal_interface.h"
#include "physics.h"

char glob_error[64];

void *tui(void *arg) {

  term_w_t terminal;
  init_tui(&terminal);
  terminal.io_block(0);
  terminal.clear();
  terminal.present();

  strcpy(glob_error, "no error");
  char source[128];
  str_t str;


  while (modes.RUNNING) {

    terminal.clear();

    str.c = 1;
    str.source = source;

    str.r = 1;
    sprintf(source, "player");
    terminal.horz_strdisp(str);

    str.r = 2;
    sprintf(source, "    [s] : (%.2f m, %.2fm, %.2fm)", player.pos.x,
            player.pos.y, player.pos.z);
    terminal.horz_strdisp(str);

    str.r = 3;
    sprintf(source, "    [v] : (%.2f ms, %.2fms, %.2fms)", player.vel.x,
            player.vel.y, player.vel.z);
    terminal.horz_strdisp(str);

    str.r = 4;
    sprintf(source, "    [a] : (%.2f ms^2, %.2fms^2, %.2fms^2)", player.acc.x,
            player.acc.y, player.acc.z);
    terminal.horz_strdisp(str);

    str.r = 6;
    sprintf(source, "    desired velocity : (%.2f N, %.2fN, %.2fN)",
            player.desired_velocity.x, player.desired_velocity.y,
            player.desired_velocity.z);
    terminal.horz_strdisp(str);

    str.r = 7;
    sprintf(source, "    applied force    : (%.2f N, %.2fN, %.2fN)",
            player.applied_force.x, player.applied_force.y,
            player.applied_force.z);
    terminal.horz_strdisp(str);

    str.r = 8;
    sprintf(source, "    net force        : (%.2f N, %.2fN, %.2fN)",
            player.net_force.x, player.net_force.y,
            player.net_force.z);
    terminal.horz_strdisp(str);
    str.r = 9;
    sprintf(source, "    flying : %s",
            flying?"true":"false");
    terminal.horz_strdisp(str);



    str.r = 11;
    sprintf(source, "camera");
    terminal.horz_strdisp(str);

    str.r = 12;
    sprintf(source, "    actor.theta %.2f :: phi %.2f", player.theta, player.phi);
    terminal.horz_strdisp(str);

    str.r = 14;
    sprintf(source, "error message: %s", glob_error);
    terminal.horz_strdisp(str);

    terminal.present();

    usleep(10000);
  }

  terminal.io_block(1);
  terminal.cursor(1);
  move_cursor(&terminal, terminal.nrows, 0);
  pthread_exit(0);
  return 0;
}