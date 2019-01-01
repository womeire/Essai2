#include "pch.h"
#include "InputStream03.h"

InputStream03::InputStream03()
{
	filePointer = NULL;
}

InputStream03::~InputStream03()
{
	fclose(filePointer);
}

/*Tries to open a file with the given filepath (read-only). Returns the filehandle as an int*/
void InputStream03::open(string filepath, int bfSize)
{
	if (filePointer != NULL) {
		printf("Stream already in use.\n");
		return;
	}
	bufferSize = bfSize;

	if (sizeof(int32_t)*bufferSize >= _HEAP_MAXREQ)
		printf("Buffer size is too large for malloc"); // error will be catched later when initialising buffer

	buffer = (int32_t*)malloc(sizeof(int32_t) * bufferSize);

	char filepathChar[_MAX_PATH];
	strcpy_s(filepathChar, filepath.c_str());

	errno_t error = fopen_s(&filePointer, filepathChar, "rb");
	if (error != 0) {
		printf("Error opening the stream: %d \n \"%s\"\n", error, filepathChar);
		return;
	}
}

int32_t* InputStream03::read_next()
{
	if (filePointer == NULL) {
		printf("Invalid file or file not opened yet. \n");
		return NULL;
	}

	if (!end_of_stream()) {
		std::size_t amountRead = fread(buffer, sizeof(int32_t), bufferSize, filePointer);
		if (amountRead != bufferSize) {
			std::vector<int32_t> res;
			for (std::size_t i = amountRead; i < bufferSize; i++)
				buffer[i] = NULL;
		}
		return buffer;
	}
	printf("Trying to read beyond end of file. \n");
	return NULL;
}

bool InputStream03::end_of_stream()
{
	return feof(filePointer);
}
