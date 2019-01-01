#include <string>
using namespace std;

#pragma once
class InputStream03
{
public:
	InputStream03();
	~InputStream03();
	void open(string, int);
	int32_t* read_next();
	bool end_of_stream();
private:
	int bufferSize;
	FILE * filePointer;
	int32_t * buffer;
};

