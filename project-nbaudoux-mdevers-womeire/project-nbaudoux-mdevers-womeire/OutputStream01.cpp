#include "pch.h"
#include "OutputStream01.h"


OutputStream01::OutputStream01()
{
	fileHandle = -1;
}


OutputStream01::~OutputStream01()
{
	close();
}

void OutputStream01::create(string filepath) // todo for the moment does overwrite as far as it writes data -> previous files can still be visible -> erase file if already exists
{
	if (fileHandle != -1) {
		printf("Stream already in use by file handle %d\n", fileHandle);
		return;
	}

	char fp[_MAX_PATH];
	strcpy_s(fp, filepath.c_str());

	errno_t error = _sopen_s(&fileHandle, fp, _O_CREAT | _O_WRONLY | _O_BINARY, _SH_DENYWR, _S_IWRITE);
	if (error != 0) {
		printf("Error opening the stream: %d %d\n", error, fileHandle);
		return;
	}
}

void OutputStream01::write(int32_t element)
{
	if (fileHandle < 0) {
		printf("File not yet created. Call the create function first.");
		return; // Todo real error?
	}

	uint8_t input[sizeof(int32_t)];
	input[0] = element;
	input[1] = element >> 8;
	input[2] = element >> 16;
	input[3] = element >> 24;
	//std::cout << "Writing : " << input << std::endl; //Test
	_write(fileHandle, input, sizeof(int32_t));
}

void OutputStream01::close()
{
	_close(fileHandle);
	fileHandle = -1;
}
