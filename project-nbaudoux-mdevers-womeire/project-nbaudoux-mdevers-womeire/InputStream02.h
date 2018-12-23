#include <string>
using namespace std;

#pragma once
class InputStream02
{
public:
	InputStream02();
	~InputStream02();
	void open(string);
	int32_t read_next();
	bool end_of_stream();
	long fileSize_32;
private:
	FILE * filePointer;
	int32_t * buffer;
	int currentPos;
};

