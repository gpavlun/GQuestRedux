#ifndef GAME_H
#define GAME_H

#define event_rate 10000 //100x a sec

#include "typing.h"

typedef struct boolean_struct{
  u8 RUNNING: 1;
  u8 DYNAMIC: 1;
  u8 WIREFRAME:1;
}boolean_t;
extern boolean_t modes;

#endif
