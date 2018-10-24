/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

#include<iostream>
#include<map>
#include <boost/asio.hpp>
#include"../FileTransmit/socket.h"
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
	if (command == "sendmsg")
	{
		//boost shared ptr command para
		boost::asio::io_context io_context;
		Chat c(io_context);
		SendMsg(c);
	}
	else if (command == "")
	{

	}
	else
	{
		std::cerr << "command not exits" << std::endl;
	}
}

void Utils::SendMsg(Chat& c) const
{
	map<string,string> p;
	if (param.empty() || (param.size() & 1) != 0)
	{
		return;
	}

	for (vector<string>::const_iterator iter = param.begin(); iter != param.end(); iter++)
	{
		p[*iter++] = *(iter + 1);
	}

	//message content -c
	//endpoint port -p
	//endpoint host -h
	if (p.size() == 3)
	{
		if (p.find("-c") == p.end() || p.find("-h") == p.end() || p.find("-p") == p.end())
		{
			std::cerr << "Usage: network -c \"hello word\" -h host -p port" << std::endl;
			return;
		}
		c.sendMsg(p["-c"], p["-h"], stoi(p["-p"]));
	}
	else if (p.size() == 2)
	{
		if (p.count("-c") == 0 || p.count("-h") == 0)
		{
			std::cerr << "Usage: network -c \"hello word\" -h host -p port" << std::endl;
			return;
		}
		c.sendMsg(p["-c"], p["-h"]);
	}
}
