#ifndef _UDP_HEADER_H_
#define _UDP_HEADER_H_

/*
copy from linux kernel
include/uapi/linux/udp.h
*/

struct udphdr
{
	unsigned short source;
	unsigned short dest;
	unsigned short len;
	unsigned short check;
};

#endif