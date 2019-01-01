#include "pch.h"
#include "OutputStream04Bis.h"

OutputStream04Bis::OutputStream04Bis()
{
}


OutputStream04Bis::~OutputStream04Bis()
{
	close();
}

void OutputStream04Bis::create(string filepathOut, string filepathIn, int bufSize)
{
	bufferSize_8 = bufSize * sizeof(int32_t); //bfSize is the size of the buffer in size int32_t, it's translated here to size 8b for easier use later (functions take 8b values)

	strcpy_s(filepathInChar, filepathIn.c_str());
	strcpy_s(filepathOutChar, filepathOut.c_str());

	struct stat buf;
	stat(filepathInChar, &buf);
	fileSize_8 = buf.st_size;

	try
	{
		//Create a file
		bi::file_mapping::remove(filepathOutChar);

		std::filebuf fbuf;
		fbuf.open(filepathOutChar, std::ios_base::in | std::ios_base::out
			| std::ios_base::trunc | std::ios_base::binary);
		//Set the size
		fbuf.pubseekoff(fileSize_8 - 1, std::ios_base::beg);
		fbuf.sputc(0);

		bi::file_mapping m_fileIn(filepathInChar, bi::read_only);
		bi::file_mapping m_fileOut(filepathOutChar, bi::read_write);
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

void OutputStream04Bis::write(int startingPoint)
{
	//todo maybe check if (startingPoint > filesize)

	//Create a file mappings
	bi::file_mapping m_fileIn(filepathInChar, bi::read_only);
	bi::file_mapping m_fileOut(filepathOutChar, bi::read_write);

	//Map the whole file with read-only permissions in this process
	bi::mapped_region regionIn(m_fileIn, bi::read_only, startingPoint, bufferSize_8);
	bi::mapped_region regionOut(m_fileOut, bi::read_write, startingPoint, bufferSize_8);

	int8_t* memAddressIn = (int8_t*)regionIn.get_address();
	int8_t* memAddressOut = (int8_t*)regionOut.get_address();
	memcpy(memAddressOut, memAddressIn, bufferSize_8);
}

void OutputStream04Bis::close()
{
	// todo if anything needs to be done -> flush probably
}
