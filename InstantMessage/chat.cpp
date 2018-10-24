#include "chat.h"

Chat::Chat(io_context& io_context) :socket(io_context)
{
	socket.open(ip::udp::v4());
	io_context.run();
}

Chat::~Chat()
{
}

void Chat::sendMsg(const std::string& content, const std::string& ip, unsigned short port)
{
	if (ip.empty())
	{
		socket.set_option(socket_base::broadcast(true));
		socket.send_to(buffer(content), ip::udp::endpoint(ip::address_v4::broadcast(), port));
	}
	else
	{
		socket.send_to(buffer(content), ip::udp::endpoint(ip::address::from_string(ip), port));
	}
}
