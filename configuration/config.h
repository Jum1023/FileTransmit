/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "../macro/export.h"
using namespace boost::property_tree;

class SHAREDOBJECT_EXPORT Config
{
public:
	Config();
	~Config();
	void load(const std::string& filename);
	void save(const std::string& filename);

	std::string getText(const std::string& path);
	std::string getAttr(const std::string& path);

private:
	ptree tree;
};

#endif