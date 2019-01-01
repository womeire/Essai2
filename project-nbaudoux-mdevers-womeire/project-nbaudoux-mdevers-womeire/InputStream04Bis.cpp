#include "pch.h"
#include "InputStream04Bis.h"

InputStream04Bis::InputStream04Bis()
{
	currentPos = 0;
}


InputStream04Bis::~InputStream04Bis()
{
	// todo one flush or another needed here?
}

void InputStream04Bis::open(string filepath, int bfSize)
{
	bufferSize = bfSize;

	strcpy_s(filepathChar, filepath.c_str());
	buffer.resize(bufferSize);

	struct stat buf;
	stat(filepathChar, &buf);
	fileSize_32 = buf.st_size / sizeof(int32_t);

	try
	{
		bi::file_mapping m_file(filepathChar, bi::read_only);
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

int32_t* InputStream04Bis::read_next()
{
	bi::file_mapping m_file(filepathChar, bi::read_only);
	bi::mapped_region region(m_file, bi::read_only, currentPos * sizeof(int32_t), bufferSize * sizeof(int32_t));

	currentPos += bufferSize; // todo still need to check eof

	memAddress = (int32_t*)region.get_address();
	int32_t* ElementsP;
	ElementsP = (int32_t*)malloc(sizeof(int32_t)*bufferSize);
	memcpy(ElementsP, memAddress, sizeof(int32_t)*bufferSize);

	return ElementsP;
}

bool InputStream04Bis::end_of_stream()
{
	return currentPos >= fileSize_32;
}