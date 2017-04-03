#ifndef SOCKETCLIENT_H
# define SOCKETCLIENT_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <sys/socket.h>
# include <arpa/inet.h>

int     initsocket(char *av);
void    authentification(int sock);

#endif
