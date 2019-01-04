#include <string>
#include "Benchmarking.h"
#include "InputStream.h"
using namespace std;
namespace bi = boost::interprocess;

#pragma once
class InputStream04 : public InputStream
{
public:
	InputStream04();
	~InputStream04();
	void open(string, int);
	int32_t* read_next();
	bool end_of_stream();
	void reset();
	string getName();
private:
	int32_t fileSize_8;
	int32_t bufferSize_8;
	int32_t currentPos_8;
	int32_t* returnPos;
	bool end_of_file;
	char filepathChar[_MAX_PATH];
	int32_t* memAddress;
};

