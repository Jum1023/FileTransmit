/*******************************************************************
		[Jum] created on 20170824
		本文件包含测试socekt类函数的定义

********************************************************************/

#ifndef _UTILS_H_
#define _UTILS_H_

#include<windows.h>

//部分函数声明部分
void TestBase();
void TestSendFile();
void TestMAC();
void ThreadSendFile();
DWORD WINAPI server(LPVOID lParam);
DWORD WINAPI client(LPVOID lParam);

#endif