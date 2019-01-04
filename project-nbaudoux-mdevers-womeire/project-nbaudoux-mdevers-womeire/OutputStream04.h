#include <string>
#include "OutputStream.h"
using namespace std;
namespace bi = boost::interprocess;

#pragma once
class OutputStream04 : public OutputStream
{
public:
	OutputStream04();
	~OutputStream04();
	void create(string, int);
	void write(int32_t*);
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

