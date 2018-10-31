/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

#ifndef _TRANSMIT_H_
#define _TRANSMIT_H_

#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "../macro/export.h"
using namespace boost::asio;
using namespace boost::asio::ip;

class SHAREOBJECT_EXPORT Transmit
{
public:
	Transmit(io_context& io_context);
	~Transmit();

	int sendFile(const std::string& path, const std::string& ip, unsigned short port = 8191);
	void recvFile();

	inline double getProgress() { return progress; }

private:
	void handleConnect(const boost::system::error_code& error);
	void handleAccept(const boost::system::error_code& error);
	void handleClientRead(const boost::system::error_code& error);
	void handleServerRead(const boost::system::error_code& error, std::size_t bytes_transferred);
	void handleServerWrite(const boost::system::error_code& error);
	void send(const boost::system::error_code& error);
	void sendover(const boost::system::error_code& error);
	void recv(const boost::system::error_code& error,std::size_t bytes_transferred);

private:
	tcp::socket sendsocket;
	tcp::socket recvsocket;

	tcp::endpoint remotepoint;
	tcp::acceptor acceptor;
	boost::array<char, 65536> recvbuf;
	boost::array<char, 1024> sendbuf;

	std::ifstream fin;
	std::streamsize filesize;
	std::string filename;

	std::ofstream fout;
	double progress;
};

#endif