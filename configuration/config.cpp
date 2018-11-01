/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

#include "config.h"

Config::Config()
{
}

Config::~Config()
{
}

void Config::load(const std::string& filename)
{
	read_xml(filename, tree, xml_parser::no_comments || xml_parser::trim_whitespace);
	std::string f = tree.get<std::string>("debug.filename");
	int t=tree.get("debug.level", 0);
}

void Config::save(const std::string& filename)
{
	write_xml(filename, tree);
}

std::string Config::getText(const std::string& path)
{
	return std::string();
}

std::string Config::getAttr(const std::string& path)
{
	return std::string();
}
