#include "pch.h"
#include "InputStream04.h"

InputStream04::InputStream04()
{
	currentPos = -1;
	currentPosInBuffer = 0;
}


InputStream04::~InputStream04()
{
	printf("File closed.\n"); // todo delete functions needed here?
}

void InputStream04::open(string filepath, int bufSize) // have third argument for size?
{
	bufferSize = bufSize;

	strcpy_s(filepathChar, filepath.c_str());
	buffer.resize(bufferSize);

	struct stat buf;
	stat(filepathChar, &buf);
	fileSize_32 = buf.st_size / 4; // divided by 4 because ftell returns the number of 8 bit units

	try
	{
		//Create a file mapping
		bi::file_mapping m_file(filepathChar, bi::read_only);
		//my_mapped_file = &m_file;
		//auto test = my_mapped_file->get_mode();
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

int32_t * InputStream04::read_next() // Should be "readnextbuffer"
{
	try
	{
		if (currentPos < 0)
			currentPos = 0;

		//Create a file mapping
		bi::file_mapping m_file(filepathChar, bi::read_only);

		//Map the whole file with read-only permissions in this process
		bi::mapped_region region(m_file, bi::read_only, currentPos * 4, bufferSize * 4);

		int32_t* res = (int32_t*)region.get_address();

		
		currentPos += bufferSize;

		int32_t test0 = *res;
		int32_t test1 = *(res+1);
		int32_t test2 = *(res+2);

		return res; // Id say this is not safe...
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

bool InputStream04::end_of_stream()
{
	//return (currentPos / 4) >= fileSize_32; // todo check if div by 4 is right
	return currentPos >= fileSize_32;
}

void InputStream04::LoadNextBuffer()
{

	try
	{
		//Create a file mapping
		bi::file_mapping m_file(filepathChar, bi::read_only);

		//Map the whole file with read-only permissions in this process
		bi::mapped_region region(m_file, bi::read_only, currentPos*4, bufferSize*4);
		
		memAddress = (int32_t*) region.get_address();

		for (int32_t i = 0; i < bufferSize; i++)
		{
			buffer[i] = *(memAddress + i);
		}
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}


}