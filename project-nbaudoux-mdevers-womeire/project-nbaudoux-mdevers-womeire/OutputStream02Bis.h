#include <string>
using namespace std;

#pragma once
class OutputStream02Bis
{
public:
	OutputStream02Bis();
	~OutputStream02Bis();
	void create(string, int);
	void write(int32_t *);
	void close();
private:
	int bufferSize;
	FILE * filePointer;
};
