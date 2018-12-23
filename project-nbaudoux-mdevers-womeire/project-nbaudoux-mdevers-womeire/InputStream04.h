#include <string>
using namespace std;

#pragma once
class InputStream04
{
public:
	InputStream04();
	~InputStream04();
	void open(string, int); 
	int32_t read_next();
private:
	int bufferSize;
	int currentPos;
	int currentPosInBuffer;
	boost::interprocess::file_mapping m_file;
	boost::interprocess::mapped_region region;
	void LoadNextBuffer();
};

