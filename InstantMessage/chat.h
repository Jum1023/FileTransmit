#ifndef _CHAT_H_
#define _CHAT_H_

#include "../macro/export.h"
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
using namespace boost::asio;

class SHAREOBJECT_EXPORT Chat
{
public:
	Chat(io_context& io_context);
	~Chat();

	void sendMsg(const std::string& content, const std::string& ip, unsigned short port = 8192);

private:
	ip::udp::socket socket;
	boost::array<char, 65536> recvbuf;
};

#endif