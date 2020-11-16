#ifndef _CREATE_SOCKET_H_
#define _CREATE_SOCKET_H_

#include "common.h"

extern struct sockaddr_in createServerInfo(const char *ip, int port);
extern int initServerSocket(const char *serverip, int serverport);

#endif