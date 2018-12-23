#include "pch.h"

#pragma once
class Benchmarking
{
	boost::chrono::steady_clock::time_point start, stop;
	boost::chrono::duration <double, boost::milli> diff;
public:
	Benchmarking();
	void startTest();
	void stopTest();
	boost::chrono::duration<double, boost::milli> getPerformance();
	~Benchmarking();
};

