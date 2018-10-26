/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

#include<iostream>
#include<map>
#include <boost/asio.hpp>
#include"utilities.h"

Utils::Utils()
{
}

Utils::~Utils()
{
}

void Utils::addParam(const string & para)
{
	param.push_back(para);
}

void Utils::excute() const
{
	try
	{
		if (command == "sendmsg")
		{
			//boost shared ptr command para
			boost::asio::io_context io_context;
			Chat c(io_context);
			sendMsg(c);
		}
		else if (command == "recvmsg")
		{
			boost::asio::io_context io_context;
			Chat c(io_context);
			c.recvMsg();
			io_context.run();
		}
		else if (command == "recvfile")
		{
			boost::asio::io_context io_context;
			Transmit t(io_context);
			t.recvFile();
			io_context.run();
		}
		else if (command == "sendfile")
		{
			boost::asio::io_context io_context;
			Transmit t(io_context);
			sendFile(t);
			io_context.run();
		}
		else
		{
			std::cerr << "command not exits" << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void Utils::sendMsg(Chat& c) const
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
			std::cerr << "Usage: network sendmsg -c \"hello word\" -h host [-p port]" << std::endl;
			return;
		}
		c.sendMsg(p["-c"], p["-h"], stoi(p["-p"]));
	}
	else if (p.size() == 2)
	{
		if (p.count("-c") == 0 || p.count("-h") == 0)
		{
			std::cerr << "Usage: network sendmsg -c \"hello word\" -h host [-p port]" << std::endl;
			return;
		}
		c.sendMsg(p["-c"], p["-h"]);
	}
	else
	{
		std::cerr << "Usage: network sendmsg -c \"hello word\" -h host [-p port]" << std::endl;
	}
}

void Utils::sendFile(Transmit & t) const
{
	map<string, string> p;
	if (param.empty() || (param.size() & 1) == 1)
	{
		return;
	}

	for (vector<string>::const_iterator iter = param.begin(); iter != param.end(); iter++)
	{
		p[*iter++] = *(iter + 1);
	}

	//file content -f
	//endpoint port -p
	//endpoint host -h
	if (p.size() == 3)
	{
		if (p.find("-f") == p.end() || p.find("-h") == p.end() || p.find("-p") == p.end())
		{
			std::cerr << "Usage: network sendfile -f path -h host [-p port]" << std::endl;
			return;
		}
		t.sendFile(p["-f"], p["-h"], stoi(p["-p"]));
	}
	else if (p.size() == 2)
	{
		if (p.count("-f") == 0 || p.count("-h") == 0)
		{
			std::cerr << "Usage: network sendfile -f path -h host [-p port]" << std::endl;
			return;
		}
		t.sendFile(p["-f"], p["-h"]);
	}
	else
	{
		std::cerr << "Usage: network sendfile -f path -h host [-p port]" << std::endl;
	}
}
