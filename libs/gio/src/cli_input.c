#include <gio.h>

int input_int(int *value){
    char buf[32];
    char *end;

    if(!fgets(buf, sizeof(buf), stdin)) return 0;

    if(!strchr(buf, '\n')){
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    long n = strtol(buf, &end, 10);
    if(end==buf || (*end!='\n' && *end!='\0')) return 0;

    if(n < INT_MIN || n > INT_MAX) return 0;

    *value = (int)n;
    return 1;
}

char get_input(void){

    char c;

    ssize_t n = read(STDIN_FILENO, &c, 1);

    if (n == 1)
        return c;

    return '\0';
}
