/*
  ==============================================================================

    TextSlider.h
    Created: 19 Jan 2024 4:23:03am
    Author:  dog1

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics&,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider&) override { }
};

struct TextSlider : juce::Slider
{
    TextSlider(juce::RangedAudioParameter& rap, const juce::String& unitSuffix) :
        juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
        parameter(&rap),
        suffix(unitSuffix)
    {
        setLookAndFeel(&lnf);
    }

    ~TextSlider()
    {
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g) override
    {
        // this code needs to be moved to a separate function, also dont use namespace juce
        using namespace juce;
		auto startAngle = degreesToRadians(135.f);

		auto bounds = getLocalBounds();
		auto radius = juce::jmin(bounds.getWidth() / 2, bounds.getHeight() / 2) - 4.0f;
		auto centreX = bounds.getCentreX();
		auto centreY = bounds.getCentreY();
		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2.0f;

		juce::Rectangle<float> r(rx, ry, rw, rw);

		g.setColour(juce::Colours::white);
		g.drawEllipse(r, 2.0f);

		g.setColour(juce::Colours::red);
		g.drawEllipse(r, 1.0f);

		g.setColour(juce::Colours::white);
		g.setFont(12.0f);
		g.drawText(juce::String(getValue(), 2) + suffix, bounds, juce::Justification::centred, true);

        // Set sensitivity (adjust this value based on your preference)
    /*    setVelocityBasedMode(true);
        setVelocityModeParameters(0.2, 1.0, 0.1, false);*/

	}

    // add listener to slider here maybe

private:
    LookAndFeel lnf;
    juce::RangedAudioParameter* parameter;
    juce::String suffix;
};
