#include "pch.h"

#pragma once
class Benchmarking
{
	boost::chrono::steady_clock::time_point start, stop;
	boost::chrono::duration <double, boost::milli> diff;
	double average, highest, lowest;
	std::size_t nbObs;
	bool started;
public:
	Benchmarking();
	void startTest();
	void stopTest();
	void setLowest(double);
	void setHighest(double);
	double getLowest();
	double getHighest();
	double getLastPerformance();
	double getAvgPerformance();
	~Benchmarking();
};

