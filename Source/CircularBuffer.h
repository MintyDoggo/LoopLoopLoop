#include <JuceHeader.h>
#include "Utilities.h"
#include "GrainWindow.h"

class CircularBuffer
{
public:
	CircularBuffer();

	void write(double sample);
	double read(double index);

	void setSize(double size);
	int getSize();
	void setUserSize(double size);
	double getUserSizeInSeconds();

	double getProgress();
	void clear();

	std::vector<GrainWindow> grains;
	bool writePause;
	int userNumGrains;

private:
	std::vector<double> buffer;
	int writeIndex;
	int userSize;
};
