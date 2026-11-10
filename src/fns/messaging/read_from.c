#include "messaging.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int read_from(int cfd, char *buffer, size_t n) {
  size_t num_red = 0;

  while (num_red < n) {
    ssize_t r = read(cfd, buffer + num_red, n - num_red);

    if (r > 0)
      num_red += (size_t)r;
    else if (r == 0)
      return 2; // EOF
    else if (errno != EINTR)
      return EXIT_FAILURE; // read error
  }

  return EXIT_SUCCESS;
}
