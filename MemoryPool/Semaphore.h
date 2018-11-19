#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

class Semaphore
{
public:
	Semaphore(int permits=0);
	~Semaphore();

	void wait(int permits=1);
	void post(int permits=1);
};

#endif