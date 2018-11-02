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
	read_xml(filename, tree, xml_parser::no_comments | xml_parser::trim_whitespace);
}

void Config::save(const std::string& filename)
{
	write_xml(filename, tree);
}

std::string Config::getText(const std::string& path)
{
	return tree.get<std::string>(path);
}

std::string Config::getAttr(const std::string& path, const std::string& attr)
{
	const ptree& childtree = tree.get_child(path, ptree());
	for (ptree::const_iterator iter = childtree.begin(); iter != childtree.end();iter++)
	{
		if (iter->first == "<xmlattr>")
		{
			return iter->second.get<std::string>(attr);
		}
	}
	return std::string();
}
