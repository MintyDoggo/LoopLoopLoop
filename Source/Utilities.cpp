#include "Utilities.h"
#include "PluginProcessor.h"
#include <cmath>
#include <random>

double secondsToSamples(double seconds)
{
	return (seconds * LoopLoopLoopAudioProcessor::sampleRate);
}

double samplesToSeconds(double samples)
{
	return samples / LoopLoopLoopAudioProcessor::sampleRate;
}

// y1 = y value of first point, y2 = y value of second point, x = x value of point between y1 and y2
double linearInterpolation(double y1, double y2, double x)
{
	return y1 + ((y2 - y1) * x);
}

double remapValue(double originalValue, double originalMin, double originalMax, double targetMin, double targetMax)
{
	// Calculate the normalized position of the original value within the original range
	double normalizedPosition = (originalValue - originalMin) / (originalMax - originalMin);

	// Use linear interpolation to find the corresponding value within the target range
	double remappedValue = linearInterpolation(targetMin, targetMax, normalizedPosition);

	return remappedValue;
}

double semitonesToPlaybackRate(double semitones)
{
	return std::pow(2.0, semitones / 12.0);
}

double playbackRateToSemitones(double playbackRate) // any ret val mod 12 is a C note
{
	return 12.0 * std::log2(playbackRate);
}

double wrapReadIndexToBuffer(double readIndex, double bufferSize)
{
	if (readIndex < 0.0)
	{
		readIndex = bufferSize - std::abs(std::fmod(readIndex, bufferSize));
		return readIndex;
	}

	return readIndex = std::abs(std::fmod(readIndex, bufferSize));
}

double getRandomDouble(double min, double max)
{
	srand(time(nullptr));
	double ran = (double)rand() / RAND_MAX;
	return min + ran * (max - min);
}
