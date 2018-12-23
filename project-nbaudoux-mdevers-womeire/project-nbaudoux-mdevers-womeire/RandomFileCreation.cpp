#include "pch.h"
#include "RandomFileCreation.h"
using namespace std;

RandomFileCreation::RandomFileCreation(string filepath, int length)
{
	ofstream myfile;
	myfile.open(filepath, ios::binary | ios::out);

	std::default_random_engine generator;
	std::uniform_int_distribution<int32_t> distribution(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());

	for (int i = 0; i < length; i++)
	{
		int32_t value = distribution(generator);
		myfile.write((char *)&value, sizeof(int32_t));
	}

	myfile.close();
}

RandomFileCreation::~RandomFileCreation()
{
}
