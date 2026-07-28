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

void info(char *message);
void data(char *message, int number);
void error(int number, char *message);
char *indent(const char *str);
void detail(const char *fmt, ...);
void assert(bool condition, char *message);
void warn(char *message);

typedef struct logging{
    void (*info)(char *message);
    void (*data)(char *message, int number);
    void (*warn)(char *message);
    void (*error)(int number, char *message);
    void (*detail)(const char *, ...);
    void (*assert)(bool condition, char *message);
} logging_t;

extern logging_t logging;


#endif