#include "reactor.h"

//epoll operation
int io_event_add(int epfd, int events, io_event *ev)
{
	struct epoll_event ep_ev = {0};
	ep_ev.data.ptr = ev;
	ep_ev.events = ev->events = events;

	int op = (ev->status == 0 ? EPOLL_CTL_ADD : EPOLL_CTL_MOD);
	ev->status = 1;
	epoll_ctl(epfd, op, ev->fd, &ep_ev);
	return 0;
}

int io_event_del(int epfd, io_event *ev)
{
	if (ev->status != 1) //not exist in epoll
		return 0;
	struct epoll_event ep_ev = {0};
	ep_ev.data.ptr = ev;
	ev->status = 0;

	epoll_ctl(epfd, EPOLL_CTL_DEL, ev->fd, &ep_ev);
	return 0;
}

//set new events to fd
int io_event_set(io_event *ev, int fd, io_events_callback callback, void *arg)
{
	if (ev == NULL)
		return -1;
	ev->fd = fd;
	ev->callback = callback;
	ev->arg = arg;
	return 0;
}

int reactor_init(Reactor *reactor)
{
	if (reactor == NULL)
		return -1;
	memset(reactor, 0, sizeof(Reactor));
	reactor->epfd = epoll_create(1);
	if (reactor->epfd < 0)
		return -1;
	reactor->events = (io_event *)malloc(MAX_FD_EVENTS * sizeof(io_event));
	if (reactor->events == NULL)
	{
		printf("events malloc error\n");
		close(reactor->epfd);
		return -1;
	}
	memset(reactor->events, 0, MAX_FD_EVENTS * sizeof(io_event));
	return 0;
}

int reactor_delete(Reactor *reactor)
{
	if (reactor == NULL)
		return -1;
	close(reactor->epfd);
	if (reactor->events != NULL)
		free(reactor->events);
	return 0;
}

void reactor_run(Reactor *reactor)
{
	struct epoll_event events[EPOLL_SIZE];
	reactor->stop = 0;
	while (!reactor->stop)
	{
		int nready = epoll_wait(reactor->epfd, events, EPOLL_SIZE, -1);
		if (nready < 0)
			continue;
		for (int i = 0; i < nready; ++i)
		{
			io_event *ev = (io_event *)events[i].data.ptr;
			//readable
			if ((ev->events & EPOLLIN) && (events[i].events & EPOLLIN))
				ev->callback(ev->fd, events[i].events, ev->arg);
			if ((ev->events & EPOLLOUT) && (events[i].events & EPOLLOUT))
				ev->callback(ev->fd, events[i].events, ev->arg);
		}
	}
}