#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

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

    void paint(juce::Graphics& g) override;

    juce::Rectangle<int> getSliderBounds() const;

    juce::String getDisplayString() const;

    double getNormalisedValue();

    float getSliderMinWidth();

    // add listener to slider here maybe

private:
    LookAndFeel lnf;
    juce::RangedAudioParameter* parameter;
    juce::String suffix;
};
