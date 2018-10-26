#ifndef _TRANSMIT_H_
#define _TRANSMIT_H_

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "../macro/export.h"
using namespace boost::asio;
using namespace boost::asio::ip;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

class SHAREOBJECT_EXPORT Transmit
{
public:
	Transmit(io_context& io_context);
	~Transmit();

	int sendFile(const string& path, const string& ip, unsigned short port = 8191);
	void recvFile();

	inline double getProgress() { return progress; }

private:
	void handleAccept(const boost::system::error_code& error);
	void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred);
private:
	tcp::socket sendsocket;
	tcp::socket recvsocket;

	tcp::endpoint remotepoint;
	tcp::acceptor acceptor;
	boost::array<char, 65536> recvbuf;

	double progress;
};

#endif