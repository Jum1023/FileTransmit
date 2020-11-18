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
gcc -o server udp_recv_file.c
*/

#define SERVER_IP "149.28.230.65"
// #define SERVER_IP "127.0.0.1"
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
	char filename[] = "cpptools-osx.vsix";

	//create socket file descriptor
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		printf("socket creation failed\n");
		return 0;
	}
	FILE* f=fopen(filename,"wb");
	if(!f)
	{
		printf("file not exist\n");
		return 0;
	}

	//init server address information
	struct sockaddr_in server_addr = createServerInfo(SERVER_IP, SERVER_PORT);
	//send msg to server
	int send_buffer_size = sendto(sock_fd, filename, strlen(filename), 0,
								  (struct sockaddr *)&server_addr, sizeof(server_addr));

	//receive msg from server
	socklen_t server_addr_len = sizeof(struct sockaddr_in);
	int recv_buffer_size = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
									(struct sockaddr *)&server_addr, &server_addr_len);
	while(recv_buffer_size!=0)
	{
		buffer[recv_buffer_size] = '\0';
		printf("%d\n",recv_buffer_size);
		fwrite(buffer,sizeof(char),recv_buffer_size,f);
		recv_buffer_size = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
									(struct sockaddr *)&server_addr, &server_addr_len);
	}

	printf("file recv over");

	//close socket
	close(sock_fd);
	fclose(f);

	return 0;
}