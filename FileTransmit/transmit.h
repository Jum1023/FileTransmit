#ifndef _TRANSMIT_H_
#define _TRANSMIT_H_

#include "../macro/export.h"

class SHAREOBJECT_EXPORT Transmit
{
public:
	Transmit();
	~Transmit();

	void sendFile();
	void sendDir();
	void recvFile();
};

#endif