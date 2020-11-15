#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*
linux help manual
open a terminal then type the following command
man select

compile command
gcc -o server select_server.c
*/

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000
#define BUFFER_SIZE 1024
#define max(a, b) (((a) > (b)) ? (a) : (b))

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

int main(void)
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

	// select
	int max_fd = 0;
	fd_set rfds, rset;
	FD_ZERO(&rfds);
	FD_SET(sock_fd, &rfds); //set server socket
	max_fd = max(max_fd, sock_fd + 1);

	/* Wait up to five seconds. */
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	while (1)
	{
		rset = rfds;
		// int res = select(max_fd, &rset, NULL, NULL, &timeout);
		int res = select(max_fd, &rset, NULL, NULL, NULL); //infinite wait
		if (res < 0)
		{
			printf("select error\n");
			return 0;
		}
		if (res == 0)
			printf("select time limit expires\n");
		else //number of ready descriptors that are contained in the descriptor set
		{
			if (FD_ISSET(sock_fd, &rset)) //server sock_fd ready
			{
				// accept
				struct sockaddr_in client_addr;
				socklen_t client_addr_len = sizeof(struct sockaddr_in); //if set to 0,we won't get client_addr
				int client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);
				FD_SET(client_fd, &rfds);
				max_fd = max(max_fd, client_fd + 1);
			}
			// read other fd
			for (int fd = sock_fd + 1; fd < max_fd; ++fd)
			{
				if (FD_ISSET(fd, &rset)) //client add fd ready for read
				{
					//receive msg from client
					int recv_buffer_size = recv(fd, buffer, BUFFER_SIZE, 0);
					if (recv_buffer_size < 0) //no data
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
							printf("already read in other thread\n");
						else
						{
							FD_CLR(fd, &rfds);
							close(fd);
						}
					}
					else if (recv_buffer_size == 0)
					{
						printf("disconnected\n");
						FD_CLR(fd, &rfds);
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
	}

	return 0;
}