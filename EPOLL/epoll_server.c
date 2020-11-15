#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define EPOLL_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000
#define BUFFER_SIZE 1024
#define max(a, b) (((a) > (b)) ? (a) : (b))

/*
linux help manual
open a terminal then type the following command
man epoll

edge-triggered (ET) 一次触发适合小块数据
level-triggered (LT) 一直触发适合大块数据(大于recvbuff)
listen 适合用LT

compile command
gcc -o server epoll_server.c
*/

struct sockaddr_in createServerInfo(const char *ip, int port)
{
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	// Filling server information
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	//server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	return server_addr;
}

int main()
{
	char buffer[BUFFER_SIZE];
	char msg[BUFFER_SIZE];

	//create socket file descriptor
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		printf("socket creation failed\n");
		return 0;
	}
	//init server address information
	struct sockaddr_in server_addr = createServerInfo(SERVER_IP, SERVER_PORT);
	//bind to address
	int res = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
	if (res < 0)
	{
		printf("bind failed,may be port %d already in use\n", SERVER_PORT);
		return 0;
	}
	//listen
	res = listen(sock_fd, 5);
	if (res < 0)
	{
		printf("Error while listening\n");
		return 0;
	}

	int epoll_fd = epoll_create(1); // greater than 0 for backward capability
	struct epoll_event ev, events[EPOLL_SIZE] = {0};

	ev.events = EPOLLIN;
	ev.data.fd = sock_fd; //add server sock_fd to epoll
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &ev);

	while (1)
	{
		int nready = epoll_wait(epoll_fd, events, EPOLL_SIZE, -1);
		if (nready == -1)
		{
			printf("epoll_wait error\n");
			break;
		}

		for (int i = 0; i < nready; ++i)
		{
			if (events[i].data.fd == sock_fd) //server sock_fd ready
			{
				// accept
				struct sockaddr_in client_addr;
				socklen_t client_addr_len = sizeof(struct sockaddr_in); //if set to 0,we won't get client_addr
				int client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = client_fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
			}
			else //client fd
			{
				int fd = events[i].data.fd;
				//receive msg from client
				int recv_buffer_size = recv(fd, buffer, BUFFER_SIZE, 0);
				if (recv_buffer_size < 0) //no data
				{
					if (errno == EAGAIN || errno == EWOULDBLOCK)
						printf("already read in other thread\n");
					else
					{
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
						close(fd);
					}
				}
				else if (recv_buffer_size == 0)
				{
					printf("disconnected\n");
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
					close(fd);
				}
				else //ready read data
				{
					buffer[recv_buffer_size] = '\0';
					printf("client msg: %s\n", buffer);
					sprintf(msg, "hello client");
					//send msg to clinet
					int send_buffer_size = send(fd, msg, strlen(msg), 0);
				}
			}
		}
	}

	return 0;
}