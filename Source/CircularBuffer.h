#include <JuceHeader.h>
#include "Utilities.h"
#include "GrainWindow.h"

class CircularBuffer
{
public:
	CircularBuffer();

	void write(double sample);

	double read(double index);

	int getSize();
	void setSize(double size);
	double getUserSizeInSeconds();
	void setUserSize(double size);

	void clear();

	int getWriteIndex();

	int getNumGrains();

	double getProgress();

	void setUserNumGrains(int numGrains);
	int getUserNumGrains();


	std::vector<GrainWindow> grains;
	bool writePause;

private:
	std::vector<double> buffer;
	int writeIndex;
	int userNumGrains;
	int userSize;
};

