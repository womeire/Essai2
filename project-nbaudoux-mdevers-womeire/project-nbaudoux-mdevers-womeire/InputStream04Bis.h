#include <string>
#include "Benchmarking.h"
using namespace std;
namespace bi = boost::interprocess;

#pragma once
class InputStream04Bis
{
public:
	InputStream04Bis();
	~InputStream04Bis();
	void open(string, int);
	int read_next();
	bool end_of_stream();
private:
	int fileSize_8;
	int bufferSize_8;
	int currentPos_8;
	char filepathChar[_MAX_PATH];
	int32_t * memAddress;
};

