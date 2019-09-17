#include "signal.hpp"

Signal::Signal(unsigned int sampleRate)
{
	sampleRate_ = sampleRate;
	count = 0;
}

unsigned int Signal::getSampleRate()
{
	return sampleRate_;
}

void Signal::normalize()
{
	std::size_t i = 0;
	while (i < size()) {
		samples[i] = (int16_t)samples[i];
		i++;
	}
}

void Signal::push_back(double value)
{
	samples.push_back(value);
	count++;
}

std::size_t Signal::size()
{
	return count;
}

double Signal::operator[](std::size_t index)
{
	return samples.at(index);
}