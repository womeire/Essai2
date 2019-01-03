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
private:
	FILE * filePointer;
	int32_t * elementP;
};

