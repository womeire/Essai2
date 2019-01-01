#include <string>
using namespace std;
namespace bi = boost::interprocess;

#pragma once
class OutputStream04Bis
{
public:
	OutputStream04Bis();
	~OutputStream04Bis();
	void create(string, int, int);
	void write(int32_t *);
	void close();
private:
	int fileSize_32;
	int bufferSize;
	int currentPos;
	int currentPosInBuffer;
	char filepathChar[_MAX_PATH];
	int32_t * memAddress;
	std::vector<int32_t> buffer;
};

