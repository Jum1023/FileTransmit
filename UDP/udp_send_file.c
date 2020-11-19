#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/*
server side send file to client
server有公网IP,client只有局域网IP
server要先接受到client请求，获取client地址后，再给client发送数据

compile command
gcc -o server udp_send_file.c
*/

#pragma pack(1)
typedef struct Data
{
	int sequenceId; // -1 file not exist
	char data[0];
} Data;
#pragma pack(0)

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000
#define BUFFER_SIZE 1024

struct sockaddr_in
createServerInfo(const char *ip, int port)
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

int getFileSize(FILE *fp)
{
	fpos_t pos = {0};
	fgetpos(fp, &pos);
	fseek(fp, 0L, SEEK_END);
	int filesize = ftell(fp);
	// rewind(fp); //seeking to the beginning
	fsetpos(fp, &pos);
	return filesize;
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

	buffer[recv_buffer_size] = '\0'; //recieve the first buff as file name
	printf("Client IP %s port %d: %s\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port), buffer);

	FILE *fp = fopen(buffer, "rb");
	if (!fp)
	{
		printf("file not exist\n");
		Data data = {-1};
		data.sequenceId = htonl(data.sequenceId);
		sendto(sock_fd, (void *)&data, sizeof(Data), 0,
			   (struct sockaddr *)&client_addr, sizeof(client_addr));
		return 0;
	}
	int filesize = getFileSize(fp);
	Data *data = (Data *)buffer;
	data->sequenceId = filesize;
	data->sequenceId = htonl(data->sequenceId);
	sendto(sock_fd, data, sizeof(Data), 0,
		   (struct sockaddr *)&client_addr, sizeof(client_addr));

	//send file to client
	int nbytes = fread(data->data, sizeof(char), 1000, fp);
	for (int count = 0; nbytes != 0; ++count)
	{
		//filling the sequence id
		data->sequenceId = count;
		data->sequenceId = htonl(data->sequenceId);
		memcpy(buffer, &data, sizeof(Data));

		int send_buffer_size = sendto(sock_fd, buffer, nbytes + sizeof(Data), 0,
									  (struct sockaddr *)&client_addr, sizeof(client_addr));
		nbytes = fread(data->data, sizeof(char), 1000, fp);
	}
	sprintf(msg, "file send over");
	sendto(sock_fd, msg, strlen(msg), 0,
		   (struct sockaddr *)&client_addr, sizeof(client_addr));
	printf("%s\n", msg);

	//close socket
	close(sock_fd);
	//close file
	fclose(fp);

	return 0;
}