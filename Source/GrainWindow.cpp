#include "GrainWindow.h"
#include "Utilities.h"
#include "ParameterRegistration.h"

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
	randomStartOffset = 0.0;
	grainHasReset = true;
	randomPitch = 0.0;
	randomPitchMax = 0.0;

	//log.open("C:\\Users\\dog1\\Desktop\\funny-log-out.txt");
}


void GrainWindow::setReadIndex(double index)
{
	readIndex = secondsToSamples(index);
}

// warning: this function will return a value outside of the bounds of the user size
double GrainWindow::getReadIndex()
{
	const double realIndex = readIndex + randomStartOffset + readOffset;
	return realIndex;
}

void GrainWindow::incrementReadIndex(double historyBufferUserSize)
{
	readIndex += playbackSpeed + randomPitch;

	const double normalizedOffsetReadIndex = (readIndex - startIndex + readOffset); // for example, if read index is 1300 and start index is 1000, normalizedOffsetReadIndex is 300
	progress = normalizedOffsetReadIndex / secondsToSamples(size); 

	// for positive playback speeds, if it has reset
	if (normalizedOffsetReadIndex >= secondsToSamples(size))
	{
		readIndex = readIndex - secondsToSamples(size);
		progress = 0.0;
		randomPitch = pitchRandomizer.nextDouble() * randomPitchMax;
		randomStartOffset = secondsToSamples(startOffsetRandomizer.nextDouble() * randomStartOffsetMax * historyBufferUserSize);

		grainHasReset = true;

		return;
	}

	// for negative playback speeds, if it has reset
	if (readIndex + readOffset < 0.0 + startIndex)
	{
		readIndex = readIndex + secondsToSamples(size);
		progress = 0.0; // progress should maybe be 1.0
		randomPitch = pitchRandomizer.nextDouble() * randomPitchMax;
		randomStartOffset = secondsToSamples(startOffsetRandomizer.nextDouble() * randomStartOffsetMax * historyBufferUserSize);

		grainHasReset = true;
		return;
	}

	grainHasReset = false;
}

void GrainWindow::setReadOffset(double offset)
{
	readOffset = secondsToSamples(offset);
}

void GrainWindow::setRandomStartOffset(double offset)
{
	randomStartOffset = secondsToSamples(offset);
}

// be careful not to set start index to a value outside of the user size
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
	// this makes it so the decay is applied after the attack, they can overlap but only one is applied at a time
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
