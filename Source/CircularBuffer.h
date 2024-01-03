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

private:
	double startIndex;
	double readIndex;
	double size;
	double attack;
	double decay;
	double playbackSpeed;
	double progress;
	bool reverse;
};

class CircularBuffer
{
public:
	CircularBuffer();

	void write(double sample);

	double read(double index);

	int getSize();
	void setSize(double size);

	void clear();

	int getWriteIndex();

	int getNumGrains();

	std::vector<GrainWindow> grains;

private:
	std::vector<double> buffer;
	int writeIndex;
};

