#include "csv.h"
#include <stdio.h>
#include <string.h>

static FILE *f = NULL;

void open_csv(char *file_path, char *opt) {
  f = fopen(file_path, opt);
}

void write_csv(char *buf) {
  fwrite(buf, strlen(buf), 1 , f);
}

void close_csv(void) {
  fclose(f);
}

