/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

/*
this Semaphore class was implemented based on condition variable and mutex
*/
#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
using namespace boost;

class Semaphore
{
public:
	Semaphore(int permits=0);
	~Semaphore();

	void wait(int permits=1);
	void post(int permits=1);

private:
	int resource;
	condition_variable cv;
	mutex m;
};

#endif