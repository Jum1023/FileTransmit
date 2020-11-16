#include "callback.h"
#include "common.h"
#include "reactor.h"

int send_callback(int fd, int events, void *arg)
{
	Reactor *reactor = (Reactor *)arg;
	if (reactor == NULL)
		return -1;
	io_event *ev = &reactor->events[fd];
	int n = send(fd, ev->buffer, BUFFER_SIZE, 0);
	if (n > 0)
	{
		printf("%s\n", ev->buffer);
		io_event_add(reactor->epfd, EPOLLIN, ev);
	}
}

int recv_callback(int fd, int events, void *arg)
{
	Reactor *reactor = (Reactor *)arg;
	if (reactor == NULL)
		return -1;
	io_event *ev = &reactor->events[fd];
	int n = recv(fd, ev->buffer, BUFFER_SIZE, 0);
	if (n > 0)
	{
		ev->bufferMsgSize = n;
		ev->buffer[n] = '\0';
		io_event_set(ev, fd, send_callback, arg);
		io_event_add(reactor->epfd, EPOLLOUT, ev);
	}
	else if (n == 0) //disconnected
	{
		close(fd);
		io_event_del(reactor->epfd, EPOLLIN, ev);
	}
	else
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			printf("already read in other thread\n");
		else
		{
			close(fd);
			io_event_del(reactor->epfd, EPOLLIN, ev);
		}
	}
}

int accept_callback(int fd, int events, void *arg)
{
	Reactor *reactor = (Reactor *)arg;
	if (reactor == NULL)
		return -1;
	// accept
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(struct sockaddr_in); //if set to 0,we won't get client_addr
	int client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_addr_len);
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	io_event_set(&reactor->events[client_fd], client_fd, recv_callback, reactor);
	io_event_add(reactor->epfd, EPOLLIN, &reactor->events[client_fd]);
}