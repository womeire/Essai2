#include "pch.h"
#include "InputStream04.h"

InputStream04::InputStream04()
{
	currentPos_8 = 0;
}


InputStream04::~InputStream04()
{
}

void InputStream04::open(string filepath, size_t bufSize)
{
	bufferSize_8 = bufSize * sizeof(int32_t); //bufSize is the size of the buffer in size int32_t, it's translated here to size 8b for easier use later (functions take 8b values)
	bufferSize_32 = bufSize;
	end_of_file = false;
	strcpy_s(filepathChar, filepath.c_str());

	struct stat buf;
	stat(filepathChar, &buf);
	fileSize_8 = buf.st_size;

	elements = (int32_t*)malloc(bufSize);

	try
	{
		bi::file_mapping m_file(filepathChar, bi::read_only);
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

int32_t* InputStream04::read_next()
{
	bi::file_mapping m_file(filepathChar, bi::read_only);
	bi::mapped_region regionIn(m_file, bi::read_only, currentPos_8, bufferSize_8);

	memcpy(elements, (int32_t*)regionIn.get_address(), bufferSize_32); // still have to code for when the buffer's not full - use the knowledge of bufsize & filesize


	currentPos_8 += bufferSize_8;
	if (currentPos_8 >= fileSize_8) {
		end_of_file = true;
	}
	return elements;
}

bool InputStream04::end_of_stream()
{
	return end_of_file;
}

void InputStream04::reset() {
	currentPos_8 = 0;
}

string InputStream04::getName() {
	return "Stream04";
}