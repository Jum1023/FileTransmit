#include "Semaphore.h"

Semaphore::Semaphore(int permits) :resource(permits)
{
}

Semaphore::~Semaphore()
{
}

void Semaphore::wait(int permits)
{
	unique_lock<mutex> lock(m);
	while (permits > resource)
	{
		cv.wait(lock);
	}
	resource -= permits;
	cv.notify_one();
}

void Semaphore::post(int permits)
{
	unique_lock<mutex> lock(m);
	resource += permits;
	cv.notify_one();
}