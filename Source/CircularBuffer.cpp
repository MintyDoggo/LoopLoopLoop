#include "CircularBuffer.h"
#include <JuceHeader.h>
#include "Utilities.h"
#include <cmath>

CircularBuffer::CircularBuffer()
{
	writeIndex = 0;
	grains.resize(128);
	userSize = secondsToSamples(2.0);
	buffer.resize(secondsToSamples(10.0));
	writePause = false;
	userNumGrains = 1;
}

void CircularBuffer::write(double sample)
{
	if (!writePause)
	{
		buffer[writeIndex] = sample;
		writeIndex = (writeIndex + 1) % userSize;
	}
}

double CircularBuffer::read(double index)
{
	// wrap index if out of bounds
	wrapReadIndexToBuffer(index, userSize);

	const int firstIndex = int(std::floor(index));
	const int nextIndex = (firstIndex + 1);
	//interpolation
	const double sample = linearInterpolation(buffer[firstIndex], buffer[nextIndex], index - firstIndex); // index - firstindex is incorrect when index out of bounds
	return sample;
}

void CircularBuffer::setSize(double sizeInSeconds)
{
	buffer.resize(secondsToSamples(sizeInSeconds));
}

int CircularBuffer::getSize()
{
	return buffer.size();
}

void CircularBuffer::setUserSize(double size)
{
	userSize = secondsToSamples(size);
}

double CircularBuffer::getUserSizeInSeconds()
{
	return samplesToSeconds(userSize);
}

double CircularBuffer::getProgress()
{
	return double(writeIndex) / double(userSize);
}

void CircularBuffer::clear()
{
	for (int i = 0; i < userSize; i++)
	{
		buffer[i] = 0;
	}
}
