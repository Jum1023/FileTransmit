/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

#include <fstream>
#include <boost/bind.hpp>
#include "transmit.h"
using std::ifstream;

Transmit::Transmit(io_context& io_context)
	:recvsocket(io_context), sendsocket(io_context),
	acceptor(io_context, tcp::endpoint(tcp::v4(), 8192))
{
}

Transmit::~Transmit()
{
}

int Transmit::sendFile(const string & path, const string& ip, unsigned short port)
{
	//check if file exists
	ifstream fin(path, std::ios::binary);
	if (fin)
	{
		cerr << "file not exit" << endl;
		return -1;
	}
	
	sendsocket.async_connect(tcp::endpoint(make_address(ip), 8192), boost::bind(&Transmit::handleConnect, this, placeholders::error));
	return 0;
}

void Transmit::recvFile()
{
	acceptor.async_accept(recvsocket, boost::bind(&Transmit::handleAccept, this, placeholders::error));
}

void Transmit::handleConnect(const boost::system::error_code & error)
{
	cout << "connect suceess" << endl;
	async_write(sendsocket, buffer("这是传输内容"), boost::bind(&Transmit::handleWrite, this, placeholders::error, placeholders::bytes_transferred));
}

void Transmit::handleAccept(const boost::system::error_code & error)
{
	if (error)
	{
		recvFile();
	}
	async_read(recvsocket, buffer(recvbuf), boost::bind(&Transmit::handleRead, this, placeholders::error, placeholders::bytes_transferred));
}

void Transmit::handleRead(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	cout << "read success" << endl;
	cout.write(recvbuf.data(), bytes_transferred) << endl;
	//async_write(recvsocket, buffer("accepted"), boost::bind(&Transmit::handleWrite, this, placeholders::error, placeholders::bytes_transferred));
}

void Transmit::handleWrite(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	cout << "write success" << endl;
	//async_write(recvsocket, buffer("accepted"), boost::bind(&Transmit::handleWrite, this, placeholders::error, placeholders::bytes_transferred));
}