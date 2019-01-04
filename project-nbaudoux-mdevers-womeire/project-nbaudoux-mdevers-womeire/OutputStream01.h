#include "OutputStream.h"

#pragma once
class OutputStream01 : public OutputStream
{
public:
	OutputStream01();
	~OutputStream01();
	void create(string, size_t);
	void write(int32_t*);
	void close();
private:
	int fileHandle;
};

