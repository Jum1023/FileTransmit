/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

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