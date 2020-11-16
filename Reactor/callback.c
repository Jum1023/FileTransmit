#include "callback.h"
#include "common.h"
#include "reactor.h"

int send_callback(int fd, int events, void *arg)
{
	Reactor *reactor = (Reactor *)arg;
	if (reactor == NULL)
		return -1;
	io_event *ev = &reactor->events[fd];
	//logic process
	sprintf(ev->buffer, "hello client");
	int n = send(fd, ev->buffer, strlen(ev->buffer), 0);
	if (n > 0)
	{
		// printf("%s\n", ev->buffer);
		io_event_del(reactor->epfd, ev);
		io_event_set(ev, fd, recv_callback, reactor);
		io_event_add(reactor->epfd, EPOLLIN, ev);
	}
	else
	{
		close(fd);
		io_event_del(reactor->epfd, ev);
		printf("%s\n", strerror(errno));
	}
	return 0;
}

int recv_callback(int fd, int events, void *arg)
{
	Reactor *reactor = (Reactor *)arg;
	if (reactor == NULL)
		return -1;
	io_event *ev = &reactor->events[fd];
	int n = recv(fd, ev->buffer, BUFFER_SIZE, 0);
	io_event_del(reactor->epfd, ev);
	if (n > 0)
	{
		ev->bufferMsgSize = n;
		ev->buffer[n] = '\0';
		//logic process
		printf("%s\n", ev->buffer);
		io_event_set(ev, fd, send_callback, arg);
		io_event_add(reactor->epfd, EPOLLOUT, ev);
	}
	else if (n == 0) //disconnected
	{
		close(fd);
	}
	else
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			printf("already read in other thread\n");
		else
			close(fd);
	}
	return 0;
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
	return 0;
}