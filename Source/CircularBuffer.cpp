#include "CircularBuffer.h"
#include <JuceHeader.h>
#include "Utilities.h"
#include <cmath>

CircularBuffer::CircularBuffer()
{
	writeIndex = 0;
	grains.resize(32);
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

int CircularBuffer::getSize()
{
	return buffer.size();
}

void CircularBuffer::setSize(double sizeInSeconds)
{
	buffer.resize(secondsToSamples(sizeInSeconds));
}

double CircularBuffer::getUserSizeInSeconds()
{
	return samplesToSeconds(userSize);
}

void CircularBuffer::setUserSize(double size)
{
	userSize = secondsToSamples(size);
}

void CircularBuffer::clear()
{
	for (int i = 0; i < userSize; i++)
	{
		buffer[i] = 0;
	}
}

int CircularBuffer::getWriteIndex()
{
	return writeIndex;
}

int CircularBuffer::getNumGrains()
{
	return grains.size();
}

double CircularBuffer::getProgress()
{
	return double(writeIndex) / double(userSize);
}

void CircularBuffer::setUserNumGrains(int numGrains)
{
	userNumGrains = numGrains;
}

int CircularBuffer::getUserNumGrains()
{
	return userNumGrains;
}