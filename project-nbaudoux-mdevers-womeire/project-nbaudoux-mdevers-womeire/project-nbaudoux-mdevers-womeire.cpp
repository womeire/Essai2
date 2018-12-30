// project-nbaudoux-mdevers-womeire.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "RandomFileCreation.h"
#include "InputStream01.h"
#include "OutputStream01.h"
#include "InputStream02.h"
#include "OutputStream02.h"
#include "InputStream03.h"
#include "OutputStream03.h"
#include "InputStream04.h"
#include "InputStream04Bis.h"
#include "OutputStream04.h"
#include "Benchmarking.h"
#include "Multiway.h"

const std::size_t NB_TESTS = 100;
const int bufferSize = 256;

void testStream01(string filepathRead, string filepathWrite) {
	InputStream01 inStream1;
	inStream1.open(filepathRead);

	OutputStream01 outStream1;
	outStream1.create(filepathWrite);

	while (!inStream1.end_of_stream())
	{
		outStream1.write(inStream1.read_next());
	}

	outStream1.close();
}

void testStream02(string filepathRead, string filepathWrite) {
	InputStream02 inStream2;
	inStream2.open(filepathRead);

	OutputStream02 outStream2;
	outStream2.create(filepathWrite);

	int32_t * buffer2 = (int32_t*)malloc(sizeof(int32_t) * inStream2.fileSize_32);
	int i;
	for (i = 0; !inStream2.end_of_stream(); i++)
	{
		buffer2[i] = inStream2.read_next();
	}

	outStream2.write(buffer2, i - 1);

	outStream2.close();
}

void testStream03(string filepathRead, string filepathWrite) {

	InputStream03 inStream3;
	inStream3.open(filepathRead, bufferSize);

	OutputStream03 outStream3;
	outStream3.create(filepathWrite, bufferSize);

	int32_t * buffer3 = (int32_t*)malloc(sizeof(int32_t) * bufferSize);
	int relativePosition = 0;

	for (int i = 0; !inStream3.end_of_stream(); ++i) {
		relativePosition = i % bufferSize;
		if (i > 0 && relativePosition == 0)
			outStream3.write(buffer3); // It might be counter-intuitive to do "write" first, but else the first value of buffer will already be overwritten by the new value in buffer

		buffer3[relativePosition] = inStream3.read_next();
	}
	if (relativePosition > 0) { // the end of file is not necessary reached on a multiple op bufferSize, thus a final write is necessary
		outStream3.write(buffer3, relativePosition);  // todo is having two kind of writes the good way to go? Default values for parameters work only for static in c++
	}

	outStream3.close();
}

void testStream04(string filepathRead, string filepathWrite) {
	InputStream04 inStream4;
	char cstr[_MAX_PATH];//wild testing
	strcpy_s(cstr, filepathRead.c_str());
	struct stat buf;
	stat(cstr, &buf);
	inStream4.open(filepathRead, bufferSize);

	OutputStream04 outStream4;
	outStream4.create(filepathWrite, buf.st_size / 4, bufferSize);

	while (!inStream4.end_of_stream())
	{
		outStream4.write(inStream4.read_next(), bufferSize);
	}

	outStream4.close();
}

void DoIt(string filepathRead, string filepathWrite) {
	char filepathReadChar[_MAX_PATH];
	char filepathWriteChar[_MAX_PATH];
	strcpy_s(filepathReadChar, filepathRead.c_str());
	strcpy_s(filepathWriteChar, filepathWrite.c_str());

	struct stat buf;
	stat(filepathReadChar, &buf);
	try {
		//Create a file
		bi::file_mapping::remove(filepathWriteChar);
		std::filebuf fbuf;
		fbuf.open(filepathWriteChar, std::ios_base::in | std::ios_base::out
			| std::ios_base::trunc | std::ios_base::binary);
		//Set the size
		fbuf.pubseekoff(buf.st_size - 1, std::ios_base::beg);
		fbuf.sputc(0);
		fbuf.close();

		//Create a file mapping
		bi::file_mapping m_fileIn(filepathReadChar, bi::read_only);
		bi::file_mapping m_fileOut(filepathWriteChar, bi::read_write);
		//bi::managed_mapped_file m_fileOut(bi::create_only, filepathWriteChar, buf.st_size);

		//Map the whole file with read-only permissions in this process
		bi::mapped_region regionOut(m_fileOut, bi::read_write, 0, buf.st_size);
		bi::mapped_region regionIn(m_fileIn, bi::read_only, 0, buf.st_size);

		int32_t * memAddressIn = (int32_t*)regionIn.get_address();
		int32_t * memAddressOut = (int32_t*)regionOut.get_address();

		for (int32_t i = 0; i < buf.st_size / 4; i++) // todo does it need to be in a "for" loop? Cant i giva a range and it copies all?
		{
			*(memAddressOut + i) = *(memAddressIn + i);
		}
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

int main()
{
	string filepathRead;
	string filepathWrite;

	//Defines different paths according the OS
	if (_WIN32) {
		//_WIN32 works for both WIN32 and WIN64

		//A folder with the name 'project-tests' has to be created on C: drive
		filepathRead = "C:\\project-tests\\exampleRead.txt";
		filepathWrite = "C:\\project-tests\\exampleWrite.txt";
	}
	else {
		//For MAC
		string filepathRead = "";
		string filepathWrite = "";
	};

	//Uncomment if you want to create a new random file
	//new RandomFileCreation(filepathRead, 100);

	//Declare a chrono for each input/output
	Benchmarking testChrono01, testChrono02, testChrono03, testChrono04, testChrono05;
	//
	//#pragma region Stream01
	//	for (std::size_t i = 0; i < NB_TESTS; i++) {
	//		testChrono01.startTest();
	//		testStream01(filepathRead, filepathWrite);
	//		testChrono01.stopTest();
	//		printf("It took %f milliseconds to complete the last OutputStream1.\n", testChrono01.getLastPerformance());
	//	}
	//#pragma endregion
	//
	//#pragma region Stream02
	//	for (std::size_t i = 0; i < NB_TESTS; i++) {
	//		testChrono02.startTest();
	//		testStream02(filepathRead, filepathWrite);
	//		testChrono02.stopTest();
	//		printf("It took %f milliseconds to complete the last OutputStream2.\n", testChrono02.getLastPerformance());
	//	}
	//#pragma endregion
	//
	//#pragma region Stream03
	//	for (std::size_t i = 0; i < NB_TESTS; i++) {
	//		testChrono03.startTest();
	//		testStream03(filepathRead, filepathWrite);
	//		testChrono03.stopTest();
	//		printf("It took %f milliseconds to complete the last OutputStream3.\n", testChrono03.getLastPerformance());
	//	}
	//#pragma endregion

#pragma region Stream 04
	for (std::size_t i = 0; i < NB_TESTS; i++) {
		testChrono04.startTest();
		testStream04(filepathRead, filepathWrite);
		testChrono04.stopTest();
		testChrono05.startTest();
		DoIt(filepathRead, filepathWrite);
		testChrono05.stopTest();
	}
#pragma endregion


#pragma region BenchmarkTest
	//printf("\n");
	//printf("It took at most %f milliseconds to complete OutputStream1.\n", testChrono01.getHighest());
	//printf("It took at least %f milliseconds to complete OutputStream1.\n", testChrono01.getLowest());
	//printf("It took %f milliseconds to complete the last OutputStream1.\n", testChrono01.getLastPerformance());
	//printf("It took in avg %f milliseconds to complete OutputStream1.\n", testChrono01.getAvgPerformance());
	//printf("\n");
	//printf("It took at most %f milliseconds to complete OutputStream2.\n", testChrono02.getHighest());
	//printf("It took at least %f milliseconds to complete OutputStream2.\n", testChrono02.getLowest());
	//printf("It took %f milliseconds to complete the last OutputStream2.\n", testChrono02.getLastPerformance());
	//printf("It took in avg %f milliseconds to complete OutputStream2.\n", testChrono02.getAvgPerformance());
	//printf("\n");
	//printf("It took at most %f milliseconds to complete OutputStream3.\n", testChrono03.getHighest());
	//printf("It took at least %f milliseconds to complete OutputStream3.\n", testChrono03.getLowest());
	//printf("It took %f milliseconds to complete the last OutputStream3.\n", testChrono03.getLastPerformance());
	//printf("It took in avg %f milliseconds to complete OutputStream3.\n", testChrono03.getAvgPerformance());
	printf("\n");
	printf("It took at most %f milliseconds to complete testChrono04.\n", testChrono04.getHighest());
	printf("It took at least %f milliseconds to complete testChrono04.\n", testChrono04.getLowest());
	printf("It took %f milliseconds to complete the last testChrono04.\n", testChrono04.getLastPerformance());
	printf("It took in avg %f milliseconds to complete testChrono04.\n", testChrono04.getAvgPerformance());
	printf("\n");
	printf("It took at most %f milliseconds to complete testChrono05.\n", testChrono05.getHighest());
	printf("It took at least %f milliseconds to complete testChrono05.\n", testChrono05.getLowest());
	printf("It took %f milliseconds to complete the last testChrono05.\n", testChrono05.getLastPerformance());
	printf("It took in avg %f milliseconds to complete testChrono05.\n", testChrono05.getAvgPerformance());

#pragma endregion

	/*std::vector<std::string> streams = { filepathRead };
	Multiway multiwayTest (streams);
	multiwayTest.read();
	multiwayTest.merge();
	multiwayTest.showRes();*/


	return 0;
}