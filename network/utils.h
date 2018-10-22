/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
This source code is licensed under the MIT License
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