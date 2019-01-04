#include <string>
using namespace std;

#pragma once
class OutputStream
{
public:
	OutputStream();
	virtual ~OutputStream() = 0;
	virtual void create(string, int) = 0;
	virtual void write(int32_t*) = 0;
	virtual void close() = 0;
};