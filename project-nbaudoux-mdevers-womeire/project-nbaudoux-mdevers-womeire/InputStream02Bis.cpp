#include "pch.h"
#include "InputStream02Bis.h"

InputStream02Bis::InputStream02Bis()
{
	filePointer = NULL;
}

InputStream02Bis::~InputStream02Bis()
{
	fclose(filePointer);
}

/*Tries to open a file with the given filepath (read-only). Returns the filehandle as an int*/
void InputStream02Bis::open(string filepath, int bfSize)
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

int32_t* InputStream02Bis::read_next()
{
	if (filePointer == NULL) {
		printf("Invalid file or file not opened yet. \n");
		return NULL;
	}

	if (!end_of_stream()) {
		int amountRead = fread(buffer, sizeof(int32_t), bufferSize, filePointer);
		if (amountRead != bufferSize) {
			//todo do something here!!!
		}
		return buffer;
	}
	printf("Trying to read beyond end of file. \n");
	return NULL;
}

bool InputStream02Bis::end_of_stream()
{
	return feof(filePointer);
}
