#include "pch.h"
#include "InputStream02.h"

InputStream02::InputStream02()
{
	filePointer = NULL;
}

InputStream02::~InputStream02()
{
	if (filePointer != NULL) {
		fclose(filePointer);
		filePointer = NULL;
	}
	free(elementP);
}

/*Tries to open a file with the given filepath (read-only). Returns the filehandle as an int*/
void InputStream02::open(string filepath, size_t bufSize)
{
	if (filePointer != NULL) {
		printf("Stream already in use.\n");
		return;
	}

	elementP = (int32_t*)malloc(sizeof(int32_t)*1);

	char filepathChar[_MAX_PATH];
	strcpy_s(filepathChar, filepath.c_str());

	errno_t error = fopen_s(&filePointer, filepathChar, "rb");
	if (error != 0) {
		printf("Error opening the stream: %d \n \"%s\"\n", error, filepathChar);
		return;
	}
}

int32_t* InputStream02::read_next()
{
	if (filePointer == NULL) {
		printf("Invalid file or file not opened yet. \n");
		return NULL;
	}

	if (!end_of_stream()) {
		fread(elementP, sizeof(int32_t), 1, filePointer);
		return elementP;
	}
	printf("Trying to read beyond end of file. \n");
	return NULL;
}

bool InputStream02::end_of_stream()
{
	return feof(filePointer);
}

void InputStream02::reset() {
	if (filePointer != NULL) { 
		fclose(filePointer);
		filePointer = NULL;
	}
}

string InputStream02::getName()
{
	return "Stream02";
}
