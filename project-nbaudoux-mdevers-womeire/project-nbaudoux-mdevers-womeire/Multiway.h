#include "pch.h"
#include "InputStream03.h"

#pragma once
class Multiway
{
	//By default std::priority_queue sorts according a decreasing order, std::greater<int> inverses it.
	std::priority_queue<std::vector<int>, std::vector<std::vector<int> >, std::greater<std::vector<int> > > _queue;
	std::vector<std::vector<int> > _content;
	std::vector<int>_sorted;
	std::vector<std::string> _streams;
public:
	Multiway(std::vector<std::string>);
	void read(std::size_t);
	void merge();
	void showRes();
	~Multiway();
};

