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


const std::size_t NB_TESTS = 10;
const std::size_t NB_STREAMS = 30;
const int BUFFER_SIZE = 65536; //65536 = page_size of memory mapping
const std::size_t NB_ELEMENTS = BUFFER_SIZE * 10; // Should not exceed capacity of size_t (= 4294967295)!

string filepathsRead[NB_STREAMS];
string filepathsWrite[NB_STREAMS * 4];


InputStream01 inStreams01[NB_STREAMS];
OutputStream01 outStreams01[NB_STREAMS];

InputStream02 inStreams02[NB_STREAMS];
OutputStream02 outStreams02[NB_STREAMS];

InputStream03 inStreams03[NB_STREAMS];
OutputStream03 outStreams03[NB_STREAMS];

InputStream04 inStreams04[NB_STREAMS];
OutputStream04 outStreams04[NB_STREAMS];

void testStream01(string filepathRead, string filepathWrite);
void testStream02(string filepathRead, string filepathWrite);
void testStream03(string filepathRead, string filepathWrite);
void testStream04(string filepathRead, string filepathWrite);
void testMemMapping(string filepathRead, string filepathWrite);

void CreateFiles(int);
void BenchmarkStream01(Benchmarking*, int, int, int);
void BenchmarkStream02(int);
void BenchmarkStream03(int);
void BenchmarkStream04(int);
void BenchmarkResultsToCSV(Benchmarking*, int, int);

int main()
{
	size_t K_values[] = { 1, 2, 5/*, 10, 30 */}; // Our test system can go up to 512 simultaneous streams (but asked to do max 30)
	size_t N_values[] = { 1, 1024, 1024 * 1024, 1024 * 1024 * 1024, 1024 * 1024 * 1024 * 3 }; // Should not exceed capacity of size_t (= 4294967295)!
	size_t B_values[] = {1, 1024, 65536, 1024 * 1024, 65536 * 1000}; // Multiples of 65536 are good for stream04 as it's the page_size

#pragma region Test_k
	
	for(size_t k: K_values)
	{
		Benchmarking StreamChronos01[NB_STREAMS * NB_TESTS];
		Benchmarking StreamChronos02[NB_STREAMS * NB_TESTS];
		Benchmarking StreamChronos03[NB_STREAMS * NB_TESTS];
		Benchmarking StreamChronos04[NB_STREAMS * NB_TESTS];

		for (size_t i = 0; i < NB_TESTS; i++)
		{
			CreateFiles(k);
			BenchmarkStream01(StreamChronos01, i, k, NB_ELEMENTS);
			/*BenchmarkStream02(i);
			BenchmarkStream03(i);
			BenchmarkStream04(i);*/
		}

		BenchmarkResultsToCSV(StreamChronos01, k, NB_ELEMENTS);
	}
#pragma endregion

#pragma region Test_N

#pragma endregion

#pragma region Test_B

#pragma endregion



	/*for (size_t i = 0; i < NB_TESTS; i++)
	{
		CreateFiles();
		BenchmarkStream01(i);
		BenchmarkStream02(i);
		BenchmarkStream03(i);
		BenchmarkStream04(i);
	}

	BenchmarkResultsToCSV();*/

	/*std::vector<std::string> streams = { filepathsRead[0] };
	Multiway multiwayTest (streams);
	multiwayTest.read(BUFFER_SIZE);
	multiwayTest.merge();
	multiwayTest.showRes();*/
	//

	std::system("pause");
	return 0;
}

void CreateFiles(int quantity = NB_STREAMS) {
#pragma region FileCreation
	printf("Starting file creation.\n");

	////////////////////////	Create the filepaths	////////////////////////////////////
	
	for (size_t i = 0; i < quantity; i++)
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
	for (size_t i = 0; i < quantity; i++)
	{
		new RandomFileCreation(filepathsRead[i], NB_ELEMENTS);
	}

#pragma endregion
}

void BenchmarkStream01(Benchmarking* chrono , int iteration, int k = NB_STREAMS, int N = NB_ELEMENTS) {
	printf("Starting iteration %d of stream01 [k=%d, N=%d].\n", iteration, k, N);

	//open and create all streams
	for (size_t i = 0; i < k; i++) {
		inStreams01[i].open(filepathsRead[i]);
		outStreams01[i].create(filepathsWrite[i * 4]);
	}

	//read and write for all streams
	for (std::size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < k; j++) {
			if (!inStreams01[j].end_of_stream()) {
				chrono[j * iteration + j].startTest();
				outStreams01[j].write(inStreams01[j].read_next());
				chrono[j * iteration + j].stopTest();
			}
		}
	}

	//close and reset all streams
	for (size_t i = 0; i < k; i++) {
		inStreams01[i].reset();
		outStreams01[i].close();
	}
}

//void BenchmarkStream02(int iteration) {
//	printf("Starting iteration %d of stream02.\n", iteration);
//
//	//open and create all streams
//	for (size_t i = 0; i < NB_STREAMS; i++) {
//		inStreams02[i].open(filepathsRead[i]);
//		outStreams02[i].create(filepathsWrite[i * 4]);
//	}
//
//	//read and write for all streams
//	for (std::size_t i = 0; i < NB_ELEMENTS; i++) {
//		for (size_t j = 0; j < NB_STREAMS; j++) {
//			if (!inStreams02[j].end_of_stream()) {
//				StreamChronos02[j * iteration + j].startTest();
//				outStreams02[j].write(inStreams02[j].read_next());
//				StreamChronos02[j * iteration + j].stopTest();
//			}
//		}
//	}
//
//	//close and reset all streams
//	for (size_t i = 0; i < NB_STREAMS; i++) {
//		inStreams02[i].reset();
//		outStreams02[i].close();
//	}
//}
//
//void BenchmarkStream03(int iteration) {
//	printf("Starting iteration %d of stream03.\n", iteration);
//
//	//open and create all streams
//	for (size_t i = 0; i < NB_STREAMS; i++) {
//		inStreams03[i].open(filepathsRead[i], BUFFER_SIZE);
//		outStreams03[i].create(filepathsWrite[i * 4], BUFFER_SIZE);
//	}
//
//	//read and write for all streams
//	for (std::size_t i = 0; i < NB_ELEMENTS; i++) {
//		for (size_t j = 0; j < NB_STREAMS; j++) {
//			if (!inStreams03[j].end_of_stream()) {
//				StreamChronos03[j * iteration + j].startTest();
//				outStreams03[j].write(inStreams03[j].read_next());
//				StreamChronos03[j * iteration + j].stopTest();
//			}
//		}
//	}
//
//	//close and reset all streams
//	for (size_t i = 0; i < NB_STREAMS; i++) {
//		inStreams03[i].reset();
//		outStreams03[i].close();
//	}
//}
//
//void BenchmarkStream04(int iteration) {
//	printf("Starting iteration %d of stream04.\n", iteration);
//
//	//open and create all streams
//	for (size_t i = 0; i < NB_STREAMS; i++) {
//		inStreams04[i].open(filepathsRead[i], BUFFER_SIZE);
//		outStreams04[i].create(filepathsWrite[i * 4], filepathsRead[i], BUFFER_SIZE);
//	}
//
//	//read and write for all streams
//	for (std::size_t i = 0; i < NB_ELEMENTS; i++) {
//		for (size_t j = 0; j < NB_STREAMS; j++) {
//			if (!inStreams04[j].end_of_stream()) {
//				StreamChronos04[j * iteration + j].startTest();
//				outStreams04[j].write(inStreams04[j].read_next());
//				StreamChronos04[j * iteration + j].stopTest();
//			}
//		}
//	}
//
//	//close and reset all streams
//	for (size_t i = 0; i < NB_STREAMS; i++) {
//		inStreams04[i].reset();
//		outStreams04[i].close();
//	}
//}

void BenchmarkResultsToCSV(Benchmarking* chrono01, int k = NB_STREAMS, int N = NB_ELEMENTS){
	#pragma region Benchmarking - results

	printf("\"Printing\" the results.\n");

	std::fstream resultsFile;

	std::time_t current_time;
	struct tm time_info;
	time(&current_time);
	localtime_s(&time_info, &current_time);
	stringstream benchInfo;
	benchInfo << "N" << N << "_S" << k << "_B" << BUFFER_SIZE << "_"
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
			resultsFile << "1" << ";" << i << ";" << j << ";" << chrono01[j + i * j].getHighest() << ";" << chrono01[j + i * j].getLowest()
				<< ";" << chrono01[j + i * j].getLastPerformance() << ";" << chrono01[j + i * j].getAvgPerformance()
				<< ";" << chrono01[j + i * j].getTotalTime() << "\n";

			/*resultsFile << "2" << ";" << i << ";" << j << ";" << StreamChronos02[j + i * j].getHighest() << ";" << StreamChronos02[j + i * j].getLowest()
				<< ";" << StreamChronos02[j + i * j].getLastPerformance() << ";" << StreamChronos02[j + i * j].getAvgPerformance()
				<< ";" << StreamChronos02[j + i * j].getTotalTime() << "\n";

			resultsFile << "3" << ";" << i << ";" << j << ";" << StreamChronos03[j + i * j].getHighest() << ";" << StreamChronos03[j + i * j].getLowest()
				<< ";" << StreamChronos03[j + i * j].getLastPerformance() << ";" << StreamChronos03[j + i * j].getAvgPerformance()
				<< ";" << StreamChronos03[j + i * j].getTotalTime() << "\n";

			resultsFile << "4" << ";" << i << ";" << j << ";" << StreamChronos04[j + i * j].getHighest() << ";" << StreamChronos04[j + i * j].getLowest()
				<< ";" << StreamChronos04[j + i * j].getLastPerformance() << ";" << StreamChronos04[j + i * j].getAvgPerformance()
				<< ";" << StreamChronos04[j + i * j].getTotalTime() << "\n";*/
		}
	}

	resultsFile.close();

	printf("The results have been stored.\n");
#pragma endregion	
}


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

void testStream04(string filepathRead, string filepathWrite) {
	InputStream04 inStream4;
	inStream4.open(filepathRead, BUFFER_SIZE);

	OutputStream04 outStream4;
	outStream4.create(filepathWrite, filepathRead, BUFFER_SIZE);

	for (int i = 0; !inStream4.end_of_stream(); i++)
	{
		outStream4.write(inStream4.read_next());
	};

	outStream4.close();
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