#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*
server side send file to client
server有公网IP,client只有局域网IP
server要先接受到client请求，获取client地址后，再给client发送数据

compile command
gcc -o server udp_send_file.c
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
	server_addr.sin_addr.s_addr = INADDR_ANY;
	// server_addr.sin_addr.s_addr = inet_addr(ip);
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
		printf("socket creation failed\n");
		return 0;
	}

	//init server address information
	struct sockaddr_in server_addr = createServerInfo(SERVER_IP, SERVER_PORT);
	int res = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
	if (res < 0)
	{
		printf("bind failed,may be port %d already in use\n", SERVER_PORT);
		return 0;
	}

	//receive msg from client
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(struct sockaddr_in); //if set to 0,we won't get client_addr
	int recv_buffer_size = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
									(struct sockaddr *)&client_addr, &client_addr_len);

	buffer[recv_buffer_size] = '\0';//recieve the first buff as file name
	printf("Client IP %s port %d: %s\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port),buffer);

	FILE* f=fopen(buffer,"rb");
	if(!f)
	{
		printf("file not exist\n");
		return 0;
	}
	//send file to client
	int nbytes = fread(buffer,sizeof(char),1000,f);
	while(nbytes!=0)
	{
		int send_buffer_size = sendto(sock_fd, buffer, nbytes, 0,
										(struct sockaddr *)&client_addr, sizeof(client_addr));
		printf("%d\n",send_buffer_size);
		nbytes = fread(buffer,sizeof(char),1000,f);
	}
	sendto(sock_fd, buffer, 0, 0,
						(struct sockaddr *)&client_addr, sizeof(client_addr));
	printf("file send over\n");

	//close socket
	close(sock_fd);
	//close file
	fclose(f);

	return 0;
}