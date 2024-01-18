/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularBuffer.h"
#include <fstream>

struct Settings
{
    double gain{ 1.0 };
    double grainSize{ 0.4 };
    double grainPitch{ 0.0 };
    double grainStart{ 0.0 };
    double historyBufferSize{ 6.0 };
    double mix { 1.0 };
    double grainAttack { 0.15 };
    double grainDecay { 0.85 };
    int grainCount { 1 };
    bool grainReverse { false };
    bool writePause { false };
    double spread { 0.0 };
    bool semitoneMode { true };
};

//==============================================================================
/**
*/
class LoopLoopLoopAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    LoopLoopLoopAudioProcessor();
    ~LoopLoopLoopAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    Settings getSettings(juce::AudioProcessorValueTreeState& treeState);

    static double sampleRate;
    
    juce::AudioProcessorValueTreeState treeState;
    std::atomic<double> amplitude;
    int peakIndexToPaint = 0; // the index in rectArray that we want to paint

    // for drawing peaks
    void calculateRMS(double sample, int shouldIncrement);
    void calculateDetectionLength(double sampleRate, double historyBufferSize);
    double RMSCombinedSum = 0.0; // sum of l and r channel
    double RMSValue = 0.0; // rms of l and r channel
    int sampleCounter = 0; // how many samples we've seen so far
    int detectionLength = 0; // how many samples to sum for RMS. this does not determine the animation speed.
    std::vector<CircularBuffer> historyBuffer;


private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    std::ofstream log;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoopLoopLoopAudioProcessor)

};


