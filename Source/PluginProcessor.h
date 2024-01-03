/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularBuffer.h"

struct Settings
{
    double gain{ 1.0 };
    double grainSize{ 0.2 };
    double grainPitch{ 0.0 };
    double grainStart{ 0.0 };
    double historyBufferSize{ 2.0 };
    double mix { 0.8 };
    double grainAttack { 0.1 };
    double grainDecay { 1.0 };
    bool grainReverse { false };
};

Settings getSettings(juce::AudioProcessorValueTreeState& treeState);

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

    static double sampleRate;
    
    juce::AudioProcessorValueTreeState treeState;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoopLoopLoopAudioProcessor)
    std::vector<CircularBuffer> historyBuffer;

};


