#include "pch.h"
#include "InputStream02.h"

InputStream02::InputStream02()
{
	filePointer = NULL;
	fileSize_32 = 0;
	currentPos = -1;
}

InputStream02::~InputStream02()
{
	fclose(filePointer);
}

/*Tries to open a file with the given filepath (read-only). Returns the filehandle as an int*/
void InputStream02::open(string filepath)
{
	if (filePointer != NULL) {
		printf("Stream already in use.\n");
		return;
	}

	char filepathChar[_MAX_PATH];
	strcpy_s(filepathChar, filepath.c_str());

	errno_t error = fopen_s(&filePointer, filepathChar, "rb");
	if (error != 0) {
		printf("Error opening the stream: %d \n \"%s\"\n", error, filepathChar);
		return;
	}

	// obtain file size:
	fseek(filePointer, 0, SEEK_END); // SEEK_END : Library implementations are allowed to not meaningfully support SEEK_END (therefore, code using it has no real standard portability).
	fileSize_32 = ftell(filePointer) / 4; // divided by 4 because ftell returns the number of 8 bit units
	rewind(filePointer);

	// allocate memory to buffer to contain the whole file:
	buffer = (int32_t*)malloc(sizeof(int32_t)*fileSize_32);
	if (buffer == NULL) {
		printf("Error allocating memory to buffer.\n");
		filePointer = NULL;
		return;
	}
}

int32_t InputStream02::read_next()
{
	if (filePointer == NULL) {
		printf("Invalid file or file not opened yet. \n");
		return NULL;
	}

	if (currentPos < 0) {
		fread(buffer, sizeof(int32_t), fileSize_32, filePointer);
	}

	currentPos++;

	if (currentPos >= fileSize_32) {
		return NULL;
	}

	return buffer[currentPos];
}

bool InputStream02::end_of_stream()
{
	if (filePointer == NULL) {
		printf("Invalid file or file not opened yet. \n");
		return true; 
	}
	return currentPos == fileSize_32;
}
