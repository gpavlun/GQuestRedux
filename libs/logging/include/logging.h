#ifndef LOGGING_H
#define LOGGING_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void gl_info(char *message);
void gl_data(char *message, int number);
void gl_error(int number, char *message);
char *indent(const char *str);
void gl_detail(const char *fmt, ...);
void gl_assert(bool condition, char *message);
void gl_warn(char *message);
void gl_set_path(char *path);

typedef struct logging{
    void (*info)(char *message);
    void (*data)(char *message, int number);
    void (*warn)(char *message);
    void (*error)(int number, char *message);
    void (*detail)(const char *, ...);
    void (*assert)(bool condition, char *message);
    void (*path)(char *path);
} logging_t;

extern logging_t logging;


#endif