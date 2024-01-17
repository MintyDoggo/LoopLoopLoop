/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <fstream>

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

    void paintVerticalLine(juce::Graphics&);
    void seizureTest(juce::Graphics& g);
    void paintFPS(juce::Graphics& g);

    std::ofstream log;

    double getFPS();


private:
    LoopLoopLoopAudioProcessor& audioProcessor;
    juce::OpenGLContext openGLContext;

    const int width = 960;
    const int height = 90;
    int rectArray[960] = { 0 };
    int previousIndexToPaint = 0;
    juce::VBlankAttachment vBlankAttachment{ this, [this] { update(); } };
    const int padding = 10;
    int paintCount = 0;

    juce::Time time;
    double startTime;
    double previousTime = 0;
    double elapsedTime = 0;

    juce::Array<juce::Colour> playheadColors
    {
        juce::Colour::fromFloatRGBA(0.5f, 0.0f, 1.0f, 1.0f),   // light purple
        juce::Colour::fromFloatRGBA(0.4f, 0.0f, 0.8f, 1.0f),   // medium purple
        juce::Colour::fromFloatRGBA(0.3f, 0.0f, 0.6f, 1.0f),   // dark purple
        juce::Colour::fromFloatRGBA(0.6f, 0.0f, 1.0f, 1.0f),   // lavender
        juce::Colour::fromFloatRGBA(0.7f, 0.0f, 1.0f, 1.0f),   // lilac
        juce::Colour::fromFloatRGBA(0.8f, 0.0f, 1.0f, 1.0f),   // orchid
        juce::Colour::fromFloatRGBA(0.5f, 0.2f, 1.0f, 1.0f),   // pale purple
        juce::Colour::fromFloatRGBA(0.4f, 0.2f, 0.8f, 1.0f),   // muted purple
        juce::Colour::fromFloatRGBA(0.3f, 0.2f, 0.6f, 1.0f),   // deep purple
        juce::Colour::fromFloatRGBA(0.7f, 0.5f, 1.0f, 1.0f),   // pastel purple
        juce::Colour::fromFloatRGBA(0.8f, 0.5f, 0.8f, 1.0f),   // dusty purple
        juce::Colour::fromFloatRGBA(0.9f, 0.5f, 0.6f, 1.0f),   // muted lavender
        juce::Colour::fromFloatRGBA(0.6f, 0.4f, 1.0f, 1.0f),   // soft purple
        juce::Colour::fromFloatRGBA(0.5f, 0.4f, 0.8f, 1.0f),   // rich purple
        juce::Colour::fromFloatRGBA(0.4f, 0.4f, 0.6f, 1.0f),   // royal purple
        juce::Colour::fromFloatRGBA(0.8f, 0.7f, 1.0f, 1.0f),   // light lavender
        juce::Colour::fromFloatRGBA(0.7f, 0.7f, 0.8f, 1.0f),   // muted lilac
        juce::Colour::fromFloatRGBA(0.6f, 0.7f, 0.6f, 1.0f)    // muted orchid
    };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopLoopLoopAudioProcessorEditor)
};
