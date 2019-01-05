#include "OutputStream.h"

#pragma once
class OutputStream02 : public OutputStream
{
public:
	OutputStream02();
	~OutputStream02();
	void create(string, size_t = 0, size_t = 0);
	void write(int32_t*);
	void close();
private:
	FILE * filePointer;
	int32_t element;
};
