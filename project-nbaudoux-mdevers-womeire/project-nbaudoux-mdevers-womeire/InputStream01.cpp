#include "pch.h"
#include "InputStream01.h"


InputStream01::InputStream01()
{
	fileHandle = NULL;
}


InputStream01::~InputStream01()
{
	if (fileHandle != NULL) {
		_close(fileHandle);
		fileHandle = NULL;
	}
}

/*Tries to open a file with the given filepath (read-only). Returns the filehandle as an int*/
void InputStream01::open(string filepath, size_t bufSize)
{
	if (fileHandle != NULL && end_of_stream()) { // if a new file needs to be opened when a previous one has finished
		_close(fileHandle);
		fileHandle = NULL;
	}

	if (fileHandle != NULL) {
		printf("This InputStream01 already opened a file.\n");
		return;
	}

	char fp[_MAX_PATH];
	strcpy_s(fp, filepath.c_str());

	errno_t error = _sopen_s(&fileHandle, fp, _O_RDONLY | _O_SEQUENTIAL | _O_BINARY, _SH_DENYWR, _S_IREAD);
	if (error != 0) {
		printf("Error opening the stream: %d %d\n", error, fileHandle);
		return;
	}
}

int32_t* InputStream01::read_next()
{
	if (fileHandle < 0) {
		printf("Invalid file handle value.\n");
		return NULL;
	}

	_read(fileHandle, element, sizeof(int32_t));

	return element;
}

bool InputStream01::end_of_stream()
{
	return _eof(fileHandle);
}

void InputStream01::reset() {
	if (fileHandle != NULL) {
		_close(fileHandle);
		fileHandle = NULL;
	}
}

string InputStream01::getName() {
	return "Stream01";
}


