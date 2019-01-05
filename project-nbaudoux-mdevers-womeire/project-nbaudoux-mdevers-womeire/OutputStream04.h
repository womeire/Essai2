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
	void create(string, size_t, size_t);
	void write(int32_t*);
	void close();
private:
	size_t fileSize_8;
	size_t bufferSize_8;
	size_t range_8;
	size_t currentPos_8;
	char filepathChar[_MAX_PATH];
	int32_t * memAddress;
};

