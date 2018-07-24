/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
This source code is licensed under the MIT License
*/

#include<iostream>
#include<sstream>
//#include<windows.h>
#include"..\FileTransmit\socket.h"
#include"utils.h"
using namespace std;

void TestMAC()
{
	cout << "client start ..." << endl;
	Socket client("127.0.0.1", 8490);
	client.ClientStart();

	client.SendFile("F:\\OpenSource\\cpp.chm", "~/Download/");
	Sleep(1000);
	//client.Close();
}

DWORD WINAPI server(LPVOID lParam) //windows thread函数标准格式
{
	cout << "server start ..." << endl;
	Socket server(8490);
	server.ServerStart();
	cout << "server accept connection ..." << endl;
	server.Accept();
	cout << server.GetIp() << ":" << server.GetPort() << endl;
	server.RecvFile();
	return 0;
}

DWORD WINAPI client(LPVOID lParam) //windows thread函数标准格式
{
	cout << "client start ..." << endl;
	Socket client("127.0.0.1", 8490);
	client.ClientStart();

	client.SendFile("F:\\Android\\FirstApp.apk", "F:\\FirstApp.apk");
	Sleep(1000);
	//client.Close();
	return 0;
}

void TestSendFile()
{
	cout << "server start ..." << endl;
	Socket server(8490);
	server.ServerStart();

	HANDLE handle;
	handle = CreateThread(NULL, 0, client, NULL, 0, NULL);

	cout << "server accept connection ..." << endl;
	server.Accept();
	cout << server.GetIp() << ":" << server.GetPort() << endl;
	server.RecvFile();
	WaitForSingleObject(handle, INFINITE);
}

//two threads one for sending file the other for receiving file
void ThreadSendFile()
{
	HANDLE handle[2];
	handle[0] = CreateThread(NULL, 0, server, NULL, 0, NULL);
	Sleep(1000);
	handle[1] = CreateThread(NULL, 0, client, NULL, 0, NULL);
	WaitForMultipleObjects(2, handle, TRUE, INFINITE);
}

void TestBase()
{
	cout << "server start..." << endl;
	Socket server(8490);
	// server.Socket();  //此处用于检查socket是否创建成功
	// server.Bind();
	// server.Listen();
	server.ServerStart();

	cout << "client0 start..." << endl;
	Socket client0("127.0.0.1", 8490);
	// client0.Socket();
	// client0.Connect();
	client0.ClientStart();

	// cout<<"client1 start..."<<endl;
	// CSocket client1("127.0.0.1",8490);
	// client1.Socket();
	// client1.Connect();


	cout << "server accept connection0..." << endl;
	server.Accept();
	cout << server.GetIp() << endl;
	cout << server.GetPort() << endl;

	//服务端向客户端发送消息
	cout << "send hello to client ..." << endl;
	server.Send("hello client0");

	//客户端接收
	cout << "get message ..." << endl;
	int len;
	len = client0.Receive();
	cout << len << endl;
	cout << client0.recvbuff << endl;

	//客户端向服务端发送消息

	// cout<<"server accept connection1..."<<endl;
	// server.Accept();
	// cout<<server.GetIp()<<endl; 
	// cout<<server.GetPort()<<endl;

	//死循环不让程序停止运行
	while (true) {}
}