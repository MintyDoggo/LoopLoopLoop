#pragma once

#include <JuceHeader.h>

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawLinearSlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        juce::Slider::SliderStyle,
        juce::Slider&) override;
};
