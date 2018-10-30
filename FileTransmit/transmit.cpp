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
	async_write(sendsocket, buffer(sendbuf,ss.tellg()), boost::bind(&Transmit::handleClientRead, this, placeholders::error, placeholders::bytes_transferred));
}

void Transmit::handleAccept(const boost::system::error_code& error)
{
	if (error)
	{
		recvFile();
		return;
	}
	async_read(recvsocket, buffer(recvbuf), boost::bind(&Transmit::handleServerRead, this, placeholders::error, placeholders::bytes_transferred));
}

void Transmit::handleClientRead(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if (error)
	{
		return;
	}
	switch (recvbuf[0])
	{
	case 0://ready for accepting file
		send(boost::system::error_code());
		break;
	case 1://can't accept file
		break;
	case 2://file already exists
		break;
	default:
		break;
	}
}

void Transmit::handleServerRead(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if (error)
	{
		return;
	}
	recvbuf[bytes_transferred] = '\0';
	std::string s(recvbuf.data());
	std::vector<std::string> strsplit;
	boost::split(strsplit, s, boost::is_any_of("|"));
	if (strsplit.size() != 2)
	{
		return;
	}
	filesize = stoi(strsplit[0]);
	filename = strsplit[1];

	//if file exists
	fout.open(filename,std::ios::binary||std::ios::_Noreplace);
	if (fout)
	{
		sendbuf[0] = 0;
		async_write(recvsocket, buffer(sendbuf, 1), boost::bind(&Transmit::recv, this, placeholders::error, placeholders::bytes_transferred));
	}
	else
	{
		sendbuf[0] = 1;
		async_write(recvsocket, buffer(sendbuf, 1), boost::bind(&Transmit::recv, this, placeholders::error, placeholders::bytes_transferred));
	}
}

void Transmit::send(const boost::system::error_code& error)
{
	if (error)
	{
		return;
	}
	fin.read(sendbuf.data(), 1000);
	std::streamsize s = fin.gcount();
	if (s == 0)//send over
	{
		sendbuf[0] = 4;
		async_write(sendsocket, buffer(sendbuf, 1), boost::bind(&Transmit::sendover, this, placeholders::error));
	}
	else
	{
		async_write(sendsocket, buffer(sendbuf, s), boost::bind(&Transmit::send, this, placeholders::error));
	}
}

void Transmit::sendover(const boost::system::error_code & error)
{
	fin.close();
}

void Transmit::recv(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if (error)
	{
		return;
	}
	if (bytes_transferred == 1 && recvbuf[0] == 4)//transfer finished
	{
		fout.close();
	}
	else
	{
		fout.write(recvbuf.data(), bytes_transferred);
		async_read(recvsocket, buffer(recvbuf), boost::bind(&Transmit::recv, this, placeholders::error, placeholders::bytes_transferred));
	}
}