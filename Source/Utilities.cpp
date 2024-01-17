#include "Utilities.h"
#include "PluginProcessor.h"
#include <cmath>

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

double semitonesToPlaybackRate(double semitones)
{
	return std::pow(2.0, semitones / 12.0);
}

void wrapReadIndexToBuffer(double& readIndex, int bufferSize)
{
	readIndex = std::abs(std::fmod(readIndex, bufferSize));
}