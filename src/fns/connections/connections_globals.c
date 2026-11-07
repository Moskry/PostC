#include "connections.h"

struct sockaddr_in6 server_addr;
Subscribers *subscribers;
size_t count;
int server_fd;
