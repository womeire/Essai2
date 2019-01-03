#include "pch.h"
#include "InputStream04.h"

InputStream04::InputStream04()
{
	currentPos_8 = 0;
}


InputStream04::~InputStream04()
{
	// todo one flush or another needed here?
}

void InputStream04::open(string filepath, int bufSize)
{
	bufferSize_8 = bufSize * sizeof(int32_t); //bfSize is the size of the buffer in size int32_t, it's translated here to size 8b for easier use later (functions take 8b values)
	end_of_file = false;
	strcpy_s(filepathChar, filepath.c_str());

	struct stat buf;
	stat(filepathChar, &buf);
	fileSize_8 = buf.st_size;

	try
	{
		bi::file_mapping m_file(filepathChar, bi::read_only);
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

int InputStream04::read_next()
{
	returnPos_8 = bufferSize_8;
	
	currentPos_8 += bufferSize_8;
	if (currentPos_8 >= fileSize_8) {
		end_of_file = true;
		returnPos_8 = fileSize_8 - currentPos_8 + bufferSize_8;
	}
	return returnPos_8;
}

bool InputStream04::end_of_stream()
{
	return end_of_file;
}

void InputStream04::reset() {
	currentPos_8 = 0;
}