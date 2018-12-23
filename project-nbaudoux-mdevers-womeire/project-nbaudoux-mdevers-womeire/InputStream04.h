#include <string>
using namespace std;

#pragma once
class InputStream04
{
public:
	InputStream04();
	~InputStream04();
	void open(string, int, int); 
	int32_t read_next();
	bool end_of_stream();
private:
	int fileSize;
	int bufferSize;
	int currentPos;
	int currentPosInBuffer;
	char filepathChar[_MAX_PATH];
	int32_t * memAddress;
	std::vector<int32_t> buffer;
	void LoadNextBuffer();
};

