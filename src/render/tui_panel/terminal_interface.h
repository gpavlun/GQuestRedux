//
// Created by berserker on 7/24/26.
//

#ifndef GQUEST_TERMINAL_INTERFACE_H
#define GQUEST_TERMINAL_INTERFACE_H

#include <gcurses.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../main/game.h"

void *tui(void *arg);
extern char glob_error[64];

#endif //GQUEST_TERMINAL_INTERFACE_H
