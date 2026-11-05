#ifndef _ACTIONS_H_
#define _ACTIONS_H_

int handle_from(int cfd, char *buffer);
int handle_all(int cfd);
int handle_send(int cfd, char *buffer);

#endif
