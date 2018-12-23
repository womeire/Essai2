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

void InputStream04::open(string filepath, int bufSize, int fSize) // have third argument for size?
{
	bufferSize = bufSize;
	fileSize = fSize;

	strcpy_s(filepathChar, filepath.c_str());
	buffer.resize(bufferSize);

	//boost::interprocess::file_mapping::remove(filepathChar); // check if it's done the way it should

	try
	{
		//Create a file mapping
		boost::interprocess::file_mapping m_file(filepathChar, boost::interprocess::read_only);

		//m_file2 = m_file;
		auto test = m_file.get_mode();
	}
	catch (const boost::interprocess::interprocess_exception e)
	{
		e;
	}

}

int32_t InputStream04::read_next()
{
	currentPos++;

	if (currentPosInBuffer % bufferSize == 0) {
		if (end_of_stream())
			return NULL; // Todo not good as -1 can be a correct value for this function... but everything from min to max is...
		LoadNextBuffer();
		currentPosInBuffer = 0;
	}

	return buffer[currentPosInBuffer++]; //equivalent to "int32_t val = buffer[currentPosInBuffer]; ++currentPosInBuffer;return val;"
}

bool InputStream04::end_of_stream()
{
	return currentPos >= fileSize;
}

void InputStream04::LoadNextBuffer()
{

	try
	{
		//Create a file mapping
		boost::interprocess::file_mapping m_file(filepathChar, boost::interprocess::read_only);

		//Map the whole file with read-write permissions in this process
		boost::interprocess::mapped_region region(m_file, boost::interprocess::read_only, currentPos*4, bufferSize*4); // bufferSize is converted to size_t which is 4 bytes

		//auto test = region.get_page_size(); // = 65536

		memAddress = (int32_t*)region.get_address();
		for (int32_t i = 0; i < bufferSize; i++)
		{
			buffer[i] = *(memAddress + i);
		}
	}
	catch (const boost::interprocess::interprocess_exception e)
	{
		e;
	}


}