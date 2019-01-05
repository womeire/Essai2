#include "pch.h"
#include "OutputStream03.h"


OutputStream03::OutputStream03()
{
	filePointer = NULL;
}


OutputStream03::~OutputStream03()
{
	close();
}

void OutputStream03::create(string filepath, size_t bufSize, size_t fileSize)
{
	if (filePointer != NULL) {
		printf("Stream already in use.\n");
		return;
	}
	bufferSize = bufSize;

	char filepathChar[_MAX_PATH];
	strcpy_s(filepathChar, filepath.c_str());

	errno_t error = fopen_s(&filePointer, filepathChar, "wb"); // "w" -> Opens an empty file for writing.If the file exists, its contents are destroyed.
	if (error != 0) {
		printf("Error opening the stream: %d \n \"%s\"\n", error, filepathChar);
		return;
	}
}

void OutputStream03::write(int32_t* element)
{
	if (filePointer == NULL) {
		printf("File not yet created. Call the create function first.\n");
		return;
	}

	fwrite(element, sizeof(int32_t), bufferSize, filePointer);
}

void OutputStream03::close()
{
	if (filePointer != NULL) {
		fclose(filePointer);
		filePointer = NULL;
	}
}