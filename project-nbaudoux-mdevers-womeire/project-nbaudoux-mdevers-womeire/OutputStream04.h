#include <string>
using namespace std;
namespace bi = boost::interprocess;

#pragma once
class OutputStream04
{
public:
	OutputStream04();
	~OutputStream04();
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

