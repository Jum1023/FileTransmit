#include "common.h"

#define BUFFER_SIZE 1024

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
} io_event;

typedef struct reactor
{
	int epfd;
	struct io_event *events;
} reactor;

//epoll operation
int io_event_add(int epfd, int events, io_event *ev)
{
	struct epoll_event ep_ev = {0};
	ep_ev.data.ptr = ev;
	ep_ev.events = ev->events = events;

	int op = ev->status == 1 ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;
	ev->status = 1;
	epoll_ctl(epfd, op, ev->fd, &ep_ev);
	return 0;
}

int io_event_del(int epfd, int events, io_event *ev)
{
	if (ev->status != 1) //not exist in epoll
		return 0;
	struct epoll_event ep_ev = {0};
	ep_ev.data.pre = ev;
	ev->status = 0;

	epoll_ctl(epfd, EPOLL_CTL_DEL, ev->fd, &ep_ev);
	return 0;
}

//set new events to fd
int io_event_set(io_event *ev, int fd, io_events_callback callback, void *arg)
{
	ev->fd = fd;
	ev->callback = callback;
	ev->arg = arg;
	return 0;
}