#include <string>
#include "pch.h"
using namespace std;

#pragma once
class InputStream
{
public:
	InputStream();
	virtual ~InputStream() = 0;
	virtual void open(string, size_t = 0) = 0;
	virtual int32_t* read_next() = 0;
	virtual bool end_of_stream() = 0;
	virtual void reset() = 0;
	virtual string getName() = 0;
};

