#pragma once
#include <JuceHeader.h>
#include <random>

#define MAX_NUM_GRAINS 512

static juce::Random random;

double secondsToSamples(double seconds);

double samplesToSeconds(double samples);

double linearInterpolation(double y1, double y2, double x);

double semitonesToPlaybackRate(double semitones);

double playbackRateToSemitones(double playbackRate);

double wrapReadIndexToBuffer(double readIndex, double bufferSize);

double remapValue(double originalValue, double originalMin, double originalMax, double targetMin, double targetMax);

double getRandomDouble(double min, double max);
