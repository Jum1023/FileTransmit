/*******************************************************************
		[Jum] created on 20170824
		本文件包含Socket类的实现

********************************************************************/

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include<fstream>
#include "socket.h"
#pragma comment(lib, "ws2_32.lib")  

//默认初始化
Socket::Socket()
{
    WSADATA wsaData;
    WSAStartup(0x0202, &wsaData);

    server.sin_family = PF_INET;      
    server.sin_port = htons(8490);      
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
}

//服务端初始化方法
Socket::Socket(const int port)
{
    WSADATA wsaData;
    WSAStartup(0x0202, &wsaData); 

    server.sin_family = AF_INET;      
    server.sin_port = htons(port);      
    server.sin_addr.s_addr = htonl(INADDR_ANY); 

}

//客户端初始化方法
Socket::Socket(const string ip,const int port)
{
    WSADATA wsaData;
    WSAStartup(0x0202, &wsaData);

    server.sin_family = PF_INET;      
    server.sin_port = htons(port);      
    server.sin_addr.s_addr = inet_addr(ip.c_str());
}

Socket::~Socket()
{}

//创建socket
//这个方法可以重载，实现修改socekt流和协议
// int CSocket::Socket()
// {
//     ListenSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
//     LinkedSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

//     //错误检查
//     if(LinkedSocket==INVALID_SOCKET||ListenSocket==INVALID_SOCKET)
//     {
//         return 0;
//     }
//     else
//     {
//         return 1;
//     }
// }

//绑定端口
int Socket::Bind()
{
    return bind(ListenSocket, (struct sockaddr *) &server, sizeof(SOCKADDR_IN));
}

//监听端口
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

//判断是否连接
void Socket::IsConnected()
{}

//发送文件
//返回结果 
//1表示上传成功
//-1表示连接断开
//-2表示
int Socket::SendFile(const string localpath,const string serverpath)
{
    ifstream file(localpath,ios::binary);
    char sendbuff[MAXSIZE];
    int size;
	//int totalsize = 0;
    strcpy(sendbuff,serverpath.c_str());
    Send(sendbuff,MAXSIZE);
    
    file.read(sendbuff,sizeof(char)*MAXSIZE);
    size = file.gcount();
    while(size==MAXSIZE)
    {
		//cout << totalsize << endl;
		//if (!Send(sendbuff, MAXSIZE))
		//{
		//	cout << "sendfailed" << endl;
		//	Sleep(0.1);
		//	Send(sendbuff, MAXSIZE);
		//}
		//totalsize += size;
		Send(sendbuff, MAXSIZE);
        file.read(sendbuff,sizeof(char)*MAXSIZE);
        size=file.gcount();
    }
    if(size)
    {
        Send(sendbuff,size);
		//totalsize += size;
    }
	//cout << totalsize << endl;
	Close();
    return 1;
}

//接收文件,非阻塞设置
void Socket::RecvFile()
{
    int filemem = Receive();
	//cout << "filemen" << filemem << endl;
    string path(recvbuff);
	ofstream file(path, ios::app | ios::binary);
    int size=Receive(); 
	//int totalsize = 0;
    do
    {
		//cout << totalsize << endl;
        file.write(recvbuff,size); 
		//totalsize += size;
	} while ((size = Receive())>0);
	//cout << file.tellp() << endl;
	file.close();
	//if(size)
	//{
	//	//totalsize += size;
	//	//cout << totalsize << endl;
	//	file.write(recvbuff, size);
	//}
	//cout << "receive" << totalsize << endl;
	Close();
	cout << "send over" << endl;
}

//设置连接超时
void Socket::SetSocketOption()
{
    int timeout = 3000; //3s
    setsockopt(LinkedSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(int));
    setsockopt(LinkedSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(int));
}

//接受
SOCKET Socket::Accept()
{
    int socketinfosize = sizeof(SOCKADDR_IN);
    LinkedSocket = accept(ListenSocket, (struct sockaddr *) &socketinfo, &socketinfosize); 
    return LinkedSocket;
}

//发送字符串
int Socket::Send(string sendbuff)
{
    return send(LinkedSocket,sendbuff.c_str(),sendbuff.length(),0);
}

int Socket::Send(char* sendbuff,int buffsize)
{
    return send(LinkedSocket,sendbuff,buffsize,0);
}
//接收
int Socket::Receive()
{
    int nchar=recv(LinkedSocket, recvbuff, MAXSIZE, 0);
    return nchar; 
}

//启动
//这里包含错误检查，socket创建失败之类
int Socket::ServerStart()
{
    try
    {
        ListenSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);   
        //SetSocketOption();		
        Bind();
        Listen();
    }
    catch(...)
    {
        return 0;
    }
    return 1;
}
int Socket::ClientStart()
{
    try
    {  
        LinkedSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        //SetSocketOption();

		//设置为非阻塞
		//unsigned long ul = 1;
		//ioctlsocket(LinkedSocket, FIONBIO, (unsigned long *)&ul);//设置成非阻塞模式。
        Connect();
    }
    catch(...)
    {
        return 0;
    }
    return 1;
}

//关闭
int Socket::Close()
{
    return closesocket(LinkedSocket);
}

//获取ip
string Socket::GetIp()
{
    return inet_ntoa(socketinfo.sin_addr);
}

//获取端口
int Socket::GetPort()
{
    return ntohs(socketinfo.sin_port);
}
