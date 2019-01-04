#include "pch.h"
#include "Multiway.h"


Multiway::Multiway(std::vector<std::string> streams): _streams(streams)
{
}

void Multiway::read(int bufSize)
{
	_content.reserve(bufSize);
	for (std::size_t i = 0; i < _streams.size(); i++) {
		//Adding one vector per stream
		_content.push_back({});
		_content.at(i).reserve(bufSize);
		int bufferSize = bufSize;

		InputStream03 inStream3;
		inStream3.open(_streams.at(i), bufferSize);

		int32_t * buffer = (int32_t*)malloc(sizeof(int32_t) * bufferSize);

		while (!inStream3.end_of_stream()) {
			buffer = inStream3.read_next();

			for (std::size_t k = 0; k < bufferSize; k++)
				_content.at(i).push_back(buffer[k]);
		}
	}
}

std::vector<int> Multiway::merge()
{
	for (int i = 0; i < _content.size(); i++) {
		std::vector<int32_t> myvect = { _content.at(i).at(0), i };
		_queue.push(myvect);
	}
	
	int min;
	while (!_queue.empty()) {
		_sorted.push_back(_queue.top().at(0));
		min = _queue.top().at(1); //Position of min value in _streams
		_queue.pop();
		_content.at(min).erase(_content.at(min).begin()); //Deletes the first element of the sublist

		//Insert the first integer of the minimum stream if it is not empty
		if (!_content.at(min).empty()) {
			_queue.push({ _content.at(min).at(0), min });
		}
	}

	return _sorted;
}

void Multiway::showRes() {
	for (std::size_t i = 0; i < _sorted.size(); i++)
		std::cout << _sorted.at(i) << std::endl;
}

Multiway::~Multiway()
{
}
