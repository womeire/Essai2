#include "pch.h"
#include "External.h"

External::External(std::string stream, std::size_t M, std::size_t d): _stream(stream), _M(M), _d(d)
{
	getFileSize();
	_nbStreams = ceil(_N / _M);
}

void External::getFileSize()
{
	std::ifstream in(_stream, std::ifstream::ate | std::ifstream::binary);
	_N = in.tellg();
}

External::~External()
{
}
