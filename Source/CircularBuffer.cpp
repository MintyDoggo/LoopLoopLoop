#include "CircularBuffer.h"
#include <JuceHeader.h>
#include "Utilities.h"
#include <cmath>


// -------------------------- GrainWindow -----------------------------------------------------
// TODO: StartIndex is not used
GrainWindow::GrainWindow()
{
	startIndex = 0.0;
	readIndex = 0.0 + startIndex;
	size = 0.2f;
	attack = 0.1f;
	decay = 1.0f;
	playbackSpeed = semitonesToPlaybackRate(0.0);
	progress = 0.0;
	reverse = false;
}

double GrainWindow::getReadIndex()
{
	return readIndex;
}

void GrainWindow::incrementReadIndex()
{
	readIndex += playbackSpeed;
	progress = (readIndex - startIndex) / secondsToSamples(size);

	// for positive playback speeds
	if (readIndex - startIndex >= secondsToSamples(size))
	{
		readIndex = readIndex - secondsToSamples(size);
		progress = 0.0;
	}

	// for negative playback speeds
	if (readIndex < 0.0 + startIndex)
	{
		readIndex = readIndex + secondsToSamples(size);
		progress = 0.0;
	}
}

// When calling this function, make sure that the index is within the bounds of the buffer
void GrainWindow::setStartIndex(double start)
{
	startIndex = secondsToSamples(start);
}

void GrainWindow::setGrainPitch(double semitones)
{

	if (!reverse)
	{
		playbackSpeed = semitonesToPlaybackRate(semitones);
	}

	if (reverse)
	{
		playbackSpeed = semitonesToPlaybackRate(semitones) * -1;
	}
}

void GrainWindow::setGrainSize(double size)
{
	this->size = size;
}

void GrainWindow::setAttack(double attack)
{
	this->attack = attack;
}

double GrainWindow::getAttack()
{
	return attack;
}

void GrainWindow::setDecay(double decay)
{
	this->decay = decay;
}

double GrainWindow::getDecay()
{
	return decay;
}

double GrainWindow::getProgress()
{
	return progress;
}

void GrainWindow::setReverse(bool reverse)
{
	this->reverse = reverse;
}

void GrainWindow::setReadIndex(double index)
{
	readIndex = index;
}

// -------------------------- GrainWindow -----------------------------------------------------

CircularBuffer::CircularBuffer()
{
	writeIndex = 0;
	grains.resize(32);
}

void CircularBuffer::write(double sample)
{
	buffer[writeIndex] = sample;
	writeIndex = (writeIndex + 1) % buffer.size();
}

double CircularBuffer::read(double index)
{
	const int firstIndex = int(std::floor(index)) % buffer.size();
	const int nextIndex = (firstIndex + 1) % buffer.size();
	//interpolation
	const double sample = linearInterpolation(buffer[firstIndex], buffer[nextIndex], index - firstIndex);
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

void CircularBuffer::clear()
{
	for (int i = 0; i < buffer.size(); i++)
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

