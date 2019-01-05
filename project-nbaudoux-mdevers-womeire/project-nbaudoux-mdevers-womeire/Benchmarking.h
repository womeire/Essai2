#include "pch.h"

#pragma once
class Benchmarking
{
	boost::chrono::steady_clock::time_point start, stop;
	boost::chrono::duration <double, boost::milli> diff;
	double average, highest, lowest, total;
	std::size_t nbObs,nbLowest;
	bool started;
public:
	Benchmarking();
	void reset();
	void startTest();
	void stopTest();
	double getLowest();
	double getHighest();
	double getLastPerformance();
	double getAvgPerformance();
	double getTotalTime();
	std::size_t getNbLowest();
	~Benchmarking();
private:
	void setLowest(double);
	void setHighest(double);
};

