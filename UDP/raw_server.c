#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*
linux help manual
open a terminal then type the following command
man udp
man sendto
man recvfrom

compile command
gcc -o server raw_server.c
*/

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000
#define BUFFER_SIZE 1024

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
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		printf("socket creation failed");
		return 0;
	}

	//init server address information
	struct sockaddr_in server_addr = createServerInfo(SERVER_IP, SERVER_PORT);
	bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));

	//receive msg from server
	struct sockaddr_in client_addr;
	socklen_t clinet_addr_len = sizeof(struct sockaddr_in); //if set to 0,we won't get client_addr
	int recv_buffer_size = recvfrom(sock_fd, (char *)buffer, BUFFER_SIZE, 0,
									(struct sockaddr *)&client_addr, &clinet_addr_len);

	buffer[recv_buffer_size] = '\0';
	printf("Client : %s\n", buffer);

	sprintf(msg, "receive msg from client\n IP: %s Port: %d",
			inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
	printf("%s\n", msg);
	//send msg to clinet
	int send_buffer_size = sendto(sock_fd, msg, strlen(msg), 0,
								  (struct sockaddr *)&client_addr, sizeof(client_addr));

	return 0;
}