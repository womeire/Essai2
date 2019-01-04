#include "InputStream.h"

#pragma once
class InputStream02 : public InputStream
{
public:
	InputStream02();
	~InputStream02();
	void open(string, size_t);
	int32_t* read_next();
	bool end_of_stream();
	void reset();
	string getName();
private:
	FILE * filePointer;
	int32_t * elementP;
};

