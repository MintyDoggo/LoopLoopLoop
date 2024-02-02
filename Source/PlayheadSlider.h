#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

struct PlayheadSlider : juce::Slider
{
    PlayheadSlider(juce::RangedAudioParameter& rap, const juce::String& unitSuffix) :
        juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
        parameter(&rap),
        suffix(unitSuffix)
    {
        setLookAndFeel(&lnf);
        setSliderStyle(LinearHorizontal);
    }

    ~PlayheadSlider()
    {
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g) override;

private:
    LookAndFeel lnf;
    juce::RangedAudioParameter* parameter;
    juce::String suffix;
};
