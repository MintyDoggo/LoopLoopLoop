#include <JuceHeader.h>
#include <fstream>

class GrainWindow
{
public:
	GrainWindow();

	void setReadIndex(double index);
	double getReadIndex();
	void incrementReadIndex(double historyBufferUserSize);

	void setReadOffset(double offset);
	void setRandomStartOffset(double offset);

	void setStartIndex(double start);

	void setGrainPitch(double semitones);

	double getGainFactor();

	// if there is special get or set functions, put them as private
	double size;
	double attack;
	double decay;
	double progress; // progress of the grain from 0 to 1
	bool reverse;
	bool semitoneMode;
	bool grainHasReset;
	double randomPitch;
	double randomPitchMax;
	double randomStartOffsetMax;

	juce::Random pitchRandomizer;
	juce::Random startOffsetRandomizer;

private:
	double startIndex;
	double readIndex;
	double readOffset;
	double randomStartOffset;
	double playbackSpeed;

	std::ofstream log;
};
