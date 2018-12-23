#include "pch.h"
#include "InputStream04.h"

InputStream04::InputStream04()
{
	currentPos = -1;
	currentPosInBuffer = 0;
}


InputStream04::~InputStream04()
{
	printf("File closed.\n");
	//boost::interprocess::file_mapping::remove(filepath);
}

void InputStream04::open(string filepath, int bufSize)
{
	bufferSize = bufSize;
	char filepathChar[_MAX_PATH];
	strcpy_s(filepathChar, filepath.c_str());

	boost::interprocess::file_mapping::remove(filepathChar); // check if it's done the way it should
	
	//Create a file mapping
	boost::interprocess::file_mapping m_file(filepathChar, boost::interprocess::read_only);

}

int32_t InputStream04::read_next()
{
	if (currentPosInBuffer % bufferSize == 0) {
		LoadNextBuffer();
		currentPosInBuffer = 0;
	}

	void * address = region.get_address();

	return 0;
}

void InputStream04::LoadNextBuffer()
{
	currentPos++;
	//Map the whole file with read-write permissions in this process
	boost::interprocess::mapped_region region(m_file, boost::interprocess::read_only, currentPos , bufferSize);
}