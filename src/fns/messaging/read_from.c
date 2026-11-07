#include "messaging.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int read_from(int cfd, char *buffer, size_t n) {
  int num_read = 0;
  for (;;) {
    num_read = read(cfd, buffer, n);
    if (num_read == -1) {
      if (errno == EINTR)
        continue;
      else
        return EXIT_FAILURE;
    } else
      break;
  }
  if (num_read < n)
    return 2; // Incorrect request
  return EXIT_SUCCESS;
}
