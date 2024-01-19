#include "GrainWindow.h"

// -------------------------- GrainWindow -----------------------------------------------------
GrainWindow::GrainWindow()
{
	startIndex = 0.0;
	readIndex = 0.0 + startIndex;
	size = 0.4f;
	attack = 0.15f;
	decay = 0.85f;
	playbackSpeed = semitonesToPlaybackRate(0.0);
	progress = 0.0;
	reverse = false;
	readOffset = 0.0;
	semitoneMode = true;
}

void GrainWindow::setReadIndex(double index)
{
	readIndex = secondsToSamples(index);
}

// warning: this function will return a value outside of the bounds of the user size
double GrainWindow::getReadIndex()
{
	const double realIndex = readIndex + readOffset;
	return realIndex;
}

void GrainWindow::incrementReadIndex()
{
	readIndex += playbackSpeed;

	const double normalizedOffsetReadIndex = (readIndex - startIndex + readOffset);
	progress = normalizedOffsetReadIndex / secondsToSamples(size);

	// for positive playback speeds
	if (readIndex - startIndex + readOffset >= secondsToSamples(size))
	{
		readIndex = readIndex - secondsToSamples(size);
		progress = 0.0;
		return;
	}

	// for negative playback speeds
	if (readIndex + readOffset < 0.0 + startIndex)
	{
		readIndex = readIndex + secondsToSamples(size);
		progress = 0.0; // progress should maybe be 1.0
		return;
	}
}

void GrainWindow::setReadOffset(double offset)
{
	offset = secondsToSamples(offset);
	readOffset = offset;
}

// When calling this function, make sure that the index is within the bounds of the buffer
void GrainWindow::setStartIndex(double start)
{
	startIndex = secondsToSamples(start);
}

void GrainWindow::setGrainPitch(double speed)
{
	if (semitoneMode)
	{
		if (!reverse)
		{
			playbackSpeed = semitonesToPlaybackRate(speed);
			return;
		}

		if (reverse)
		{
			playbackSpeed = semitonesToPlaybackRate(speed) * -1;
			return;
		}
	}

	playbackSpeed = speed;
}


double GrainWindow::getGainFactor()
{
	// this makes it so the decay is applied after the attack, they dont overlap
	if (progress < attack)
	{
		return progress / attack;
	}
	else if (progress > decay)
	{
		return 1.0 - ((progress - decay) / (1.0 - decay));
	}
	else
	{
		return 1.0;
	}
}
