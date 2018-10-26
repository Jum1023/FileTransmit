#ifndef _CHAT_H_
#define _CHAT_H_

#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../macro/export.h"
using namespace boost::asio;
using namespace boost::asio::ip;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

class SHAREOBJECT_EXPORT Chat : public udp::socket
{
public:
	Chat(io_context& io_context);
	~Chat();

	void sendMsg(const string& content, const string& ip, unsigned short port = 8191);
	void recvMsg();

private:
	void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);

private:
	udp::endpoint remotepoint;
	boost::array<char, 65536> recvbuf;
};

#endif