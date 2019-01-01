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
//65536 is the page size in bytes of mapped_region, best have buffer sizes that are a multiple of it.[On Wouter's computer]
const int BUFFER_SIZE = 65536 / 4; // buffer size in 4 bytes of size (int_32t);
const std::size_t NB_OF_ELEMENTS = BUFFER_SIZE * 1000;

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

void testStream03(string filepathRead, string filepathWrite) {
	InputStream03 inStream3;
	inStream3.open(filepathRead, BUFFER_SIZE);

	OutputStream03 outStream3;
	outStream3.create(filepathWrite, BUFFER_SIZE);

	for (int i = 0; !inStream3.end_of_stream(); i++)
	{
		outStream3.write(inStream3.read_next());
	};

	outStream3.close();
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

		//Map the whole file with read-only permissions in this process
		bi::mapped_region regionOut(m_fileOut, bi::read_write, 0, buf.st_size);
		bi::mapped_region regionIn(m_fileIn, bi::read_only, 0, buf.st_size);

		/*regionOut.swap(regionIn); // bad test, did not help
		regionOut.flush();*/

		int8_t * memAddressIn = (int8_t*)regionIn.get_address();
		int8_t * memAddressOut = (int8_t*)regionOut.get_address();
		
		memcpy(memAddressOut, memAddressIn, buf.st_size); // Ce n'est pas logique de devoir dupliquer la mémoire !!! Il doit y avoir moyer de faire autrement !
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}


int main()
{

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



	Benchmarking chrono1, chrono2, chrono3, chrono4, chronoDoIt;

	//chrono1.startTest();
	//testStream01("C:\\project-tests\\exampleRead0.txt", "C:\\project-tests\\exampleWrite0.txt");
	//chrono1.stopTest();

	//chrono2.startTest();
	//testStream02("C:\\project-tests\\exampleRead0.txt", "C:\\project-tests\\exampleWrite0.txt");
	//chrono2.stopTest();

	chrono3.startTest();
	testStream03("C:\\project-tests\\exampleRead0.txt", "C:\\project-tests\\exampleWrite0.txt");
	chrono3.stopTest();

	chrono4.startTest();
	testStream04Bis("C:\\project-tests\\exampleRead0.txt", "C:\\project-tests\\exampleWrite0.txt");
	chrono4.stopTest();

	chronoDoIt.startTest();
	DoIt("C:\\project-tests\\exampleRead0.txt", "C:\\project-tests\\exampleWrite0.txt");
	chronoDoIt.stopTest();
	
	//printf("01 -> %f.\n", chrono1.getLastPerformance());
	//printf("02 -> %f.\n", chrono2.getLastPerformance());
	printf("03 -> %f.\n", chrono3.getLastPerformance());
	printf("04 -> %f.\n", chrono4.getLastPerformance());
	printf("di -> %f.\n", chronoDoIt.getLastPerformance());

	return 0;
}