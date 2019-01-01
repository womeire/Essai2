#include "pch.h"
#include "OutputStream04Bis.h"

OutputStream04Bis::OutputStream04Bis()
{
}


OutputStream04Bis::~OutputStream04Bis()
{
	close();
}

void OutputStream04Bis::create(string filepath, int bufSize, int fS_32)
{
	bufferSize = bufSize;

	strcpy_s(filepathChar, filepath.c_str());

	currentPos = 0;

	try
	{
		//Create a file
		bi::file_mapping::remove(filepathChar);
		fileSize_32 = fS_32;

		std::filebuf fbuf;
		fbuf.open(filepathChar, std::ios_base::in | std::ios_base::out
			| std::ios_base::trunc | std::ios_base::binary);
		//Set the size
		fbuf.pubseekoff((fileSize_32 * 4) - 1, std::ios_base::beg);
		fbuf.sputc(0);

		bi::file_mapping m_file(filepathChar, bi::read_write);
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

void OutputStream04Bis::write(int32_t * elements)
{
	//Create a file mapping;
	bi::file_mapping m_file(filepathChar, bi::read_write);

	//Map the whole file with read-only permissions in this process
	bi::mapped_region region(m_file, bi::read_write, currentPos * sizeof(int32_t), bufferSize * sizeof(int32_t));

	currentPos += bufferSize; //increment size

	int32_t* memAddressOut = (int32_t*)region.get_address();

	memcpy(memAddressOut, elements, sizeof(int32_t)*bufferSize);
}

void OutputStream04Bis::close()
{
	// todo if anything needs to be done -> flush probably
}
