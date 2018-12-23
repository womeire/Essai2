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
#include "Benchmarking.h"

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

#pragma region Stream01
	/*InputStream01 inStream;
	inStream.open(filepathRead);

	OutputStream01 outStream;
	outStream.create(filepathWrite);

	while (!inStream.end_of_stream())
	{
		outStream.write(inStream.read_next());
	}
	outStream.close();*/
#pragma endregion

#pragma region Stream02
	/*InputStream02 inStream;
	inStream.open(filepathRead);

	OutputStream02 outStream;
	outStream.create(filepathWrite);

	int32_t * buffer = (int32_t*) malloc (sizeof(int32_t) * inStream.fileSize_32);
	int i;
	for (i = 0; !inStream.end_of_stream(); i++)
	{
		buffer[i] = inStream.read_next();
	}
	outStream.write(buffer, i-1);

	outStream.close();*/
#pragma endregion

#pragma region Stream03
	int bufferSize = 18;

	InputStream03 inStream;
	inStream.open(filepathRead, bufferSize);

	OutputStream03 outStream;
	outStream.create(filepathWrite, bufferSize);

	int32_t * buffer = (int32_t*)malloc(sizeof(int32_t) * bufferSize);
	int relativePosition = 0;

	Benchmarking testChrono;
	testChrono.startTest();

	for (int i = 0;  !inStream.end_of_stream(); ++i) {
		relativePosition = i % bufferSize;
		if(i > 0 && relativePosition == 0)
			outStream.write(buffer); // It might be counter-intuitive to do "write" first, but else the first value of buffer will already be overwritten by the new value in buffer

		buffer[relativePosition] = inStream.read_next();
	}
	if (relativePosition > 0) { // the end of file is not necessary reached on a multiple op bufferSize, thus a final write is necessary
		outStream.write(buffer, relativePosition);  // todo is having two kind of writes the good way to go? Default values for parameters work only for static in c++
	}

	testChrono.stopTest();
	printf("It took %f milliseconds to complete.\n", testChrono.getPerformance().count());

	outStream.close();

#pragma endregion

#pragma region Stream 04

#pragma endregion


#pragma region BenchmarkTest

	////int tst04 = CreateFileMapping();
	////boost::regex pat("^Subject: (Re: |Aw: )*(.*)");
	////Benchmarking testChrono;
	//testChrono.startTest();

	//Sleep(100);

	//testChrono.stopTest();

	//printf("It took %f seconds to complete.\n", testChrono.getPerformance());


#pragma endregion


	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
