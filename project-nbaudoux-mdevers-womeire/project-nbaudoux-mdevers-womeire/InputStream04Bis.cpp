#include "pch.h"
#include "InputStream04Bis.h"

InputStream04Bis::InputStream04Bis()
{
	currentPos = -1;
	currentPosInBuffer = 0;
}


InputStream04Bis::~InputStream04Bis()
{
	printf("File closed.\n"); // todo delete functions needed here?
}

bi::file_mapping InputStream04Bis::open(string filepath, int bufSize) // have third argument for size?
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
		return m_file; // does not work.... "cannot reference deleted function"
		//my_mapped_file = &m_file;
		//auto test = my_mapped_file->get_mode();
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

int32_t InputStream04Bis::read_next(bi::file_mapping m_file)
{
	bi::mapped_region region(m_file, bi::read_only, currentPos * 4, bufferSize * 4);

	memAddress = (int32_t*)region.get_address();

	return *memAddress;
}
//
//int32_t InputStream04Bis::read_next()
//{
//	currentPos++;
//
//	if (currentPosInBuffer % bufferSize == 0) {
//		if (end_of_stream())
//			return NULL; // Todo not good as -1 can be a correct value for this function... but everything from min to max is...
//		LoadNextBuffer();
//		currentPosInBuffer = 0;
//	}
//
//	return buffer[currentPosInBuffer++]; //equivalent to "int32_t val = buffer[currentPosInBuffer]; ++currentPosInBuffer;return val;"
//}

bool InputStream04Bis::end_of_stream()
{
	return currentPos >= fileSize_32;
}

void InputStream04Bis::LoadNextBuffer()
{

	try
	{
		Benchmarking testChrono;

		testChrono.startTest();

		//Create a file mapping
		bi::file_mapping m_file(filepathChar, bi::read_only);

		//Map the whole file with read-only permissions in this process
		bi::mapped_region region(m_file, bi::read_only, currentPos * 4, bufferSize * 4);
		//my_mapped_region = &region;
		//auto test = region.get_page_size(); // = 65536

		//memAddress = (int32_t*)my_mapped_region->get_address();
		memAddress = (int32_t*)region.get_address();

		testChrono.stopTest();
		auto test = testChrono.getLastPerformance();

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