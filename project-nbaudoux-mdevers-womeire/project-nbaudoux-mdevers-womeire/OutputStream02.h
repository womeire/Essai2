#include <string>
using namespace std;

#pragma once
class OutputStream02
{
public:
	OutputStream02();
	~OutputStream02();
	void create(string);
	void write(int32_t *, int);
	void close();
private:
	FILE * filePointer;
};
