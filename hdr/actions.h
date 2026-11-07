#ifndef _ACTIONS_H_
#define _ACTIONS_H_

/* GET */

int handle_from(int cfd, char *buffer);
int handle_all(int cfd);

/* POST */

int handle_send(int cfd, char *buffer);
int handle_unsubscribe(int cfd);
#endif
