#include "todo.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  printf("%x", 1024);
  if (add(5, 10) == 15)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}
