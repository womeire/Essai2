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

void OutputStream04::create(string filepathOut, size_t bufSize)
{
	string filepathIn = "this is just temporary"; //todo !!!
	bufferSize_8 = bufSize * sizeof(int32_t); //bufSize is the size of the buffer in size int32_t, it's translated here to size 8b for easier use later (functions take 8b values)

	strcpy_s(filepathInChar, filepathIn.c_str());
	strcpy_s(filepathOutChar, filepathOut.c_str());

	struct stat buf;
	stat(filepathInChar, &buf);
	fileSize_8 = bufferSize_8; // todo change back

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

void OutputStream04::write(int32_t* sizeToRead)
{
	//Create a file mapping
	bi::file_mapping m_fileOut(filepathOutChar, bi::read_write);

	//Map the whole file with read-only permissions in this process
	bi::mapped_region regionOut(m_fileOut, bi::read_write, currentPos_8, bufferSize_8);

	int32_t* memAddressOut = (int32_t*)regionOut.get_address();
	memcpy(memAddressOut, sizeToRead, bufferSize_8);

	currentPos_8 += bufferSize_8;
}

void OutputStream04::write_s(int8_t* sizeToRead)
{
	//Create a file mapping
	bi::file_mapping m_fileOut(filepathOutChar, bi::read_write);

	//Map the whole file with read-only permissions in this process
	bi::mapped_region regionOut(m_fileOut, bi::read_write, currentPos_8, bufferSize_8);

	int8_t* memAddressOut = (int8_t*)regionOut.get_address();
	memcpy(memAddressOut, sizeToRead, bufferSize_8);

	currentPos_8 += bufferSize_8;
}

void OutputStream04::close()
{
	currentPos_8 = 0;
}
