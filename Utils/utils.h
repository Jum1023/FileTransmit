/*******************************************************************
		[Jum] created on 20170824
		���ļ���������socekt�ຯ���Ķ���

********************************************************************/

#ifndef _UTILS_H_
#define _UTILS_H_

#include<windows.h>

//���ֺ�����������
void TestBase();
void TestSendFile();
void TestMAC();
void ThreadSendFile();
DWORD WINAPI server(LPVOID lParam);
DWORD WINAPI client(LPVOID lParam);

#endif