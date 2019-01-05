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
	void open(string, size_t);
	int32_t* read_next();
	int8_t* read_next_8();
	bool end_of_stream();
	void reset();
	string getName();
private:
	size_t fileSize_8;
	size_t bufferSize_8;
	size_t currentPos_8;
	size_t range_8;
	bool end_of_file;
	char filepathChar[_MAX_PATH];
	int32_t* elements;
	int8_t* elements_8;
};

