/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/
#ifndef _SOCKET_H_
#define _SOCKET_H_

#include<string>
#include<iostream>
#include<winsock2.h>
using namespace std;

class __declspec(dllexport) Socket
{
	//Attributes
private:
	static const int MAXSIZE = 4096;

	//服务端用socekt
	SOCKET ListenSocket;
	//已经连接成功的socekt
	SOCKET LinkedSocket;

	//socekt信息
	SOCKADDR_IN socketinfo;
	SOCKADDR_IN server;

public:
	char recvbuff[MAXSIZE];

public:
	Socket();
	Socket(const int port);
	Socket(const string& ip, const int port);
	virtual ~Socket();

	//Operations
		//创建socket
		// int Socket();

		//绑定端口
	int Bind();

	//监听端口
	int Listen();

	//连接端口
	int Connect();

	//判断是否连接
	void IsConnected();

	//发送文件
	int SendFile(const string localpath, const string serverpath);

	//接收文件
	int RecvFile();

	//设置连接选项
	void SetSocketOption();

	//接受
	SOCKET Accept();

	//发送
	int Send(string sendbuff) const;
	int Send(const char* sendbuff, const int buffsize) const;

	//接收
	int Receive();

	//启动
	int ServerStart();
	int ClientStart();

	//关闭
	int Close();

	//获取ip
	string GetIp() const;

	//获取端口
	int GetPort() const;
	//Implemention
};

#endif