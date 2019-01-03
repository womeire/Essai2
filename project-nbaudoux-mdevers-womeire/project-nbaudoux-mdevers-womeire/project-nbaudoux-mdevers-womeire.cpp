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
#include "OutputStream04.h"
#include "Benchmarking.h"
#include "Multiway.h"

/*const std::size_t NB_TESTS = 1000;
const std::size_t NB_STREAMS = 30;
const std::size_t NB_OF_ELEMENTS = 25600;*/
const std::size_t NB_TESTS = 2;
const std::size_t NB_STREAMS = 2;
const int BUFFER_SIZE = 65536; //65536 = page_size of memory mapping
const std::size_t NB_ELEMENTS = BUFFER_SIZE * 30000; // Should not exceed capacity of size_t (= 4294967295)!

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
	InputStream04 inStream4b;
	inStream4b.open(filepathRead, BUFFER_SIZE);

	OutputStream04 outStream4b;
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
		//bi::managed_mapped_file m_fileOut(bi::create_only, filepathWriteChar, buf.st_size);

		//Map the whole file with read-only permissions in this process
		bi::mapped_region regionOut(m_fileOut, bi::read_write, 0, buf.st_size);
		bi::mapped_region regionIn(m_fileIn, bi::read_only, 0, buf.st_size);
		auto test = regionIn.get_page_size();
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

#pragma region FileCreation
	printf("Starting file creation.\n");

	////////////////////////	Create the filepaths	////////////////////////////////////
	string filepathsRead[NB_STREAMS];
	string filepathsWrite[NB_STREAMS * 4];

	for (size_t i = 0; i < NB_STREAMS; i++)
	{
		std::ostringstream stream;
		//Defines different paths according the OS
		if (_WIN32) {
			//_WIN32 works for both WIN32 and WIN64
			//!WARNING! A folder with the name 'project-tests' has to be created on C: drive

			stream.str("");
			stream << "C:\\project-tests\\exampleRead" << i << ".txt"; // todo maybe not use text but some binary file extension
			filepathsRead[i] = stream.str();
			for (std::size_t j = 0; j < 4; j++) {
				stream.str("");
				stream << "C:\\project-tests\\exampleWrite" << i << "_" << j << ".txt";
				filepathsWrite[i * 4 + j] = stream.str();
			}
		}
		else {
			//For MAC todo
			filepathsRead[i] = "";
			filepathsWrite[i] = "";
		};
	}

	////////////////////////	Generate the files	////////////////////////////////////
	for (size_t i = 0; i < NB_STREAMS; i++)
	{
		new RandomFileCreation(filepathsRead[i], NB_ELEMENTS);
	}

#pragma endregion

#pragma region Benchmarking - computing
	printf("Starting the benchmarking.\n");


	//todo change benchmark to take NB_TESTS into account
	double AverageChrono;
	AverageChrono = 0;

	Benchmarking StreamChronos01[NB_STREAMS];
	Benchmarking StreamChronos02[NB_STREAMS];
	Benchmarking StreamChronos03[NB_STREAMS];
	Benchmarking StreamChronos04[NB_STREAMS];

	InputStream01 inStreams01[NB_STREAMS];
	OutputStream01 outStreams01[NB_STREAMS];

	InputStream02 inStreams02[NB_STREAMS];
	OutputStream02 outStreams02[NB_STREAMS];

	InputStream03 inStreams03[NB_STREAMS];
	OutputStream03 outStreams03[NB_STREAMS];

	InputStream04 inStreams04[NB_STREAMS];
	OutputStream04 outStreams04[NB_STREAMS];

	//open and create all streams
	for (size_t i = 0; i < NB_STREAMS; i++) {
		inStreams01[i].open(filepathsRead[i]);
		outStreams01[i].create(filepathsWrite[i * 4]);

		inStreams02[i].open(filepathsRead[i]);
		outStreams02[i].create(filepathsWrite[i * 4 + 1]);

		inStreams03[i].open(filepathsRead[i], BUFFER_SIZE);
		outStreams03[i].create(filepathsWrite[i * 4 + 2], BUFFER_SIZE);

		inStreams04[i].open(filepathsRead[i], BUFFER_SIZE);
		outStreams04[i].create(filepathsWrite[i * 4 + 3], filepathsRead[i], BUFFER_SIZE);
	}

	for (std::size_t i = 0; i < NB_ELEMENTS; i++) {
		for (size_t j = 0; j < NB_STREAMS; j++) {
			if (!inStreams01[j].end_of_stream()) {
				StreamChronos01[j].startTest();
				outStreams01[j].write(inStreams01[j].read_next());
				StreamChronos01[j].stopTest();
			}

			if (!inStreams02[j].end_of_stream()) {
				StreamChronos02[j].startTest();
				outStreams02[j].write(inStreams02[j].read_next());
				StreamChronos02[j].stopTest();
			}

			if (!inStreams03[j].end_of_stream()) {
				StreamChronos03[j].startTest();
				outStreams03[j].write(inStreams03[j].read_next());
				StreamChronos03[j].stopTest();
			}

			if (!inStreams04[j].end_of_stream()) {
				StreamChronos04[j].startTest();
				outStreams04[j].write(inStreams04[j].read_next());
				StreamChronos04[j].stopTest();
			}
		}
	}

	//close all streams
	for (size_t i = 0; i < NB_STREAMS; i++) {
		outStreams01[i].close();
		outStreams02[i].close();
		outStreams03[i].close();
		outStreams04[i].close();
	}

	//std::cout << "Lowest appears " << StreamChronos03[0].getNbLowest() << " times and is " << StreamChronos03[0].getLowest() << std::endl;
#pragma endregion

#pragma region Benchmarking - results

	printf("\"Printing\" the results.\n");

	std::fstream resultsFile;

	std::time_t current_time;
	struct tm time_info;
	time(&current_time);
	localtime_s(&time_info, &current_time);
	stringstream benchInfo;
	benchInfo << "N" << NB_TESTS << "_S" << NB_STREAMS << "_B" << BUFFER_SIZE << "_"
		<< time_info.tm_hour << "h" << time_info.tm_min << "m" << time_info.tm_sec << "s_"
		<< (time_info.tm_mday) << "_" << (time_info.tm_mon + 1);

	string filename = "C:\\project-tests\\BenchResult_";
	filename += benchInfo.str();
	filename += ".csv";
	resultsFile.open(filename, std::ios::out | std::ios::trunc);


	resultsFile << "Method" << ";" << "Stream#" << ";" << "Highest" << ";" << "Lowest"
		<< ";" << "LastPerformance" << ";" << "AvgPerformance"
		<< ";" << "TotalTime" << "\n";

	for (size_t i = 0; i < NB_STREAMS; i++)
	{
		resultsFile << "1" << ";" << i << ";" << StreamChronos01[i].getHighest() << ";" << StreamChronos01[i].getLowest()
			<< ";" << StreamChronos01[i].getLastPerformance() << ";" << StreamChronos01[i].getAvgPerformance()
			<< ";" << StreamChronos01[i].getTotalTime() << "\n";

		resultsFile << "2" << ";" << i << ";" << StreamChronos02[i].getHighest() << ";" << StreamChronos02[i].getLowest()
			<< ";" << StreamChronos02[i].getLastPerformance() << ";" << StreamChronos02[i].getAvgPerformance()
			<< ";" << StreamChronos02[i].getTotalTime() << "\n";

		resultsFile << "3" << ";" << i << ";" << StreamChronos03[i].getHighest() << ";" << StreamChronos03[i].getLowest()
			<< ";" << StreamChronos03[i].getLastPerformance() << ";" << StreamChronos03[i].getAvgPerformance()
			<< ";" << StreamChronos03[i].getTotalTime() << "\n";

		resultsFile << "4" << ";" << i << ";" << StreamChronos04[i].getHighest() << ";" << StreamChronos04[i].getLowest()
			<< ";" << StreamChronos04[i].getLastPerformance() << ";" << StreamChronos04[i].getAvgPerformance()
			<< ";" << StreamChronos04[i].getTotalTime() << "\n";
	}

	system("pause");

	resultsFile.close();

	printf("The results have been stored.\n");
#pragma endregion	
	/*std::vector<std::string> streams = { filepathsRead[0] };
	Multiway multiwayTest (streams);
	multiwayTest.read(BUFFER_SIZE);
	multiwayTest.merge();
	multiwayTest.showRes();*/
	//
	return 0;
}