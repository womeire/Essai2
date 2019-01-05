#include "OutputStream.h"

#pragma once
class OutputStream03 : public OutputStream
{
public:
	OutputStream03();
	~OutputStream03();
	void create(string, size_t, size_t = 0);
	void write(int32_t *);
	void close();
private:
	int bufferSize;
	FILE * filePointer;
};
