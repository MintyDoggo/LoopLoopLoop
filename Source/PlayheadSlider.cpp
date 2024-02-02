#include "PlayheadSlider.h"
#include "Utilities.h"

// paint implementation
void PlayheadSlider::paint(juce::Graphics& g)
{
	auto bounds = getLocalBounds();

	// blue playhead
    g.setColour(juce::Colour::fromFloatRGBA(0.380, 0.654f, 0.910f, 1.0f));

	float xPos = bounds.getX() + (bounds.getWidth() * getValue());

    //
    g.drawLine(xPos, 0, xPos, bounds.getHeight(), 3);

    // playhead triangles
    juce::Path triangle;
    triangle.addTriangle(xPos - 9, 0, xPos - 3, 0, xPos - 3, 10);
	g.fillPath(triangle);
    triangle.addTriangle(xPos + 9, 0, xPos + 3, 0, xPos + 3, 10);
    g.fillPath(triangle);


	//// Set sensitivity (adjust this value based on your preference)
	//setVelocityBasedMode(true);
	//setVelocityModeParameters(0.2, 1.0, 0.1, false);
}
