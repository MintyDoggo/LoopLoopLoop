#include "TextSlider.h"
#include "Utilities.h"

// paint implementation
void TextSlider::paint(juce::Graphics& g)
{
	// this code needs to be moved to a separate function, also dont use namespace juce

	auto bounds = getLocalBounds();
	auto top = bounds.removeFromTop(bounds.getHeight() * 0.40f);
	auto bottom = bounds.removeFromBottom(bounds.getHeight() * 0.10f);

	g.setColour(juce::Colours::white);
	g.setFont(top.getHeight());
	g.drawText(getName(), top, juce::Justification::centred, true);

	g.setFont(bounds.getHeight());
	g.drawText(getDisplayString(), bounds, juce::Justification::centred, true);

	//// draw bounds that are draggable (not working)
	//g.setColour(juce::Colours::yellow);
	//g.drawRect(bounds);

	 //draw progress bar
	// get value in bounds range
	auto progressInBounds = juce::jmap(getNormalisedValue(), 0.0, (double)bottom.getWidth());


	g.setColour(juce::Colours::white);
	g.drawLine(bottom.getX(), bottom.getCentreY(), progressInBounds, bottom.getCentreY(), 2.0f);


	//getLookAndFeel().drawLinearSlider(g,
	//	bounds.getX(),
	//	bounds.getY(),
	//	bounds.getWidth(),
	//	bounds.getHeight(),
	//	juce::jmap(getValue(), getMinimum(), getMaximum(), 0.0, 1.0), // normalised value
	//	0.0,
	//	1.0,
	//	getSliderStyle(),
	//	*this);


	//// Set sensitivity (adjust this value based on your preference)
	//setVelocityBasedMode(true);
	//setVelocityModeParameters(0.2, 1.0, 0.1, false);

}

juce::Rectangle<int> TextSlider::getSliderBounds() const
{
	auto bounds = getLocalBounds();

	auto height = bounds.getHeight();

	juce::Rectangle<int> sliderBounds;

	// get string width
	auto displayStringWidth = juce::Font(height * 0.5f).getStringWidthFloat(getDisplayString());

	sliderBounds.setSize(displayStringWidth, height);
	sliderBounds.setCentre(bounds.getCentreX(), 0);
	sliderBounds.setY(0);

	return sliderBounds;
}

juce::String TextSlider::getDisplayString() const
{
	if(suffix == "%")
		return juce::String(getValue() * 100.0, 1) + " " + suffix;

	if(getName() == "Grains")
		return juce::String(getValue(), 0);

	return juce::String(getValue(), 1) + " " + suffix;
}

double TextSlider::getNormalisedValue()
{
	return juce::jmap(getValue(), getMinimum(), getMaximum(), 0.0, 1.0);
}

// get min width of slider
float TextSlider::getSliderMinWidth()
{
	auto bounds = getLocalBounds();

	auto height = bounds.getHeight();

	juce::String longestString;
	juce::String padding = "  ";

	// get string width
	longestString = juce::String(getMaximum(), 1) + " " + suffix + padding;

	if (suffix == "%") // for percentage
		longestString = juce::String(getMaximum() * 100.0, 1) + " " + suffix + padding;

	if (getMinimum() < 0.0) // for negative values (- sign makes it longer)
		longestString = juce::String(getMinimum(), 1) + " " + suffix + padding;

	auto displayStringWidth = juce::Font(height * 0.5).getStringWidthFloat(longestString);

	return displayStringWidth;
}
