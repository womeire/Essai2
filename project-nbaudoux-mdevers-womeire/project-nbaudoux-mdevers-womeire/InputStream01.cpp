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
void InputStream01::open(string filepath)
{
	if (fileHandle != NULL) {
		printf("This InputStream01 already opened a file");
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

int32_t InputStream01::read_next()
{
	if (fileHandle < 0) {
		printf("Invalid file handle value");
		return NULL;
	}

	uint8_t elements[sizeof(int32_t)];
	_read(fileHandle, elements, sizeof(int32_t));

	return elements[3] << 24 | elements[2] << 16 | elements[1] << 8 | elements[0];
}

bool InputStream01::end_of_stream()
{
	return _eof(fileHandle);
}
