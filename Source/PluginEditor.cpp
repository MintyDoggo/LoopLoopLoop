/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <fstream>
//==============================================================================
LoopLoopLoopAudioProcessorEditor::LoopLoopLoopAudioProcessorEditor (LoopLoopLoopAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), openGLComponent()
{
    addAndMakeVisible(openGLComponent);
    openGLComponent.setBounds(0, 0, 400, 300);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    // start timer
    startTimerHz(60);
}

LoopLoopLoopAudioProcessorEditor::~LoopLoopLoopAudioProcessorEditor()
{
    // stop timer
    stopTimer();
}

void LoopLoopLoopAudioProcessorEditor::timerCallback()
{
	//openGLComponent.updateAnimation();
}
//==============================================================================
void LoopLoopLoopAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void LoopLoopLoopAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
