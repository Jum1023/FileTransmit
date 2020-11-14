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
gcc -o client raw_client.c
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
	char msg[] = "hello server";

	//create socket file descriptor
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		printf("socket creation failed\n");
		return 0;
	}

	//init server address information
	struct sockaddr_in server_addr = createServerInfo(SERVER_IP, SERVER_PORT);
	//send msg to server
	int send_buffer_size = sendto(sock_fd, msg, strlen(msg), 0,
								  (struct sockaddr *)&server_addr, sizeof(server_addr));

	//receive msg from server
	socklen_t server_addr_len = sizeof(struct sockaddr_in);
	int recv_buffer_size = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
									(struct sockaddr *)&server_addr, &server_addr_len);
	buffer[recv_buffer_size] = '\0';
	printf("Server : %s\n", buffer);

	//close socket
	close(sock_fd);

	return 0;
}