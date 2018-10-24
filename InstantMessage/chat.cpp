#include <boost/bind.hpp>
#include "chat.h"

Chat::Chat(io_context& io_context) :socket(io_context, ip::udp::endpoint(ip::udp::v4(), 8191))
{
	//socket.open(ip::udp::v4());
	socket.set_option(ip::udp::socket::reuse_address(true));
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
		socket.send_to(buffer(content), ip::udp::endpoint(ip::make_address(ip), port));
	}
}

void Chat::recvMsg()
{
	socket.async_receive_from(buffer(recvbuf), remotepoint, boost::bind(&Chat::handleReceive, this, placeholders::error, placeholders::bytes_transferred));
}

void Chat::handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if (!error)
	{
		std::cout << remotepoint.address() << ":" << remotepoint.port() << "\t";
		std::cout.write(recvbuf.data(), bytes_transferred) << std::endl;
		//std::cout << remotepoint.protocol() << std::endl;
	}
	recvMsg();
}
