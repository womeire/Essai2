#include "pch.h"
#include "OutputStream04.h"

OutputStream04::OutputStream04()
{
	currentPos_8 = 0;
}


OutputStream04::~OutputStream04()
{
	close();
}

void OutputStream04::create(string filepath, size_t bufSize, size_t fileSize)
{
	bufferSize_8 = bufSize * sizeof(int32_t); //bufSize is the size of the buffer in size int32_t, it's translated here to size 8b for easier use later (functions take 8b values)
	bufferSize_32 = bufSize;
	strcpy_s(filepathChar, filepath.c_str());
	fileSize_8 = fileSize * sizeof(int32_t);

	try
	{
		//Create a file
		bi::file_mapping::remove(filepathChar);

		std::filebuf fbuf;
		fbuf.open(filepathChar, std::ios_base::in | std::ios_base::out
			| std::ios_base::trunc | std::ios_base::binary);
		//Set the size
		fbuf.pubseekoff(fileSize_8 - 1, std::ios_base::beg);
		fbuf.sputc(0);

		bi::file_mapping m_file(filepathChar, bi::read_write);
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

void OutputStream04::write(int32_t* elements)
{
	//Create a file mapping
	bi::file_mapping m_file(filepathChar, bi::read_write);

	//Map the whole file with read-only permissions in this process
	bi::mapped_region region(m_file, bi::read_write, currentPos_8, bufferSize_8);

	memAddress = (int32_t*)region.get_address();
	memcpy(memAddress, elements, bufferSize_32);

	currentPos_8 += bufferSize_8;
}

void OutputStream04::close()
{
	currentPos_8 = 0;
}
