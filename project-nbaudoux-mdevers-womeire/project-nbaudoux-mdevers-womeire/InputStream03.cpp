#include "pch.h"
#include "InputStream03.h"


InputStream03::InputStream03()
{
	filePointer = NULL;
	fileSize_32 = 0;
	currentPos = -1;
}


InputStream03::~InputStream03()
{
	printf("File closed.\n");
	fclose(filePointer);
}

/*Tries to open a file with the given filepath (read-only). Returns the filehandle as an int*/
void InputStream03::open(string filepath, int bufSize)
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

	printf("File \"%s\" opened successfully.\n", filepathChar);

	// obtain file size:
	fseek(filePointer, 0, SEEK_END); // SEEK_END : Library implementations are allowed to not meaningfully support SEEK_END (therefore, code using it has no real standard portability).
	fileSize_32 = ftell(filePointer) / 4; // divided by 4 because ftell returns the number of 8 bit units
	rewind(filePointer);

	bufferSize = bufSize;

	// allocate memory to buffer to contain the whole file:
	buffer = (int32_t*)malloc(sizeof(int32_t)*bufferSize);
	if (buffer == NULL) {
		printf("Error allocating memory to buffer.\n");
		filePointer = NULL;
		return;
	}
}

int32_t InputStream03::read_next()
{
	if (filePointer == NULL) {
		printf("Invalid file or file not opened yet. \n");
		return -1; // Todo not good as -1 can be a correct value for this function... but everything from min to max is...
	}

	if (++currentPos % bufferSize == 0) {
		fread(buffer, sizeof(int32_t), bufferSize, filePointer);
	}

	//currentPos++;

	if (currentPos >= fileSize_32) {
		return -1; //todo same remark as just above
	}

	return buffer[currentPos % bufferSize];
}

bool InputStream03::end_of_stream()
{
	if (filePointer == NULL) {
		printf("Invalid file or file not opened yet. \n");
		return true; //todo return true or false?
	}
	return currentPos == fileSize_32;
}
