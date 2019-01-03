#include "pch.h"
#include "OutputStream04.h"

OutputStream04::OutputStream04()
{
}


OutputStream04::~OutputStream04()
{
}

void OutputStream04::create(string filepath, int fileSize, int bufSize)
{
	bufferSize = bufSize;

	strcpy_s(filepathChar, filepath.c_str());
	buffer.resize(bufferSize);

	struct stat buf;
	stat(filepathChar, &buf);
	fileSize_32 = fileSize;
	currentPos = 0;

	try
	{
		//Create a file
		bi::file_mapping::remove(filepathChar);
		std::filebuf fbuf;
		fbuf.open(filepathChar, std::ios_base::in | std::ios_base::out
			| std::ios_base::trunc | std::ios_base::binary);
		//Set the size
		fbuf.pubseekoff((fileSize_32 * 4) - 1, std::ios_base::beg);
		fbuf.sputc(0);

		bi::file_mapping m_file(filepathChar, bi::read_write);

		// todo try managed_mapped_file so as to not have to create an empty file first?
		//bi::managed_mapped_file mfile2(bi::create_only, "MyMappedFile", (fileSize_32 * 4) - 1);
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

void OutputStream04::write(int32_t * elements)
{

}

void OutputStream04::write(int32_t * elements, int size_32)
{
	try
	{
		string bla = "C:\\project-tests\\exampleRead.txt";
		char cstr[_MAX_PATH];//wild testing
		strcpy_s(cstr, bla.c_str());
		//Create a file mapping
		bi::file_mapping m_fileIn(cstr, bi::read_only);
		bi::file_mapping m_fileOut(filepathChar, bi::read_write);

		//Map the whole file with read-only permissions in this process
		bi::mapped_region regionIn(m_fileIn, bi::read_only, currentPos * 4, size_32 * 4);
		bi::mapped_region region(m_fileOut, bi::read_write, currentPos * 4, size_32 * 4);

		currentPos += size_32; //increment size

		memAddress = (int32_t*)regionIn.get_address();
		int32_t* memAddressOut = (int32_t*)region.get_address();

		/*int32_t testa0 = *memAddress;
		int32_t testa1 = *(memAddress + 1);
		int32_t testa2 = *(memAddress + 2);

		int32_t test0 = elements[0];
		int32_t test1 = elements[1];
		int32_t test2 = elements[2];*/

		for (int32_t i = 0; i < size_32; i++) // todo does it need to be in a "for" loop? Cant i giva a range and it copies all?
		{
			*(memAddressOut + i) = *(memAddress + i);
		}
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

void OutputStream04::close()
{
	// todo if anything needs to be done
}
