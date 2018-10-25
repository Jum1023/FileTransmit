#ifndef _TRANSMIT_H_
#define _TRANSMIT_H_

#include <boost/asio.hpp>
#include "../macro/export.h"
using namespace boost::asio;
using std::string;

class SHAREOBJECT_EXPORT Transmit
{
public:
	Transmit(io_context& io_context);
	~Transmit();

	void sendFile(const string& path);
	void recvFile();

private:
	ip::tcp::socket sendsocket;
	ip::tcp::socket recvsocket;
};

#endif