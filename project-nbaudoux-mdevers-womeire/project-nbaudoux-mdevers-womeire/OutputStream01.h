#include "OutputStream.h"

#pragma once
class OutputStream01 : public OutputStream
{
public:
	OutputStream01();
	~OutputStream01();
	void create(string, size_t = 0, size_t = 0);
	void write(int32_t*);
	void close();
private:
	int fileHandle;
};

