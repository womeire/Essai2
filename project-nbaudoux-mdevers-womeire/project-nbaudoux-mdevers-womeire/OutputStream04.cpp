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

void OutputStream04::create(string filepath, size_t bufSize_32, size_t fileSize_8)
{

	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int32_t> distribution(0, std::numeric_limits<int32_t>::max());
	int32_t value = distribution(generator);
	char buf[33];
	_itoa_s(value, buf, 10);

	filepath.append(buf);
	filepath.append(".txt");

	bufferSize_8 = bufSize_32 * sizeof(int32_t); //bufSize is the size of the buffer in size int32_t, it's translated here to size 8b for easier use later (functions take 8b values)
	strcpy_s(filepathChar, filepath.c_str());
	fileSize_8 = fileSize_8;

	try
	{
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
	if (currentPos_8 + bufferSize_8 >= fileSize_8) {
		range_8 = fileSize_8 - currentPos_8;
		if (range_8 <= 0)
			return;
	}
	else
		range_8 = bufferSize_8;
	try {
		//Create a file mapping
		bi::file_mapping m_file(filepathChar, bi::read_write);

		//Map the whole file with read-only permissions in this process
		bi::mapped_region region(m_file, bi::read_write, currentPos_8, range_8);

		memcpy((int32_t*)region.get_address(), elements, range_8);

		currentPos_8 += bufferSize_8;
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

void OutputStream04::write_8(int8_t* elements_8)
{
	if (currentPos_8 + bufferSize_8 >= fileSize_8) {
		range_8 = fileSize_8 - currentPos_8;
	}
	else
		range_8 = bufferSize_8;

	//Create a file mapping
	bi::file_mapping m_file(filepathChar, bi::read_write);

	//Map the whole file with read-only permissions in this process
	bi::mapped_region region(m_file, bi::read_write, currentPos_8, range_8);

	memcpy((int8_t*)region.get_address(), elements_8, range_8);

	currentPos_8 += bufferSize_8;
}

void OutputStream04::close()
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	currentPos_8 = 0;
	//bi::file_mapping::remove(filepathChar);
}
