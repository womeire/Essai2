#include <string>
using namespace std;

#pragma once
class InputStream02Bis
{
public:
	InputStream02Bis();
	~InputStream02Bis();
	void open(string, int);
	int32_t* read_next();
	bool end_of_stream();
private:
	int bufferSize;
	FILE * filePointer;
	int32_t * buffer;
};

