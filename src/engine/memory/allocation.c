
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

size_t glob_alloccount;
char glob_lf_path[64];

int logfile_initializer(void){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char path[] = "./logs/alloc_";
    sprintf(glob_lf_path,"%s%02d-%02d_%02dh%02dm%02ds.log",
       path,
       t->tm_mon + 1,
       t->tm_mday,
       t->tm_hour,
       t->tm_min,
       t->tm_sec);
    FILE *logfile = fopen(glob_lf_path, "a");
    if(!logfile) return 1;
    fprintf(logfile,"┌─[ ALLOCATION LOGFILE ]────────────\n");
    fprintf(logfile,"│\n│ created:\n│\t%04d-%02d-%02d\n│\t%02d:%02d:%02d\n│\n",
       t->tm_year + 1900,
       t->tm_mon + 1,
       t->tm_mday,
       t->tm_hour,
       t->tm_min,
       t->tm_sec);
    fclose(logfile);
    return 0;
}



void *accountable_allocate(
    char *Parent_File,
    char *Function_Name,
    char *Descrition,
    size_t nbytes,
    bool cleared
){
    if(!glob_lf_path[0]){
        if(logfile_initializer()) 
            goto allocate;
    }
    FILE *logfile = fopen(glob_lf_path, "a");
    if(!logfile) goto allocate;
    fprintf(logfile, "┌─[ ALLOC %lu ]───────────────────────\n",glob_alloccount);
    fprintf(logfile, "│\n");
    fprintf(logfile, "│\tfile: %s\n",Parent_File);
    fprintf(logfile, "│\tfunc: %s\n", Function_Name);
    fprintf(logfile, "│\tdesc: %s\n", Descrition);
    fprintf(logfile, "│\tsize: %lu\n", nbytes);
    fprintf(logfile, "│\n");
    fclose(logfile);
    glob_alloccount++;
allocate:
    if(cleared) return calloc(1, nbytes);
    else        return malloc(nbytes);
}

void main(void){

    int count = rand()%50;
    for(int i=0; i<count; i++){
        accountable_allocate(
            "allocation.c",
            "main",
            "loop test",
            rand()%512, rand()%2);
    }


}