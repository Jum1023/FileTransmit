#ifndef _REACTOR_H_
#define _REACTOR_H_

#include "common.h"

#define BUFFER_SIZE 1024
#define EPOLL_SIZE 1024
#define MAX_FD_EVENTS (1 << 20) //1024 * 1024 = 1048576

typedef int io_events_callback(int, int, void *);

//io-->event
typedef struct io_event
{
	int fd;
	int events; //EPOLLIN | EPOLLET ...
	int (*callback)(int fd, int events, void *arg);
	void *arg;
	char buffer[BUFFER_SIZE];
	int bufferMsgSize; //received msg length
	int status;		   //1 exist in epoll 0 not exist
	long last_active;  //remove dummzy connection
} io_event;

typedef struct Reactor
{
	int epfd;
	int stop;	 //is running
	int setsize; //max number of file descriptors tracked
	io_event *events;
} Reactor;

/*****epoll operation*****/
//add event
extern int io_event_add(int epfd, int events, io_event *ev);
//del event
extern int io_event_del(int epfd, io_event *ev);
//set new events to fd
extern int io_event_set(io_event *ev, int fd, io_events_callback callback, void *arg);
/*****end of epoll operation*****/

//reactor
extern int reactor_init(Reactor *reactor);
extern int reactor_delete(Reactor *reactor);
extern void reactor_run(Reactor *reactor);

#endif