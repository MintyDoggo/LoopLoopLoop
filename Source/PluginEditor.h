/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <fstream>
#include "TextSlider.h"

//==============================================================================
/**
*/
class LoopLoopLoopAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    LoopLoopLoopAudioProcessorEditor (LoopLoopLoopAudioProcessor&);
    ~LoopLoopLoopAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

    void update();

    void paintWaveform(juce::Graphics&);

    void paintGrainWindow(juce::Graphics&);

    void paintFPS(juce::Graphics& g);

    std::ofstream log;

    double getFPS();


private:
    LoopLoopLoopAudioProcessor& audioProcessor;
    juce::OpenGLContext openGLContext;

    const int width = 840;
    const int topPanelHeight = 105;
    const int bottomPanelHeight = 35;

    int rectArray[1920] = { 0 };
    int previousIndexToPaint = 0;
    juce::VBlankAttachment vBlankAttachment{ this, [this] { update(); } };
    int paintCount = 0;

    juce::Time time;
    double startTime;
    double previousTime = 0;
    double elapsedTime = 0;

    juce::Array<juce::Colour> playheadColors
    {
        juce::Colour::fromFloatRGBA(0.70f, 0.45f, 0.95f, 1.0f),
        juce::Colour::fromFloatRGBA(0.69f, 0.44f, 0.94f, 1.0f),
        juce::Colour::fromFloatRGBA(0.68f, 0.43f, 0.93f, 1.0f),
        juce::Colour::fromFloatRGBA(0.67f, 0.42f, 0.92f, 1.0f),
        juce::Colour::fromFloatRGBA(0.66f, 0.41f, 0.91f, 1.0f),
        juce::Colour::fromFloatRGBA(0.65f, 0.40f, 0.90f, 1.0f),
        juce::Colour::fromFloatRGBA(0.64f, 0.39f, 0.89f, 1.0f),
        juce::Colour::fromFloatRGBA(0.63f, 0.38f, 0.88f, 1.0f),
        juce::Colour::fromFloatRGBA(0.62f, 0.37f, 0.87f, 1.0f),
        juce::Colour::fromFloatRGBA(0.61f, 0.36f, 0.86f, 1.0f),
        juce::Colour::fromFloatRGBA(0.60f, 0.35f, 0.85f, 1.0f),
        juce::Colour::fromFloatRGBA(0.59f, 0.34f, 0.84f, 1.0f),
        juce::Colour::fromFloatRGBA(0.58f, 0.33f, 0.83f, 1.0f),
        juce::Colour::fromFloatRGBA(0.57f, 0.32f, 0.82f, 1.0f),
        juce::Colour::fromFloatRGBA(0.56f, 0.31f, 0.81f, 1.0f),
        juce::Colour::fromFloatRGBA(0.55f, 0.30f, 0.80f, 1.0f),
        juce::Colour::fromFloatRGBA(0.54f, 0.29f, 0.79f, 1.0f),
        juce::Colour::fromFloatRGBA(0.53f, 0.28f, 0.78f, 1.0f),
        juce::Colour::fromFloatRGBA(0.52f, 0.27f, 0.77f, 1.0f),
        juce::Colour::fromFloatRGBA(0.51f, 0.26f, 0.76f, 1.0f),
        juce::Colour::fromFloatRGBA(0.50f, 0.25f, 0.75f, 1.0f),
        juce::Colour::fromFloatRGBA(0.49f, 0.24f, 0.74f, 1.0f),
        juce::Colour::fromFloatRGBA(0.48f, 0.23f, 0.73f, 1.0f),
        juce::Colour::fromFloatRGBA(0.47f, 0.22f, 0.72f, 1.0f),
        juce::Colour::fromFloatRGBA(0.46f, 0.21f, 0.71f, 1.0f),
        juce::Colour::fromFloatRGBA(0.45f, 0.20f, 0.70f, 1.0f),
        juce::Colour::fromFloatRGBA(0.44f, 0.19f, 0.69f, 1.0f),
        juce::Colour::fromFloatRGBA(0.43f, 0.18f, 0.68f, 1.0f),
        juce::Colour::fromFloatRGBA(0.42f, 0.17f, 0.67f, 1.0f),
        juce::Colour::fromFloatRGBA(0.41f, 0.16f, 0.66f, 1.0f),
        juce::Colour::fromFloatRGBA(0.40f, 0.15f, 0.65f, 1.0f),
        juce::Colour::fromFloatRGBA(0.39f, 0.14f, 0.64f, 1.0f),
        juce::Colour::fromFloatRGBA(0.38f, 0.13f, 0.63f, 1.0f),
        juce::Colour::fromFloatRGBA(0.37f, 0.12f, 0.62f, 1.0f),
        juce::Colour::fromFloatRGBA(0.36f, 0.11f, 0.61f, 1.0f),
    };

    // Slider definitions and attachments
    TextSlider
        grainPitchSlider,
        grainSizeSlider,
        grainSpreadSlider,
        grainCountSlider;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment
		grainPitchSliderAttachment,
		grainSizeSliderAttachment,
		grainSpreadSliderAttachment,
        grainCountSliderAttachment;

    std::vector<juce::Component*> getComponents();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopLoopLoopAudioProcessorEditor)
};
