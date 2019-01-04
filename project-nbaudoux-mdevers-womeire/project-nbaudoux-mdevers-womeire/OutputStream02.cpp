#include "pch.h"
#include "OutputStream02.h"


OutputStream02::OutputStream02()
{
	filePointer = NULL;
}


OutputStream02::~OutputStream02()
{
	close();
}

void OutputStream02::create(string filepath)
{
	if (filePointer != NULL) {
		printf("Stream already in use.\n");
		return;
	}

	char filepathChar[_MAX_PATH];
	strcpy_s(filepathChar, filepath.c_str());

	errno_t error = fopen_s(&filePointer, filepathChar, "wb"); // "w" -> Opens an empty file for writing.If the file exists, its contents are destroyed.
	if (error != 0) {
		printf("Error opening the stream: %d \n \"%s\"\n", error, filepathChar);
		return;
	}
}

void OutputStream02::write(int32_t* element)
{
	if (filePointer == NULL) {
		printf("File not yet created. Call the create function first.\n");
		return;
	}

	fwrite(element, sizeof(int32_t), 1, filePointer);
}

void OutputStream02::close()
{
	if (filePointer != NULL) {
		fclose(filePointer);
		filePointer = NULL;
	}
}