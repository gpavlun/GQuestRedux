#include "logging.h"

logging_t logging = {
    .info = gl_info,
    .data = gl_data,
    .warn = gl_warn,
    .error = gl_error,
    .detail = gl_detail,
    .assert = gl_assert,
    .path = gl_set_path
};
FILE *glob_gl_file;

void gl_exit_des(void){
    if(glob_gl_file!=stdout){
        fclose(glob_gl_file);
    }
}


__attribute__((constructor))
void gl_start_con(void){
    glob_gl_file = stdout;
    atexit(gl_exit_des);
}


void gl_set_path(char *path){
    FILE *file = fopen(path,"w+");
    if(!file) return;
    if(glob_gl_file!=stdout){
        fclose(glob_gl_file);
    }
    glob_gl_file = file;
}

void gl_assert(bool condition, char *message){
    if(!condition){
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        fprintf(glob_gl_file,"[%02d:%02d:%02d] \x1b[31mASSERT!\x1b[0m\t> %s\n",
            t->tm_hour,
            t->tm_min,
            t->tm_sec,
            message);

        exit(1);
    }
}

void gl_info(char *message){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(glob_gl_file,"[%02d:%02d:%02d] \x1b[32mINFO:\x1b[0m\t> %s\n",
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        message);
}
void gl_data(char *message, int number){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(glob_gl_file,"[%02d:%02d:%02d] \x1b[34mDATA:\x1b[0m\t> %s %d\n",
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        message, 
        number);
}
void gl_error(int number, char *message){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(glob_gl_file,"[%02d:%02d:%02d] \x1b[31mERROR CODE %d:\x1b[0m\t> %s\n",
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        number, 
        message);
    exit(number);
}
void gl_warn(char *message){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(glob_gl_file,"[%02d:%02d:%02d] \x1b[38;5;208mWARN:\x1b[0m\t> %s\n",
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        message);
}

char *indent(const char *str){
    if(!str) return NULL;

    size_t len = strlen(str);
    size_t newlines = 0;

    for(const char *p = str; *p; ++p)
        if (*p == '\n') ++newlines;

    char *result = malloc(len + (newlines*5) + 1);

    const char *src = str;
    char *dst = result;

    while(*src){
        *dst++ = *src;
        if (*src == '\n'){
            *dst++ = '\t';
            *dst++ = '\t';
            *dst++ = '\t';
            *dst++ = '|';
            *dst++ = ' ';            
        }
        ++src;
    }
    *dst = '\0';
    return result;
}
void gl_detail(const char *fmt, ...){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char *message;

    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);

    int len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);
    
    if(len<0){
        message = malloc(strlen("error parsing")+1);
        strcpy(message,"error parsing");
    }else{
        message = malloc((size_t)len+1);
        vsnprintf(message, (size_t)len+1, fmt, args);
    }
    va_end(args);

    char *formatted = indent(message);
    free(message);

    fprintf(glob_gl_file,"[%02d:%02d:%02d] \x1b[33mDETAIL:\x1b[0m\t> %s\n",
        t->tm_hour,
        t->tm_min,
        t->tm_sec, 
        formatted);
    
    free(formatted);
}

