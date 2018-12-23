#include "pch.h"

#pragma once
class External
{
	std::size_t _M, _N, _d, _nbStreams;
	std::string _stream;
public:
	External(std::string, std::size_t, std::size_t);
	void getFileSize();
	~External();
};

