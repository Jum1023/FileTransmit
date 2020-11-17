#include "common.h"

#define HTTP_SERVER_PORT 80
#define BUFFER_SIZE 1024

#define HTTP_VERSION "HTTP/1.1"
#define USER_AGENT "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Safari/537.36\n"
#define ENCODE_TYPE "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
#define CONNECTION_TYPE "Connection: close\n"

int http_create_socket(const char *ip)
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(HTTP_SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(ip);
	if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) < 0)
	{
		close(sock_fd);
		printf("can not connect to server\n");
		return -1;
	}
	fcntl(sock_fd, F_SETFL, O_NONBLOCK);
	return sock_fd;
}

char *host_to_ip(const char *hostname)
{
	struct hostent *host_entry = gethostbyname(hostname); //gethostbyname_r / getaddrinfo
	if (!host_entry)
		return NULL;
	// printf("host ip: %s\n", *host_entry->h_addr_list);
	return inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);
}

char *http_send_request(int sock_fd, const char *hostname, const char *resource)
{
	char buffer[BUFFER_SIZE] = {0};
	int len = sprintf(buffer,
					  "GET %s %s\n\
Host: %s\n\
%s\n",
					  resource, HTTP_VERSION,
					  hostname,
					  CONNECTION_TYPE);
	printf("request buffer:\n%s\n", buffer);
	send(sock_fd, buffer, len, 0);

	struct timeval tv = {0};
	tv.tv_sec = 5;

	fd_set fdread;
	FD_ZERO(&fdread);
	FD_SET(sock_fd, &fdread);

	char *res = NULL;
	int resSize = 0;
	while (1)
	{
		int nready = select(sock_fd + 1, &fdread, NULL, NULL, &tv);
		if (nready == 0 || !(FD_ISSET(sock_fd, &fdread)))
			break;

		len = recv(sock_fd, buffer, BUFFER_SIZE, 0);
		if (len == 0)
			break;
		res = (char *)realloc(res, sizeof(char) * (resSize + len + 1));
		strncat(&res[resSize], buffer, len);
		resSize += len;
	}

	return res;
}

int http_client_commit(const char *hostname, const char *resource)
{
	char *ip = host_to_ip(hostname);
	printf("==============request==============\n");
	printf("DNS %s ==> %s\n", hostname, ip);
	int sock_fd = http_create_socket(ip);
	if (sock_fd < 0)
	{
		printf("socket creation failed\n");
		return 0;
	}
	char *content = http_send_request(sock_fd, hostname, resource);
	if (content == NULL || !*content)
	{
		printf("%s: no data\n", hostname);
		return 0;
	}
	printf("==============response==============\n");
	puts(content);
	close(sock_fd);
	free(content);
	return 0;
}

struct http_request
{
	char *hostname;
	char *resource;
};

int main(int argc, char *argv[])
{
	struct http_request requests[] = {
		{"www.baidu.com", "/img/flexible/logo/pc/result.png"},
		{"www.jumhorn.com", "/gomoku/gobang.html"},
	};
	int size = ARRAY_SIZE(requests);
	for (int i = 0; i < size; ++i)
		http_client_commit(requests[i].hostname, requests[i].resource);
	return 0;
}