#include "chat.h"

Chat::Chat(boost::asio::io_context& io_context) :socket(io_context)
{
	socket.open(udp::v4());
}

Chat::~Chat()
{
}

void Chat::sendMsg(const std::string& content, const std::string& ip, unsigned short port)
{
	udp::endpoint receiver_endpoint;
	if (ip.empty())
	{

	}
	else
	{
	}
	receiver_endpoint.address(address::from_string(ip));
	receiver_endpoint.port(port);
	socket.send_to(boost::asio::buffer(content), receiver_endpoint);
}
