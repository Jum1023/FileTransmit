/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

#include<iostream>
#include"..\FileTransmit\socket.h"
#include"utilities.h"

Utils::Utils()
{
}

Utils::~Utils()
{
}

void Utils::addParam(const string & pa)
{
	param.push_back(pa);
}

void Utils::excute() const
{
	if (command == "")
	{
		//boost shared ptr command para
		//SendFile(Share_ptr<FileTransmit>(new FileTransmit()));
	}
	else if (command == "")
	{

	}
	else
	{
		std::cerr << "command not exits" << std::endl;
	}
}
