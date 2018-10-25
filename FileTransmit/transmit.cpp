#include <fstream>
#include <boost/bind.hpp>
#include "transmit.h"
using std::ifstream;

Transmit::Transmit(io_context& io_context)
	:recvsocket(io_context), sendsocket(io_context),
	acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), 8192))
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
	
	connect(sendsocket, resolver.resolve(ip::tcp::v4(), 8192));

}

void Transmit::recvFile()
{
	acceptor.async_accept(recvsocket, boost::bind(&Transmit::handleAccept, this, placeholders::error));
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
{}