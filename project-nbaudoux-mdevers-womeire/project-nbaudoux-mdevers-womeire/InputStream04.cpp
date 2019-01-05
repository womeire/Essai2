#include "pch.h"
#include "InputStream04.h"

InputStream04::InputStream04()
{
	currentPos_8 = 0;
	end_of_file = false;
}


InputStream04::~InputStream04()
{
	free(elements);
	free(elements_8);
}

void InputStream04::open(string filepath, size_t bufSize)
{
	bufferSize_8 = bufSize * sizeof(int32_t); //bufSize is the size of the buffer in size int32_t, it's translated here to size 8b for easier use later (functions take 8b values)
	strcpy_s(filepathChar, filepath.c_str());

	struct stat buf;
	stat(filepathChar, &buf);
	fileSize_8 = buf.st_size;

	elements = (int32_t*)malloc(bufSize);
	elements_8 = (int8_t*)malloc(bufferSize_8);

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
	if (currentPos_8 + bufferSize_8 > fileSize_8)
		range_8 = fileSize_8 - currentPos_8;
	else
		range_8 = bufferSize_8;

	bi::file_mapping m_file(filepathChar, bi::read_only);
	bi::mapped_region regionIn(m_file, bi::read_only, currentPos_8, range_8);

	memcpy(elements, (int32_t*)regionIn.get_address(), range_8); 

	currentPos_8 += bufferSize_8;
	if (currentPos_8 >= fileSize_8)
		end_of_file = true;
	return elements;
}

int8_t* InputStream04::read_next_8()
{
	if (currentPos_8 + bufferSize_8 > fileSize_8) {
		end_of_file = true;
		range_8 = fileSize_8 - currentPos_8;
	}
	else
		range_8 = bufferSize_8;

	bi::file_mapping m_file(filepathChar, bi::read_only);
	bi::mapped_region regionIn(m_file, bi::read_only, currentPos_8, range_8);

	memcpy(elements_8, (int8_t*)regionIn.get_address(), range_8);

	currentPos_8 += bufferSize_8;
	if (currentPos_8 >= fileSize_8)
		end_of_file = true;
	return elements_8;
}

bool InputStream04::end_of_stream()
{
	return end_of_file;
}

void InputStream04::reset() {
	end_of_file = false;
	currentPos_8 = 0;
}

string InputStream04::getName() {
	return "Stream04";
}