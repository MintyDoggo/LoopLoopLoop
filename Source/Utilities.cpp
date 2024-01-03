#include "Utilities.h"
#include "PluginProcessor.h"

double secondsToSamples(double seconds)
{
	return (seconds * LoopLoopLoopAudioProcessor::sampleRate);
}

double samplesToSeconds(double samples)
{
	return samples / LoopLoopLoopAudioProcessor::sampleRate;
}

double linearInterpolation(double y1, double y2, double x)
{
	return y1 + ((y2 - y1) * x);
}

double semitonesToPlaybackRate(double semitones)
{
	return std::pow(2.0, semitones / 12.0);
}