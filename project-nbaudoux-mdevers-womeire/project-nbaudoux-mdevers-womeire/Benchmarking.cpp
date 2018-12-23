#include "pch.h"
#include "Benchmarking.h"


Benchmarking::Benchmarking()
{
}


Benchmarking::~Benchmarking()
{
}

void Benchmarking::startTest()
{
	start = boost::chrono::steady_clock::now();
}

void Benchmarking::stopTest()
{
	stop = boost::chrono::steady_clock::now();
	diff = stop - start;
}

boost::chrono::duration<double, boost::milli> Benchmarking::getPerformance()
{
	return diff;
}
