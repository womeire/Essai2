#include "OutputStream.h"

#pragma once
class OutputStream02 : public OutputStream
{
public:
	OutputStream02();
	~OutputStream02();
	void create(string, int);
	void write(int32_t*);
	void close();
private:
	FILE * filePointer;
	int32_t element;
};
