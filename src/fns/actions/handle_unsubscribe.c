#include "actions.h"
#include "connections.h"
#include <stdio.h>
#include <stdlib.h>

int handle_unsubscribe(int cfd) {
  for (size_t i = 0; i < count; i++)
    if (subscribers[i].fd == cfd)
      return unsubscribe(subscribers[i].defname);
  perror("subscriber not found");
  return EXIT_FAILURE;
}
