#include "pch.h"
#include "Multiway.h"


Multiway::Multiway(std::vector<std::string> streams): _streams(streams)
{
}

void Multiway::read()
{
	_content.reserve(65000);
	for (std::size_t i = 0; i < _streams.size(); i++) {
		//Adding one vector per stream
		_content.push_back({});
		std::cout << _content.at(i).capacity() << std::endl;
		_content.at(i).reserve(1000);
		std::cout << _content.at(i).capacity() << std::endl;
		int bufferSize = 18;

		InputStream03 inStream3;
		inStream3.open(_streams.at(i), bufferSize);

		int32_t * buffer = (int32_t*)malloc(sizeof(int32_t) * bufferSize);
		int relativePosition = 0;

		for (std::size_t j = 0; !inStream3.end_of_stream(); j++) {
			std::cout << "Outer loop No" << i << " Inner loop No" << j << std::endl;
			relativePosition = j % bufferSize;
			if (j > 0 && relativePosition == 0) {
				for (std::size_t k = 0; k < bufferSize; k++)
					_content.at(i).push_back(buffer[k]);
			}
			
			buffer[relativePosition] = inStream3.read_next();
		}
		if (relativePosition > 0) { // the end of file is not necessary reached on a multiple op bufferSize, thus a final write is necessary
			for (std::size_t k = 0; k < relativePosition; k++)
				_content.at(i).push_back(buffer[k]);
		}
	}
	std::cout << "Read done" << std::endl;
}

void Multiway::merge()
{
	for (int i = 0; i < _content.size(); i++) {
		std::vector<int> myvect = { _content.at(i).at(0), i };
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
}

void Multiway::showRes() {
	for (std::size_t i = 0; i < _sorted.size(); i++)
		std::cout << _sorted.at(i) << std::endl;
}

Multiway::~Multiway()
{
}
