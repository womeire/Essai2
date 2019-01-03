#include "pch.h"
#include "RandomFileCreation.h"
using namespace std;

RandomFileCreation::RandomFileCreation(string filepath, size_t length)
{
	//Delete file if it existed
	char filepathChar[_MAX_PATH];
	strcpy_s(filepathChar, filepath.c_str());
	std::remove(filepathChar);

	//create file
	ofstream myfile;
	myfile.open(filepath, ios::binary | ios::out);
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int32_t> distribution(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());

	for (size_t i = 0; i < length; i++)
	{
		int32_t value = distribution(generator);
		myfile.write((char *)&value, sizeof(int32_t));
	}

	myfile.close();
}

RandomFileCreation::~RandomFileCreation()
{
}
