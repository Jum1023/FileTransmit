/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

#ifndef _UTILS_H_
#define _UTILS_H_

#include<string>
#include<vector>
#include"../InstantMessage/chat.h"
using namespace std;

class Utils
{
public:
	Utils();
	~Utils();
	inline void setCommand(const string& cmd) { command = cmd; }
	inline const string getCommand() const { return command; }
	inline void setParam(const vector<string>& pa) { param = pa; }
	inline const vector<string> getParam() const { return param; }
	void addParam(const string& pa);
	void excute() const;

private:
	//other implimentation
	void SendMsg(Chat& c) const;
private:
	string command;
	vector<string> param;
};

#endif