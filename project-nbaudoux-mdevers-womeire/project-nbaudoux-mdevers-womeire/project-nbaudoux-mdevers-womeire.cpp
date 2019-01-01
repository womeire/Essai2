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
#include "OutputStream04Bis.h"
#include "Benchmarking.h"
#include "Multiway.h"

/*const std::size_t NB_TESTS = 1000;
const std::size_t NB_STREAMS = 30;
const std::size_t NB_OF_ELEMENTS = 25600;*/ 
const std::size_t NB_TESTS = 1;
const std::size_t NB_STREAMS = 1;
const std::size_t NB_OF_ELEMENTS = 10;
const int BUFFER_SIZE = 256;

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

	for (int i = 0; !inStream2.end_of_stream(); i++)
	{
		outStream2.write(inStream2.read_next());
	};

	outStream2.close();
}

void testStream04Bis(string filepathRead, string filepathWrite) {
	InputStream04Bis inStream4b;
	inStream4b.open(filepathRead, BUFFER_SIZE);
	
	OutputStream04Bis outStream4b;
	outStream4b.create(filepathWrite, filepathRead, BUFFER_SIZE);

	for (int i = 0; !inStream4b.end_of_stream(); i++)
	{
		outStream4b.write(inStream4b.read_next());
	};

	outStream4b.close();
}

//void testStream03(string filepathRead, string filepathWrite) {
//
//	InputStream03 inStream3;
//	inStream3.open(filepathRead, BUFFER_SIZE);
//
//	OutputStream03 outStream3;
//	outStream3.create(filepathWrite, BUFFER_SIZE);
//
//	int32_t * buffer3 = (int32_t*)malloc(sizeof(int32_t) * BUFFER_SIZE);
//	int relativePosition = 0;
//
//	for (int i = 0; !inStream3.end_of_stream(); ++i) {
//		relativePosition = i % BUFFER_SIZE;
//		if (i > 0 && relativePosition == 0)
//			outStream3.write(buffer3); // It might be counter-intuitive to do "write" first, but else the first value of buffer will already be overwritten by the new value in buffer
//
//		buffer3[relativePosition] = inStream3.read_next();
//	}
//	if (relativePosition > 0) { // the end of file is not necessary reached on a multiple op bufferSize, thus a final write is necessary
//		outStream3.write(buffer3, relativePosition);  // todo is having two kind of writes the good way to go? Default values for parameters work only for static in c++
//	}
//
//	outStream3.close();
//}

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
	testStream04Bis("C:\\project-tests\\exampleRead1.txt", "C:\\project-tests\\exampleWrite1.txt");
//#pragma region FileCreation
//	printf("Starting file creation.\n");
//
//	////////////////////////	Create the filepaths	////////////////////////////////////
//	string filepathsRead[NB_STREAMS];
//	string filepathsWrite[NB_STREAMS];
//
//	for (size_t i = 0; i < NB_STREAMS; i++)
//	{
//		std::ostringstream stream;
//		//Defines different paths according the OS
//		if (_WIN32) {
//			//_WIN32 works for both WIN32 and WIN64
//			//!WARNING! A folder with the name 'project-tests' has to be created on C: drive
//
//			stream.str("");
//			stream << "C:\\project-tests\\exampleRead" << i << ".txt"; // todo maybe not use text but some binary file extension
//			filepathsRead[i] = stream.str();
//			stream.str("");
//			stream << "C:\\project-tests\\exampleWrite" << i << ".txt";
//			filepathsWrite[i] = stream.str();
//		}
//		else {
//			//For MAC todo
//			filepathsRead[i] = "";
//			filepathsWrite[i] = "";
//		};
//	}
//
//	////////////////////////	Generate the files	////////////////////////////////////
//	for (size_t i = 0; i < NB_STREAMS; i++)
//	{
//		new RandomFileCreation(filepathsRead[i], NB_OF_ELEMENTS);
//	}
//
//#pragma endregion
//
//#pragma region Benchmarking - computing
//	printf("Starting the benchmarking.\n");
//
//
//	//todo change benchmark to take NB_TESTS into account
//	double AverageChrono;
//	AverageChrono = 0;
//
//	Benchmarking StreamChronos[NB_STREAMS];
//
//	InputStream01 inStreams[NB_STREAMS];
//	OutputStream01 outStreams[NB_STREAMS];
//
//	//open and create all streams
//	for (size_t i = 0; i < NB_STREAMS; i++) {
//		inStreams[i].open(filepathsRead[i]);
//		outStreams[i].create(filepathsWrite[i]);
//	}
//
//	//Read all stream values one after the other todo does not take into account endoffile yet...
//	// todo separate chronos for read and write? I'd say no
//	for (size_t i = 0; i < NB_OF_ELEMENTS; i++) {
//		for (size_t j = 0; j < NB_STREAMS; j++)
//		{
//			StreamChronos[j].startTest();
//			outStreams[j].write(inStreams[j].read_next());
//			StreamChronos[j].stopTest();
//		}
//	}
//
//	//close all streams
//	for (size_t i = 0; i < NB_STREAMS; i++) {
//		outStreams[i].close();
//	}
//#pragma endregion
//
//#pragma region Benchmarking - results
//
//	printf("\"Printing\" the results.\n");
//
//	std::fstream resultsFile;
//
//	std::time_t current_time;
//	struct tm time_info;
//	time(&current_time);
//	localtime_s(&time_info, &current_time);
//	stringstream benchInfo;
//	benchInfo << "N" << NB_TESTS << "_S" << NB_STREAMS << "_"
//		<< time_info.tm_hour << "h"	<< time_info.tm_min << "m" << time_info.tm_sec << "s_"
//		<< (time_info.tm_mon + 1) << "_" << (time_info.tm_year + 1900);
//
//	string filename = "C:\\project-tests\\BenchResult_";
//	filename += benchInfo.str();
//	filename += ".csv";
//	resultsFile.open(filename, std::ios::out | std::ios::trunc);
//
//
//	resultsFile << "Stream#" << ";" << "Highest" << ";" << "Lowest" 
//		<< ";" << "LastPerformance" << ";" << "AvgPerformance" 
//		<< ";" << "TotalTime" << "\n";
//
//	for (size_t i = 0; i < NB_STREAMS; i++)
//	{
//		resultsFile << i << ";" << StreamChronos[i].getHighest() << ";" << StreamChronos[i].getLowest()
//			<< ";" << StreamChronos[i].getLastPerformance() << ";" << StreamChronos[i].getAvgPerformance()
//			<< ";" << StreamChronos[i].getTotalTime() << "\n";
//	}
//
//	resultsFile.close();
//
//	printf("The results have been stored.\n");
//#pragma endregion	
//	/*std::vector<std::string> streams = { filepathRead };
//	Multiway multiwayTest (streams);
//	multiwayTest.read();
//	multiwayTest.merge();
//	multiwayTest.showRes();*/
//
	return 0;
}