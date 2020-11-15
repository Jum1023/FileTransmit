#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> //for sleep

/*
compile command
gcc -o test test_tcp_client.c
*/

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000
#define BUFFER_SIZE 1024
#define CLIENT_NUM 10

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

int createTcpClient(const char *connect_server_ip, int connect_server_port)
{
	//create socket file descriptor
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		printf("socket creation failed\n");
		return 0;
	}

	//init server address information
	struct sockaddr_in server_addr = createServerInfo(connect_server_ip, connect_server_port);
	int res = connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
	if (res < 0)
	{
		printf("unable to connect to server\n");
		return 0;
	}
	return sock_fd;
}

int main()
{
	char buffer[BUFFER_SIZE], msg[BUFFER_SIZE];

	int client[CLIENT_NUM];
	for (int i = 0; i < CLIENT_NUM; ++i)
	{
		client[i] = createTcpClient(SERVER_IP, SERVER_PORT);
		printf("client %d: connected successfully\n", i);
		sprintf(msg, "hello server this is client %d\n", i);
		//send msg to server
		int send_buffer_size = send(client[i], msg, strlen(msg), 0);

		//receive msg from server
		int recv_buffer_size = recv(client[i], buffer, BUFFER_SIZE, 0);
		buffer[recv_buffer_size] = '\0';
		printf("client %d received server buffer: %s\n", i, buffer);
	}

	//sleep 2s
	sleep(2);

	//close socket
	for (int i = 0; i < CLIENT_NUM; ++i)
		close(client[i]);

	return 0;
}