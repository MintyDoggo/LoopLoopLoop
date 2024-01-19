#include <JuceHeader.h>
#include "Utilities.h"

class GrainWindow
{
public:
	GrainWindow();

	void setReadIndex(double index);
	double getReadIndex();
	void incrementReadIndex();

	void setReadOffset(double offset);

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

private:
	double startIndex;
	double readIndex;
	double readOffset;
	double playbackSpeed;
};
