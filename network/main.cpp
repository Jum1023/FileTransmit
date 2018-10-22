/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/
#include<iostream>
#include"utilities.h"

int main(int argc, char* argv[])
{
	Utils u;
	if (argc == 1)
	{
		std::cerr << "Usage: network <command> <parameter>" << std::endl;
		return 1;
	}
	u.setCommand(argv[1]);
	for (int i = 2; i < argc; i++)
	{
		u.addParam(argv[i]);
	}
	u.excute();
	return 0;
}