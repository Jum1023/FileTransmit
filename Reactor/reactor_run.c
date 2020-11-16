#include "callback.h"
#include "common.h"
#include "createsocket.h"
#include "reactor.h"

#define EPOLL_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000
#define BUFFER_SIZE 1024

void reactor_addlistener(Reactor *reactor, int sockfd, io_events_callback acceptor)
{
	io_event_set(&reactor->events[sockfd], sockfd, acceptor, reactor);
	io_event_add(reactor->epfd, EPOLLIN, &reactor->events[sockfd]);
}

int main()
{
	int server_fd = initServerSocket(SERVER_IP, SERVER_PORT);
	Reactor reactor;
	reactor_init(&reactor);
	reactor_addlistener(&reactor, server_fd, accept_callback);
	// infinite loop
	reactor_run(&reactor);
	// finished
	reactor_destroy(&reactor);
	close(server_fd);
	return 0;
}