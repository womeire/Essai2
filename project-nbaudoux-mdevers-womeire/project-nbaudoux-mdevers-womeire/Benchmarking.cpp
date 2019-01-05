#include "pch.h"
#include "Benchmarking.h"


Benchmarking::Benchmarking()
{
	reset();
}


Benchmarking::~Benchmarking()
{
}

void Benchmarking::reset()
{
	average = 0;
	nbObs = 0;
	highest = 0;
	total = 0;
	lowest = 9999;
	nbLowest = 0;
	started = false;
}

void Benchmarking::startTest()
{
	if (!started) {
		start = boost::chrono::steady_clock::now();
		started = true;
	}
}

void Benchmarking::stopTest()
{
	if (started) {
		stop = boost::chrono::steady_clock::now();
		started = false;
		diff = stop - start;
		nbObs++;
		average += diff.count();
		total += diff.count();
		setLowest(diff.count());
		setHighest(diff.count());
	}
	else {
		std::cout << "Test not started yet" << std::endl;
	}
}

void Benchmarking::setLowest(double val) {
	if (val == lowest) {
		nbLowest++;
	}else if (val < lowest) {
		lowest = val;
		nbLowest = 0;
	}
}

void Benchmarking::setHighest(double val) {
	if (val > highest)
		highest = val;
}

std::size_t Benchmarking::getNbLowest() {
	return nbLowest;
}

double Benchmarking::getLowest() {
	return lowest;
}

double Benchmarking::getHighest() {
	return highest;
}

double Benchmarking::getLastPerformance()
{
	if (started) {
		std::cout << "Be aware that a test is currently running. The displayed value, is the value of the previous observation" << std::endl;
	}
	return diff.count();
}

double Benchmarking::getAvgPerformance()
{
	double res = average / nbObs;
	return res;
}

double Benchmarking::getTotalTime()
{
	return total;
}
