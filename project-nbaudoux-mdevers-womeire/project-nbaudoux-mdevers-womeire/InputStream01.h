#include <string>
using namespace std;

#pragma once
class InputStream01
{
public:
	InputStream01();
	~InputStream01();
	void open(string);
	int32_t read_next();
	bool end_of_stream();
	void reset();
private:
	int fileHandle;
};

