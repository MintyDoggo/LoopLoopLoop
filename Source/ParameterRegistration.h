#pragma once
#include <JuceHeader.h>

struct Settings
{
    double gain = 1.0;
    double grainSize = 0.4;
    double grainPitch = 0.0;
    double grainStart = 0.0;
    double historyBufferSize = 6.0;
    double mix = 1.0;
    double grainAttack = 0.15;
    double grainDecay = 0.85;
    int grainCount = 1;
    bool grainReverse = false;
    bool writePause = false;
    double positionRandom = 0.0;
    bool semitoneMode = true;
    double pitchRandom = 0.0;
    double grainReadOffset = 0.0;
};

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
void addParameterListeners(juce::AudioProcessorValueTreeState& treeState, juce::AudioProcessorValueTreeState::Listener* listener);
void removeParameterListeners(juce::AudioProcessorValueTreeState& treeState, juce::AudioProcessorValueTreeState::Listener* listener);
Settings getSettings(juce::AudioProcessorValueTreeState& treeState);
