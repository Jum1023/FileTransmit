#include "createsocket.h"

struct sockaddr_in createServerInfo(const char *ip, int port)
{
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	// Filling server information
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	//server_addr.sin_addr.s_addr = inet_addr(ip);
	return server_addr;
}

int initServerSocket(const char *serverip, int serverport)
{
	//create socket file descriptor
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		printf("socket creation failed\n");
		return -1;
	}
	//init server address information
	struct sockaddr_in server_addr = createServerInfo(serverip, serverport);
	//bind to address
	int res = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
	if (res < 0)
	{
		close(sock_fd);
		printf("bind failed,may be port %d already in use\n", serverport);
		return -1;
	}
	//listen
	res = listen(sock_fd, 5);
	if (res < 0)
	{
		close(sock_fd);
		printf("Error while listening\n");
		return -1;
	}
	return sock_fd;
}