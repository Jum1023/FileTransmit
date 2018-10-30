/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

#include <sstream>
#include <vector>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include "transmit.h"

Transmit::Transmit(io_context& io_context)
	:recvsocket(io_context), sendsocket(io_context),
	acceptor(io_context, tcp::endpoint(tcp::v4(), 8192))
{
}

Transmit::~Transmit()
{
}

int Transmit::sendFile(const std::string& path, const std::string& ip, unsigned short port)
{
	//check if file exists
	fin.open(path, std::ios::binary);
	if (!fin)
	{
		std::cerr << "file not exit" << std::endl;
		return -1;
	}
	std::vector<std::string> pathsplit;
	boost::split(pathsplit, path, boost::is_any_of("/\\"));
	filename = pathsplit.back();
	fin.seekg(0, std::ios::end);
	filesize = fin.tellg();
	//fin.clear();
	fin.seekg(0, std::ios::beg);
	sendsocket.async_connect(tcp::endpoint(make_address(ip), 8192), boost::bind(&Transmit::handleConnect, this, placeholders::error));
	return 0;
}

void Transmit::recvFile()
{
	acceptor.async_accept(recvsocket, boost::bind(&Transmit::handleAccept, this, placeholders::error));
}

void Transmit::handleConnect(const boost::system::error_code& error)
{
	if (error)
	{
		return;
	}
	std::stringstream ss;
	ss << filesize << '|' << filename;
	ss >> sendbuf.data();
	ss.seekg(0, std::ios::end);
	async_write(sendsocket, buffer(sendbuf,ss.tellg()), boost::bind(&Transmit::handleRead, this, placeholders::error, placeholders::bytes_transferred));
}

void Transmit::handleAccept(const boost::system::error_code& error)
{
	if (error)
	{
		recvFile();
		return;
	}
	async_read(recvsocket, buffer(recvbuf), boost::bind(&Transmit::handleRead, this, placeholders::error, placeholders::bytes_transferred));
}

void Transmit::handleRead(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if (error)
	{
		return;
	}
	switch (recvbuf[0])
	{
	case 0://ready for accepting file
		//send();
		break;
	case 1://can't accept file
		break;
	case 2://file already exists
		break;
	default:
		break;
	}
}

void Transmit::handleWrite(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if (error)
	{
		return;
	}
	async_read(sendsocket, buffer(recvbuf), boost::bind(&Transmit::handleRead, this, placeholders::error, placeholders::bytes_transferred));
}

void Transmit::send(const boost::system::error_code& error)
{
	if (error)
	{
		return;
	}
	fin.read(sendbuf.data(), 1000);
	async_write(sendsocket, buffer(sendbuf), boost::bind(&Transmit::send, this, placeholders::error));
}