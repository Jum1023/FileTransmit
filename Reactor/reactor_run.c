#include "callback.h"
#include "common.h"
#include "createsocket.h"
#include "reactor.h"

#define EPOLL_SIZE 1024
#define SERVER_IP "81.69.226.177"
#define SERVER_PORT 16384
#define BUFFER_SIZE 1024

void reactor_addlistener(Reactor *reactor, int sockfd, io_events_callback acceptor)
{
	io_event_set(&reactor->events[sockfd], sockfd, acceptor, reactor);
	io_event_add(reactor->epfd, EPOLLIN, &reactor->events[sockfd]);
}

int main()
{
	int server_fd = initServerSocket(SERVER_IP, SERVER_PORT);
	if (server_fd < 0)
		return 0;

	Reactor reactor;
	reactor_init(&reactor);
	reactor_addlistener(&reactor, server_fd, accept_callback);

	// infinite loop
	reactor_run(&reactor);

	// finished
	reactor_delete(&reactor);
	close(server_fd);
	return 0;
}