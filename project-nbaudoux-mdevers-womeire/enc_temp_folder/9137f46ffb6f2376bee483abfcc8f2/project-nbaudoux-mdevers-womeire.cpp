// project-nbaudoux-mdevers-womeire.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "RandomFileCreation.h"
#include "InputStream.h"
#include "OutputStream.h"
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
#include "External.h"

// Default values for the benchmarking parameters
const std::size_t NB_TESTS = 3;
const std::size_t NB_STREAMS = 2;
const int BUFFER_SIZE = 65536; //65536 = page_size of memory mapping
// NB_ELEMENTS should not exceed capacity of size_t (= 4294967295)!
const std::size_t NB_ELEMENTS = BUFFER_SIZE * 100;
const bool isRelease = false; // changes the folder where the testing is done

string filepathsRead[NB_STREAMS];
string filepathsWrite[NB_STREAMS * 4];

void testMemMapping(string filepathRead, string filepathWrite);

void CreateFiles(size_t);
void BenchmarkStream(std::vector<InputStream*>, std::vector<OutputStream*>, Benchmarking*, size_t, size_t, size_t, size_t);
void BenchmarkResultsToCSV(Benchmarking*, Benchmarking*, Benchmarking*, Benchmarking*, size_t, size_t, size_t, string);
void testExternal();

int main()
{
	//CreateFiles(1);
	//testExternal();
	//std::system("pause");
	//return 0;

	size_t K_values[] = { 1, 2, 5, 10, 30}; // Our test system can go up to 512 simultaneous streams (but asked to do max 30)
	size_t N_values[] = { 1, 1024, 1024 * 1024, 1024 * 1024 * 1024, 1024 * 1024 * 1024 * 3}; // Should not exceed capacity of size_t (= 4294967295)!
	size_t B_values[] = { 1 , 1024, 65536, 1024 * 1024, 65536 * 100}; // Multiples of 65536 are good for stream04 as it's the page_size


	InputStream04 testIn4 = InputStream04();
	OutputStream04 testOut4 = OutputStream04();
	testIn4.open("C:\\Users\\Wouter\\Desktop\\exampleRead0.txt", BUFFER_SIZE);
	struct stat buf;
	stat("C:\\Users\\Wouter\\Desktop\\exampleRead0.txt", &buf);
	testOut4.create("C:\\Users\\Wouter\\Desktop\\exampleWrite0.txt", BUFFER_SIZE, buf.st_size);
	while (!testIn4.end_of_stream())
	{
		testOut4.write(testIn4.read_next());
	}
	testOut4.close();

#pragma region Test_k

	for (size_t k : K_values)
	{
#pragma region Chrono initialisation
		Benchmarking* chronos01 = (Benchmarking*)malloc(sizeof(Benchmarking) * NB_TESTS * k);
		Benchmarking* chronos02 = (Benchmarking*)malloc(sizeof(Benchmarking) * NB_TESTS * k);
		Benchmarking* chronos03 = (Benchmarking*)malloc(sizeof(Benchmarking) * NB_TESTS * k);
		Benchmarking* chronos04 = (Benchmarking*)malloc(sizeof(Benchmarking) * NB_TESTS * k);

		for (size_t i = 0; i < NB_TESTS * k; i++)
		{
			chronos01[i] = Benchmarking();
			chronos02[i] = Benchmarking();
			chronos03[i] = Benchmarking();
			chronos04[i] = Benchmarking();
		}
#pragma endregion

#pragma region Stream initialisation
		std::vector<InputStream*> inStreams01(k);
		std::vector<OutputStream*> outStreams01(k);

		std::vector<InputStream*> inStreams02(k);
		std::vector<OutputStream*> outStreams02(k);

		std::vector<InputStream*> inStreams03(k);
		std::vector<OutputStream*> outStreams03(k);

		std::vector<InputStream*> inStreams04(k);
		std::vector<OutputStream*> outStreams04(k);

		for (size_t i = 0; i < k; i++)
		{
			inStreams01[i] = new InputStream01();
			outStreams01[i] = new OutputStream01();
			inStreams02[i] = new InputStream02();
			outStreams02[i] = new OutputStream02();
			inStreams03[i] = new InputStream03();
			outStreams03[i] = new OutputStream03();
			inStreams04[i] = new InputStream04();
			outStreams04[i] = new OutputStream04();
		}
#pragma endregion

		for (size_t i = 0; i < NB_TESTS; i++)
		{
			CreateFiles(k);

			BenchmarkStream(inStreams01, outStreams01, chronos01, i, k, BUFFER_SIZE, NB_ELEMENTS);
			BenchmarkStream(inStreams02, outStreams02, chronos02, i, k, BUFFER_SIZE, NB_ELEMENTS);
			BenchmarkStream(inStreams03, outStreams03, chronos03, i, k, BUFFER_SIZE, NB_ELEMENTS);
			BenchmarkStream(inStreams04, outStreams04, chronos04, i, k, BUFFER_SIZE, NB_ELEMENTS);
		}

		BenchmarkResultsToCSV(chronos01, chronos02, chronos03, chronos04, k, BUFFER_SIZE, NB_ELEMENTS, "TEST_K");
	}
#pragma endregion

#pragma region Test_N
	for (size_t N : N_values)
	{
#pragma region Chrono initialisation
		Benchmarking* chronos01 = (Benchmarking*)malloc(sizeof(Benchmarking) * NB_TESTS * NB_STREAMS);
		Benchmarking* chronos02 = (Benchmarking*)malloc(sizeof(Benchmarking) * NB_TESTS * NB_STREAMS);
		Benchmarking* chronos03 = (Benchmarking*)malloc(sizeof(Benchmarking) * NB_TESTS * NB_STREAMS);
		Benchmarking* chronos04 = (Benchmarking*)malloc(sizeof(Benchmarking) * NB_TESTS * NB_STREAMS);

		for (size_t i = 0; i < NB_TESTS * NB_STREAMS; i++)
		{
			chronos01[i] = Benchmarking();
			chronos02[i] = Benchmarking();
			chronos03[i] = Benchmarking();
			chronos04[i] = Benchmarking();
		}
#pragma endregion

#pragma region Stream initialisation
		std::vector<InputStream*> inStreams01(NB_STREAMS);
		std::vector<OutputStream*> outStreams01(NB_STREAMS);

		std::vector<InputStream*> inStreams02(NB_STREAMS);
		std::vector<OutputStream*> outStreams02(NB_STREAMS);

		std::vector<InputStream*> inStreams03(NB_STREAMS);
		std::vector<OutputStream*> outStreams03(NB_STREAMS);

		std::vector<InputStream*> inStreams04(NB_STREAMS);
		std::vector<OutputStream*> outStreams04(NB_STREAMS);

		for (size_t i = 0; i < NB_STREAMS; i++)
		{
			inStreams01[i] = new InputStream01();
			outStreams01[i] = new OutputStream01();
			inStreams02[i] = new InputStream02();
			outStreams02[i] = new OutputStream02();
			inStreams03[i] = new InputStream03();
			outStreams03[i] = new OutputStream03();
			inStreams04[i] = new InputStream04();
			outStreams04[i] = new OutputStream04();
		}
#pragma endregion

		for (size_t i = 0; i < NB_TESTS; i++)
		{
			CreateFiles(NB_STREAMS);

			BenchmarkStream(inStreams01, outStreams01, chronos01, i, NB_STREAMS, BUFFER_SIZE, N);
			BenchmarkStream(inStreams02, outStreams02, chronos02, i, NB_STREAMS, BUFFER_SIZE, N);
			BenchmarkStream(inStreams03, outStreams03, chronos03, i, NB_STREAMS, BUFFER_SIZE, N);
			BenchmarkStream(inStreams04, outStreams04, chronos04, i, NB_STREAMS, BUFFER_SIZE, N);
		}

		BenchmarkResultsToCSV(chronos01, chronos02, chronos03, chronos04, NB_STREAMS, BUFFER_SIZE, N, "TEST_N");
	}
#pragma endregion

#pragma region Test_B
	for (size_t B : B_values)
	{
#pragma region Chrono initialisation
		Benchmarking* chronos03 = (Benchmarking*)malloc(sizeof(Benchmarking) * NB_TESTS * NB_STREAMS);
		Benchmarking* chronos04 = (Benchmarking*)malloc(sizeof(Benchmarking) * NB_TESTS * NB_STREAMS);

		for (size_t i = 0; i < NB_TESTS * NB_STREAMS; i++)
		{
			chronos03[i] = Benchmarking();
			chronos04[i] = Benchmarking();
		}
#pragma endregion

#pragma region Stream initialisation

		std::vector<InputStream*> inStreams03(NB_STREAMS);
		std::vector<OutputStream*> outStreams03(NB_STREAMS);

		std::vector<InputStream*> inStreams04(NB_STREAMS);
		std::vector<OutputStream*> outStreams04(NB_STREAMS);

		for (size_t i = 0; i < NB_STREAMS; i++)
		{
			inStreams03[i] = new InputStream03();
			outStreams03[i] = new OutputStream03();
			inStreams04[i] = new InputStream04();
			outStreams04[i] = new OutputStream04();
		}
#pragma endregion

		for (size_t i = 0; i < NB_TESTS; i++)
		{
			CreateFiles(NB_STREAMS);

			BenchmarkStream(inStreams03, outStreams03, chronos03, i, NB_STREAMS, B, NB_ELEMENTS);
			BenchmarkStream(inStreams04, outStreams04, chronos04, i, NB_STREAMS, B, NB_ELEMENTS);
		}

		BenchmarkResultsToCSV(NULL, NULL, chronos03, chronos04, NB_STREAMS, B, NB_ELEMENTS, "TEST_B");
	}
#pragma endregion

	testExternal();


	/*std::vector<std::string> streams = { filepathsRead[0] };
	Multiway multiwayTest (streams);
	multiwayTest.read(BUFFER_SIZE);
	multiwayTest.merge();
	multiwayTest.showRes();*/
	//

	std::system("pause");
	return 0;
}

void CreateFiles(size_t quantity = NB_STREAMS) {
#pragma region FileCreation
	printf("Starting file creation.\n");

	////////////////////////	Create the filepaths	////////////////////////////////////

	for (size_t i = 0; i < quantity; i++)
	{
		std::ostringstream stream;
		string root;
		if (isRelease)
			root = "C:\\project-release\\";
		else
			root = "C:\\project-tests\\";

		//Defines different paths according the OS
		if (_WIN32) {
			//_WIN32 works for both WIN32 and WIN64
			//!WARNING! A folder with the name 'project-tests' has to be created on C: drive

			stream.str("");
			stream << root << "exampleRead" << i << ".txt"; // todo maybe not use text but some binary file extension
			filepathsRead[i] = stream.str();
			for (std::size_t j = 0; j < 4; j++) {
				stream.str("");
				stream << root << "exampleWrite" << i << "_" << j << ".txt";
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
	for (size_t i = 0; i < quantity; i++)
	{
		new RandomFileCreation(filepathsRead[i], NB_ELEMENTS);
	}

#pragma endregion
}

void BenchmarkStream(std::vector<InputStream*> inStream, std::vector<OutputStream*> outStream, Benchmarking* chrono, size_t iteration, size_t k = NB_STREAMS, size_t B = BUFFER_SIZE, size_t N = NB_ELEMENTS) {
	printf("Starting iteration %d [k=%d, N=%d] with ", iteration, k, N);
	cout << inStream[0]->getName() << ".\n"; // Funfact: printf makes it crash with getName() and s%

	//open and create all streams
	for (size_t i = 0; i < k; i++) {
		inStream[i]->open(filepathsRead[i], B);
		outStream[i]->create(filepathsWrite[i * 4], B, N);
	}

	//read and write for all streams
	for (std::size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < k; j++) {
			if (!inStream[j]->end_of_stream()) {
				chrono[iteration + iteration * j].startTest();
				outStream[j]->write(inStream[j]->read_next());
				chrono[iteration + iteration * j].stopTest();
			}
		}
	}

	//close and reset all streams
	for (size_t i = 0; i < k; i++) {
		inStream[i]->reset();
		outStream[i]->close();
	}
}

void BenchmarkResultsToCSV(Benchmarking* chrono01, Benchmarking* chrono02, Benchmarking* chrono03, Benchmarking* chrono04, size_t k = NB_STREAMS, size_t B = BUFFER_SIZE, size_t N = NB_ELEMENTS, string descriptionWord = "") {
#pragma region Benchmarking - results

	printf("\"Printing\" the results.\n");

	std::fstream resultsFile;

	std::time_t current_time;
	struct tm time_info;
	time(&current_time);
	localtime_s(&time_info, &current_time);
	stringstream benchInfo;
	benchInfo << "N" << N << "_S" << k << "_B" << B << "_" << descriptionWord << "_"
		<< time_info.tm_hour << "h" << time_info.tm_min << "m" << time_info.tm_sec << "s_"
		<< (time_info.tm_mday) << "_" << (time_info.tm_mon + 1);

	string filename = "C:\\project-tests\\BenchResult_";
	filename += benchInfo.str();
	filename += ".csv";
	resultsFile.open(filename, std::ios::out | std::ios::trunc);

	resultsFile << "Method" << ";" << "Test#" << ";" << "Stream#" << ";" << "Highest" << ";" << "Lowest"
		<< ";" << "LastPerformance" << ";" << "AvgPerformance"
		<< ";" << "TotalTime" << "\n";
	for (size_t i = 0; i < NB_TESTS; i++)
	{
		for (size_t j = 0; j < k; j++)
		{
			if (chrono01 != NULL) {
				resultsFile << "1" << ";" << i << ";" << j << ";" << chrono01[i + i * j].getHighest() << ";" << chrono01[i + i * j].getLowest()
					<< ";" << chrono01[i + i * j].getLastPerformance() << ";" << chrono01[i + i * j].getAvgPerformance()
					<< ";" << chrono01[i + i * j].getTotalTime() << "\n";
			}
			if (chrono02 != NULL) {
				resultsFile << "2" << ";" << i << ";" << j << ";" << chrono02[i + i * j].getHighest() << ";" << chrono02[i + i * j].getLowest()
					<< ";" << chrono02[i + i * j].getLastPerformance() << ";" << chrono02[i + i * j].getAvgPerformance()
					<< ";" << chrono02[i + i * j].getTotalTime() << "\n";
			}
			if (chrono03 != NULL) {
				resultsFile << "3" << ";" << i << ";" << j << ";" << chrono03[i + i * j].getHighest() << ";" << chrono03[i + i * j].getLowest()
					<< ";" << chrono03[i + i * j].getLastPerformance() << ";" << chrono03[i + i * j].getAvgPerformance()
					<< ";" << chrono03[i + i * j].getTotalTime() << "\n";
			}
			if (chrono04 != NULL) {
				resultsFile << "4" << ";" << i << ";" << j << ";" << chrono04[i + i * j].getHighest() << ";" << chrono04[i + i * j].getLowest()
					<< ";" << chrono04[i + i * j].getLastPerformance() << ";" << chrono04[i + i * j].getAvgPerformance()
					<< ";" << chrono04[i + i * j].getTotalTime() << "\n";
			}
		}
	}

	resultsFile.close();

	printf("The results have been stored.\n");
#pragma endregion	
}

void testMemMapping(string filepathRead, string filepathWrite) {
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

		int32_t * memAddressIn = (int32_t*)regionIn.get_address();
		int32_t * memAddressOut = (int32_t*)regionOut.get_address();

		memcpy(memAddressOut, memAddressIn, buf.st_size / 4);
	}
	catch (const bi::interprocess_exception e)
	{
		e;
	}
}

void testExternal()
{
	Benchmarking chrono;
	std::size_t D_values[] = { 1, 32, 256, 1024 };
	std::size_t M_values[] = { 1, 64, 1024, 2048 };
	for (std::size_t m : M_values) {
		for (std::size_t d : D_values) {
			if (d <= m) {
				chrono.startTest();
				External external(filepathsRead[0], BUFFER_SIZE, 32);
				chrono.stopTest();
				std::cout << "External Multiway mergesort done in " << chrono.getLastPerformance() << "ms with parameters M = " << m << ", d = " << d << std::endl;
			}
		}
	}
	std::cout << "Average performance " << chrono.getAvgPerformance() << std::endl;
}