#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <gio.h>

#define event_rate 10000 //100x a sec

#define def typedef struct

#define true 1
#define false 0

typedef struct boolean_struct{
  uint8_t RUNNING: 1;
  uint8_t dynamic: 1;
}boolean_t;
extern boolean_t modes;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#endif
