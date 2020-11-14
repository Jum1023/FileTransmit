#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000
#define BUFFER_SIZE 1024

static int g_clientfd = 0;

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

void processSignal(int sig)
{
	char buffer[BUFFER_SIZE];
	char msg[] = "hello client";
	//receive msg from client
	int recv_buffer_size = recv(g_clientfd, buffer, BUFFER_SIZE, 0);
	buffer[recv_buffer_size] = '\0';
	printf("client messge:%s\n", buffer);
	//send msg to clinet
	int send_buffer_size = send(g_clientfd, msg, strlen(msg), 0);
}

int main(int argc, char *argv[])
{
	//create socket file descriptor
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	//init server address information
	struct sockaddr_in server_addr = createServerInfo(SERVER_IP, SERVER_PORT);
	int res = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
	if (res < 0)
	{
		printf("bind failed,may be port %d already in use\n", SERVER_PORT);
		return 0;
	}
	// listen
	res = listen(sock_fd, 1);
	if (res < 0)
	{
		printf("Error while listening\n");
		return 0;
	}
	//accept
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(struct sockaddr_in); //if set to 0,we won't get client_addr
	g_clientfd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);

	//signal/slots
	struct sigaction sigio_action;
	sigio_action.sa_flags = 0;
	sigio_action.sa_handler = processSignal;
	sigaction(SIGIO, &sigio_action, NULL);

	fcntl(g_clientfd, F_SETOWN, getpid());
	int flags = fcntl(g_clientfd, F_GETFL, 0);
	flags |= O_ASYNC | O_NONBLOCK;
	fcntl(g_clientfd, F_SETFL, flags);

	while (1)
		sleep(1000);

	close(sock_fd);

	return 0;
}