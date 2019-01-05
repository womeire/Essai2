#include <string>
#include "pch.h"
using namespace std;

#pragma once
class OutputStream
{
public:
	OutputStream();
	virtual ~OutputStream() = 0;
	virtual void create(string, size_t, size_t = 0) = 0;
	virtual void write(int32_t*) = 0;
	virtual void close() = 0;
};