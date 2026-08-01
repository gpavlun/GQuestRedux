#include "filehelper.h"

char *read_file(char *path){
  size_t nread;

  FILE *file = fopen(path, "rb");
  logging.assert(file, "file not found!");

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  if(size< 0){
      fclose(file);
      return NULL;
  }

  char *source = malloc(size + 1);
  nread = fread(source, 1, size, file);
  source[nread] = 0;

  fclose(file);

  return source;
}