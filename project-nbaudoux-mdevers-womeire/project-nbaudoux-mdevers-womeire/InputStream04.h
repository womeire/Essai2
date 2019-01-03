#include <string>
#include "Benchmarking.h"
using namespace std;
namespace bi = boost::interprocess;

#pragma once
class InputStream04
{
public:
	InputStream04();
	~InputStream04();
	void open(string, int);
	int read_next();
	bool end_of_stream();
	void reset();
private:
	int fileSize_8;
	int bufferSize_8;
	int currentPos_8;
	int returnPos_8;
	bool end_of_file;
	char filepathChar[_MAX_PATH];
	int32_t * memAddress;
};

