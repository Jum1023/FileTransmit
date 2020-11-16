#ifndef _CALLBACK_H_
#define _CALLBACK_H_

//custom callbacks
int send_callback(int fd, int events, void *arg);
int recv_callback(int fd, int events, void *arg);
int accept_callback(int fd, int events, void *arg);

#endif