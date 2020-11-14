#ifndef _TCP_HEADER_H_
#define _TCP_HEADER_H_

/*
copy from linux kernel
include/uapi/linux/tcp.h
*/

struct tcphdr
{
	unsigned short source;
	unsigned short dest;
	unsigned int seq;
	unsigned int ack_seq;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	unsigned short res1 : 4,
		doff : 4,
		fin : 1,
		syn : 1,
		rst : 1,
		psh : 1,
		ack : 1,
		urg : 1,
		ece : 1,
		cwr : 1;
#elif defined(__BIG_ENDIAN_BITFIELD)
	unsigned short doff : 4,
		res1 : 4,
		cwr : 1,
		ece : 1,
		urg : 1,
		ack : 1,
		psh : 1,
		rst : 1,
		syn : 1,
		fin : 1;
#else
#endif
	unsigned short window;
	unsigned short check;
	unsigned short urg_ptr;
};

#endif