/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
This source code is licensed under the MIT License
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include<fstream>
#include "socket.h"
#include "md5.h"
#pragma comment(lib, "ws2_32.lib")  

//default initialization
Socket::Socket()
{
	WSADATA wsaData;
	int iResult;
	//initiates use of the Winsock DLL by a process
	iResult = WSAStartup(0x0202, &wsaData);
	if (iResult != 0)
	{
		cout << "initialize error:" << WSAGetLastError() << endl;
	}

	server.sin_family = PF_INET;
	server.sin_port = htons(8490);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
}


Socket::Socket(const int port)
{
	WSADATA wsaData;
	int iResult;
	WSAStartup(0x0202, &wsaData);
	iResult = WSAStartup(0x0202, &wsaData);
	if (iResult != 0)
	{
		cout << "initialize error:" << WSAGetLastError() << endl;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

}


Socket::Socket(const string& ip, const int port)
{
	WSADATA wsaData;
	int iResult;
	WSAStartup(0x0202, &wsaData);
	iResult = WSAStartup(0x0202, &wsaData);
	if (iResult != 0)
	{
		cout << "initialize error:" << WSAGetLastError() << endl;
	}

	server.sin_family = PF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(ip.c_str());
}

Socket::~Socket()
{}


int Socket::Bind()
{
	return bind(ListenSocket, (struct sockaddr *) &server, sizeof(SOCKADDR_IN));
}


int Socket::Listen()
{
	return listen(ListenSocket, 5);
}

//连接端口
//端口不在监听，不会阻塞，立即返回
int Socket::Connect()
{
	return connect(LinkedSocket, (struct sockaddr *) &server, sizeof(SOCKADDR_IN));
}


void Socket::IsConnected()
{}

//send file
//1 means success
//-1 means disconnected
//-2 means localpath not exists
int Socket::SendFile(const string localpath, const string serverpath)
{
	int iResult;
	ifstream file(localpath, ios::binary);
	char sendbuff[MAXSIZE];
	int size;
	strcpy(sendbuff, serverpath.c_str());
	sendbuff[serverpath.length()] = 3;

	char md5[33];
	md5[32] = '\0';
	iResult = GetMD5(localpath.c_str(), md5);
	if (iResult == -1)
	{
		return -2;
	}
	for (int i = 0; i < 32; i++)
	{
		sendbuff[267 + i] = md5[i];
	}
	sendbuff[299] = '\0';
	//can be less than the number requested to be sent in the len parameter
	//300 = 260 filename length + 32 bytes of md5
	iResult = Send(sendbuff, MAXSIZE);
	if (iResult == SOCKET_ERROR)
	{
		//WSAGetLastError();
	}
	file.read(sendbuff, sizeof(char)*MAXSIZE);
	size = file.gcount();
	while (size == MAXSIZE)
	{
		Send(sendbuff, MAXSIZE);
		file.read(sendbuff, sizeof(char)*MAXSIZE);
		size = file.gcount();
	}
	if (size)
	{
		Send(sendbuff, size);
	}

	//transmit over,close socket
	iResult = Close();
	if (iResult == SOCKET_ERROR)
	{
		//WSAGetLastError();
	}
	return 1;
}

//accept file
int Socket::RecvFile()
{
	string path, fileinfo;
	//guarantee of getting 4096 bytes
	int iResult, fileinfosize = MAXSIZE;
	do
	{
		iResult = Receive();
		if (iResult > 0)
		{
			fileinfosize -= iResult;
			fileinfo.assign(recvbuff, 0, iResult);
		}
		else if (iResult == SOCKET_ERROR)
		{
			//an error occurs
		}
		else if (iResult == 0)
		{
			//the connection has been gracefully closed
		}
	} while (fileinfosize > 0);


	//if the file already exists, check the md5
	char md5[33];
	md5[32] = '\0';
	for (int i = 0; i < 32; i++)
	{
		md5[i] = fileinfo[267 + i];
	}
	for (unsigned int i = 0; i < fileinfo.length(); i++)
	{
		if (fileinfo[i] == 3)
		{
			path.append(fileinfo, 0, i);
			break;
		}
	}
	ifstream existsfile(path);
	if (existsfile.is_open())
	{
		existsfile.close();
		char existsfilemd5[33];
		existsfilemd5[32] = '\0';
		GetMD5(path.c_str(), md5);
		if (strcmp(md5, existsfilemd5))
		{
			iResult = Close();
			if (iResult == SOCKET_ERROR)
			{
				//WSAGetLastError();
			}
			return 0;
		}
		iResult = remove(path.c_str());
		//remove file failed
		if (iResult != 0)
		{
			//what to do
		}
	}
	ofstream file(path, ios::app | ios::binary);
	iResult = Receive();
	while (iResult > 0)
	{
		file.write(recvbuff, iResult);
		iResult = Receive();
	}

	if (iResult == SOCKET_ERROR)
	{
		//an error occurs
	}
	else if (iResult == 0)
	{
		//the connection has been gracefully closed
	}

	//finish file transfer
	file.close();
	iResult = Close();
	if (iResult == SOCKET_ERROR)
	{
		//WSAGetLastError();
	}
	cout << "send over" << endl;
	return 0;
}

//set timeout
void Socket::SetSocketOption()
{
	int timeout = 3000; //3s
	setsockopt(LinkedSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(int));
	setsockopt(LinkedSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(int));
}

//accept a connection
SOCKET Socket::Accept()
{
	int socketinfosize = sizeof(SOCKADDR_IN);
	LinkedSocket = accept(ListenSocket, (struct sockaddr *) &socketinfo, &socketinfosize);
	return LinkedSocket;
}

//send string buffer
int Socket::Send(string sendbuff) const
{
	return send(LinkedSocket, sendbuff.c_str(), sendbuff.length(), 0);
}

int Socket::Send(const char* sendbuff, const int buffsize) const
{
	return send(LinkedSocket, sendbuff, buffsize, 0);
}
//receive string buffer
int Socket::Receive()
{
	int nchar = recv(LinkedSocket, recvbuff, MAXSIZE, 0);
	return nchar;
}

//start server
//contains error checking，socket creation failure included
int Socket::ServerStart()
{
	int iResult;
	try
	{
		ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (ListenSocket == INVALID_SOCKET)
		{
			cout << "socket error:" << WSAGetLastError() << endl;
			return 0;
		}
		//SetSocketOption();
		iResult = Bind();
		if (iResult == SOCKET_ERROR)
		{
			//WSAGetLastError();
			return 0;
		}
		iResult = Listen();
		if (iResult == SOCKET_ERROR)
		{
			//WSAGetLastError();
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}
	return 1;
}
int Socket::ClientStart()
{
	int iResult;
	try
	{
		LinkedSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (LinkedSocket == INVALID_SOCKET)
		{
			//WSAGetLastError();
			return 0;
		}
		//SetSocketOption();

		//set non-blocked model
		//unsigned long ul = 1;
		//ioctlsocket(LinkedSocket, FIONBIO, (unsigned long *)&ul);//set non-blocked model
		iResult = Connect();
		if (iResult == SOCKET_ERROR)
		{
			//WSAGetLastError();
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}
	return 1;
}

//close connected socket
int Socket::Close()
{
	return closesocket(LinkedSocket);
}

//get connected socket ip
string Socket::GetIp() const
{
	return inet_ntoa(socketinfo.sin_addr);
}

//get connected socket port
int Socket::GetPort() const
{
	return ntohs(socketinfo.sin_port);
}