#include <string>
using namespace std;

#pragma once
class OutputStream01
{
public:
	OutputStream01();
	~OutputStream01();
	void create(string);
	void write(int32_t);
	void close();
private:
	int fileHandle;
};

