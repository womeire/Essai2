#include <string>
using namespace std;
namespace bi = boost::interprocess;

#pragma once
class OutputStream04Bis
{
public:
	OutputStream04Bis();
	~OutputStream04Bis();
	void create(string, string, int);
	void write(int);
	void close();
private:
	int fileSize_8;
	int bufferSize_8;
	int currentPos_8;
	char filepathInChar[_MAX_PATH];
	char filepathOutChar[_MAX_PATH];
	int32_t * memAddressIn;
	int32_t * memAddressOut;
};

