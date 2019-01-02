#include "pch.h"
#include "InputStream04Bis.h"

InputStream04Bis::InputStream04Bis()
{
	currentPos_8 = 0;
}


InputStream04Bis::~InputStream04Bis()
{
	// todo one flush or another needed here?
}

void InputStream04Bis::open(string filepath, int bufSize)
{
	bufferSize_8 = bufSize * sizeof(int32_t); //bfSize is the size of the buffer in size int32_t, it's translated here to size 8b for easier use later (functions take 8b values)

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

int InputStream04Bis::read_next()
{
	int startingPos = currentPos_8;

	currentPos_8 += bufferSize_8; // todo something to take into account "incomplete" last buffer with fileSize_8 & currentPos_8

	return startingPos;
}

bool InputStream04Bis::end_of_stream()
{
	return currentPos_8 >= fileSize_8;
}