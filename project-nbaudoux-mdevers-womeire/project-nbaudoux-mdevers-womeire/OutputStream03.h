#include <string>
using namespace std;

#pragma once
class OutputStream03
{
public:
	OutputStream03();
	~OutputStream03();
	void create(string, int);
	void write(int32_t * buffer);
	void write(int32_t * buffer, int buffSize);
	void close();
private:
	FILE * filePointer;
	int bufferSize;
};
