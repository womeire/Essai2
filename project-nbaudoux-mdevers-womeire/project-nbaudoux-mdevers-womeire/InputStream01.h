#include "InputStream.h"

#pragma once
class InputStream01 : public InputStream
{
public:
	InputStream01();
	~InputStream01();
	void open(string, size_t = 0);
	int32_t* read_next();
	bool end_of_stream();
	void reset();
	string getName();
private:
	int fileHandle;
	int32_t element[1];
};

