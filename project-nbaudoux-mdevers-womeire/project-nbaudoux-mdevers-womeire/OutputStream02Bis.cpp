#include "pch.h"
#include "OutputStream02Bis.h"


OutputStream02Bis::OutputStream02Bis()
{
	filePointer = NULL;
}


OutputStream02Bis::~OutputStream02Bis()
{
	close();
}

void OutputStream02Bis::create(string filepath, int bfSize)
{
	if (filePointer != NULL) {
		printf("Stream already in use.\n");
		return;
	}
	bufferSize = bfSize;

	char filepathChar[_MAX_PATH];
	strcpy_s(filepathChar, filepath.c_str());

	errno_t error = fopen_s(&filePointer, filepathChar, "wb"); // "w" -> Opens an empty file for writing.If the file exists, its contents are destroyed.
	if (error != 0) {
		printf("Error opening the stream: %d \n \"%s\"\n", error, filepathChar);
		return;
	}
}

void OutputStream02Bis::write(int32_t* element)
{
	if (filePointer == NULL) {
		printf("File not yet created. Call the create function first.\n");
		return;
	}

	fwrite(element, sizeof(int32_t), bufferSize, filePointer);
}

void OutputStream02Bis::close()
{
	if (filePointer != NULL) {
		fclose(filePointer);
		filePointer = NULL;
	}
}