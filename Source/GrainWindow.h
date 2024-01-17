#include <JuceHeader.h>
#include "Utilities.h"

class GrainWindow
{
public:
	GrainWindow();

	double getReadIndex();

	void incrementReadIndex();

	void setStartIndex(double start);

	void setGrainPitch(double semitones);

	void setGrainSize(double size);

	void setAttack(double attack);
	double getAttack();

	void setDecay(double decay);
	double getDecay();

	double getProgress();

	void setReverse(bool reverse);

	void setReadIndex(double index);

	double getGainFactor();

	void setReadOffset(double offset);

private:
	double startIndex;
	double readIndex;
	double readOffset;
	double size;
	double attack;
	double decay;
	double playbackSpeed;
	double progress; // progress of the grain from 0 to 1
	bool reverse;
};