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
	int32_t* read_next();
	bool end_of_stream();
private:
	int fileSize_32;
	int bufferSize;
	int currentPos;
	int currentPosInBuffer;
	char filepathChar[_MAX_PATH];
	int32_t * memAddress;
	std::vector<int32_t> buffer;
	boost::interprocess::file_mapping * my_mapped_file;
	boost::interprocess::mapped_region * my_mapped_region;
	void LoadNextBuffer();
};

