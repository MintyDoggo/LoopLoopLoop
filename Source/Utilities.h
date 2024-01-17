#pragma once

double secondsToSamples(double seconds);

double samplesToSeconds(double samples);

double linearInterpolation(double y1, double y2, double x);

double semitonesToPlaybackRate(double semitones);

void wrapReadIndexToBuffer(double& readIndex, int bufferSize);

double remapValue(double originalValue, double originalMin, double originalMax, double targetMin, double targetMax);