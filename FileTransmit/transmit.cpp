#include "transmit.h"

Transmit::Transmit(io_context& io_context)
	:recvsocket(io_context, ip::tcp::endpoint(ip::tcp::v4(), 8192)),
	sendsocket(io_context)
{
}

Transmit::~Transmit()
{
}
