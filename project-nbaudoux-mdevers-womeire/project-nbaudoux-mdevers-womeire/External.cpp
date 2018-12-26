#include "pch.h"
#include "External.h"
#include "Multiway.h"
#include "InputStream03.h"
#include "OutputStream03.h"

External::External(std::string stream, std::size_t M, std::size_t d): _stream(stream), _M(M), _d(d)
{
	_id = 0;
	getFileSize();
	_nbStreams = (std::size_t) ceil(_N / _M);
	createStreams();
	merge();
}

void External::nextID()
{
	_streamID = std::to_string(++_id); //Giving IDs in order to not erase an existing file
}

std::string External::getAPath()
{
	std::string path = "C:\\project-tests\\sorted" + _streamID + ".txt";
	_queue_of_streams.push(path); //Saving reference to stream;
	nextID();
	return path;
}

void External::getFileSize()
{
	std::ifstream in(_stream, std::ifstream::ate | std::ifstream::binary);
	_N = in.tellg()/sizeof(int32_t);
}

void External::createStreams()
{
	InputStream03 stream;
	stream.open(_stream, bufferSize);
	
	for (std::size_t i = 0; i < _nbStreams; i++){
		//Sorting via a priority queue
		std::priority_queue<int, std::vector<int>, std::greater<int> > priority_queue;
		std::size_t j = 0;

		while (!stream.end_of_stream() and j < _M) {
			priority_queue.push(stream.read_next());
			j++;
		}

		std::string path = getAPath();

		OutputStream03 output; //Creating stream
		output.create(path, bufferSize);
		
		std::vector<int> res;
		while (!priority_queue.empty()) {
			res.push_back(priority_queue.top());
			priority_queue.pop();
		}

		output.write(&res[0]);
	}
}

void External::merge() 
{
	//While all integers not ordered in one single stream
	while (_queue_of_streams.size() > 1) {
		std::vector<std::string> streamsToOrder;
		std::size_t _dFirstStreams = std::min(_d, _queue_of_streams.size()); //If less than _d streams remaining
		for (std::size_t i = 0; i < _dFirstStreams; i++) {
			streamsToOrder.push_back(_queue_of_streams.front());
			_queue_of_streams.pop();
		}

		Multiway multiway(streamsToOrder);
		
		std::string path = getAPath();

		OutputStream03 output; //Creating stream
		output.create(path, bufferSize);
		std::vector<int> res = multiway.merge();
		output.write(&res[0]);
	}
}

External::~External()
{
}
