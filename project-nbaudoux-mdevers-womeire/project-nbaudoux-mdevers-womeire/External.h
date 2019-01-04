#include "pch.h"

#pragma once
class External
{
	const int bufferSize = 18;
	std::size_t _M, _N, _d, _nbStreams, _id;
	std::string _stream, _streamID;
	std::queue<std::string> _queue_of_streams;
public:
	External(std::string, std::size_t, std::size_t);
	void nextID();
	std::string getAPath();
	void getFileSize();
	void createStreams();
	void merge();
	void showRes();
	~External();
};

