#include <string>
using namespace std;

#pragma once
class OutputStream
{
public:
	OutputStream();
	virtual ~OutputStream() = 0;
	virtual void create(string, size_t) = 0;
	virtual void write(int32_t*) = 0;
	virtual void close() = 0;
};