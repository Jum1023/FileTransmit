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

#pragma pack(1)
typedef struct Data
{
	int sequenceId; // -1 file not exist
	char data[0];
} Data;
#pragma pack(0)

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

/*
模拟三次握手确认机制
由发送端确认，并超时重传请求
接收端在接收到确认ack后再执行下一步

sender             reciever
		   data
发送数据 =========>
			ack     接收数据返回ack
收到ack	<=========
			ack
返回ack	=========>  收到ack执行下一步

1. 发送端data丢失，发送端重传
2. 接收端ack丢失，发送端重传
3. 发送端ack丢失，发送端重传
*/
int send_and_wait_for_ack(int socket, const void *buffer, size_t length, int flags,
						  const struct sockaddr *dest_addr, socklen_t dest_len)
{
	char recv_buff[BUFFER_SIZE] = {0};
	char msg[] = "ack";

	struct timeval tv = {0};
	tv.tv_sec = 5; //5s
	setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

	while (1)
	{
		sendto(socket, buffer, length, flags, dest_addr, dest_len); //data
		int nbyte = recvfrom(socket, recv_buff, length, flags, dest_addr, dest_len);
		if (nbyte < 0)
			continue;
		recv_buff[nbyte] = '\0';
		if (nbyte > 0 && strcmp(msg, recv_buff) == 0)
		{
			sendto(socket, msg, strlen(msg), flags, dest_addr, dest_len);
			break;
		}
	}
	return 0;
}

int main()
{
	char buffer[BUFFER_SIZE] = {0};
	char filename[] = "cpptools-osx.vsix";

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
	int send_buffer_size = sendto(sock_fd, filename, strlen(filename), 0,
								  (struct sockaddr *)&server_addr, sizeof(server_addr));

	//receive msg from server
	socklen_t server_addr_len = sizeof(struct sockaddr_in);
	int recv_buffer_size = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
									(struct sockaddr *)&server_addr, &server_addr_len);
	//ack
	sendto(sock_fd, "ack", strlen("ack"), 0,
		   (struct sockaddr *)&server_addr, sizeof(server_addr));
	buffer[recv_buffer_size] = '\0';
	Data *data = (Data *)buffer;
	data->sequenceId = ntohl(data->sequenceId);

	FILE *fp = fopen(filename, "rb+"); //特定位置以覆盖的方式更改文件(不是追加)
	if (!fp)
	{
		printf("open file failed\n");
		return 0;
	}

	//recieve file begin
	recv_buffer_size = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
								(struct sockaddr *)&server_addr, &server_addr_len);
	while (recv_buffer_size != 0)
	{
		buffer[recv_buffer_size] = '\0';
		rewind(fp);
		fseek(fp, data->sequenceId * 1000, SEEK_SET);
		fwrite(buffer, sizeof(char), recv_buffer_size, fp);
		recv_buffer_size = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
									(struct sockaddr *)&server_addr, &server_addr_len);
	}

	printf("file recv over");

	//close socket
	close(sock_fd);
	fclose(fp);

	return 0;
}