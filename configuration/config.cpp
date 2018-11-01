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
	read_xml(filename, tree, xml_parser::trim_whitespace);
}

void Config::save(const std::string& filename)
{
	write_xml(filename, tree);
}

std::string Config::getText(const std::string& path)
{
	std::string tmp= tree.get<std::string>(path);
	return tree.get<std::string>(path);
}

std::string Config::getAttr(const std::string& path)
{
	return std::string();
}
