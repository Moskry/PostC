#include "messaging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  int p[2];
  char buffer[50] = {0};

  pipe(p);
  write(p[1], "hello!", 6);
  read_from(p[0], buffer, 6);
  printf("%s\n", buffer);

  memset(buffer, 0, sizeof(buffer));
  write(p[1], "BIMBOYE", 7);
  read_from(p[0], buffer, 5);
  printf("%s\n", buffer);

  close(p[1]);
  close(p[0]);
  return EXIT_SUCCESS;
}
