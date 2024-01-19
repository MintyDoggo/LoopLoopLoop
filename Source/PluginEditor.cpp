/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <jive_layouts/jive_layouts.h>

//==============================================================================
LoopLoopLoopAudioProcessorEditor::LoopLoopLoopAudioProcessorEditor (LoopLoopLoopAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    grainPitchSlider(*audioProcessor.treeState.getParameter("grainPitch"), "Semitones"),
    grainSizeSlider(*audioProcessor.treeState.getParameter("grainSize"), "Seconds"),
    grainSpreadSlider(*audioProcessor.treeState.getParameter("spread"), "idk"),
    grainCountSlider(*audioProcessor.treeState.getParameter("grainCount"), "Grains"),

    grainPitchSliderAttachment(audioProcessor.treeState, "grainPitch", grainPitchSlider),
    grainSizeSliderAttachment(audioProcessor.treeState, "grainSize", grainSizeSlider),
    grainSpreadSliderAttachment(audioProcessor.treeState, "spread", grainSpreadSlider),
    grainCountSliderAttachment(audioProcessor.treeState, "grainCount", grainCountSlider)
{
    
    startTimerHz(240);
    log.open("C:\\Users\\dog1\\Desktop\\funny-log-out.txt");

    for (auto* component : getComponents())
	{
		addAndMakeVisible(component);
    }

    setResizable(true, true);
    setResizeLimits(width / 2, topPanelHeight / 2, 1920, 1080);
    getConstrainer()->setFixedAspectRatio(6.0);
    setSize(width, topPanelHeight + bottomPanelHeight);

    openGLContext.attachTo(*getTopLevelComponent());

    startTime = juce::Time::getMillisecondCounterHiRes();

}

LoopLoopLoopAudioProcessorEditor::~LoopLoopLoopAudioProcessorEditor()
{
    log.close();
}

void LoopLoopLoopAudioProcessorEditor::update()
{
    //cancelPendingRepaint();
	repaint();
}

void LoopLoopLoopAudioProcessorEditor::timerCallback()
{

}

//==============================================================================
void LoopLoopLoopAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

    //log << "FPS: " << getFPS() << std::endl;
    //g.fillAll(juce::Colour::fromFloatRGBA(0.149f, 0.149f, 0.169f, 1.0f));
    auto bounds = getBounds();

    g.fillAll(juce::Colour::fromFloatRGBA(0.15f, 0.15f, 0.16f, 1.0f));


    paintWaveform(g);
    paintGrainWindow(g);
    //paintVerticalLine(g);
    //paintFPS(g);
    //seizureTest(g);

    // horizontal separator line
    g.setColour(juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f));
    g.drawHorizontalLine(bounds.getHeight() * 0.75, 0, bounds.getWidth());
    paintCount++;
}

void LoopLoopLoopAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getBounds();
    // Calculate the width for each component based on the total number of components
    int numComponents = getComponents().size();
    int componentWidth = bounds.getWidth() / numComponents;

    auto bottomPanelBounds = bounds.removeFromBottom(bounds.getHeight() * 0.25);
    //auto waveformBounds = bounds.removeFromTop(bounds.getHeight() * 0.66);
    //auto randomParameterBounds = bounds.removeFromBottom(bounds.getHeight() * 1);

    int i = 0;
    // Calculate the new bounds for each component based on the scale factor
    for (auto* component : getComponents())
    {
        int x = componentWidth * i;
        int y = bottomPanelBounds.getY(); // TODO: this is a hacky way to fit vertically
        component->setBounds(x, y, componentWidth, bottomPanelBounds.getHeight());
        ++i;
    }

}

void LoopLoopLoopAudioProcessorEditor::paintWaveform(juce::Graphics& g)
{
    auto bounds = getBounds();
    auto waveformBounds = bounds.removeFromTop(bounds.getHeight() * 0.75);
    const int localHeight = waveformBounds.getHeight();
    const int localWidth = waveformBounds.getWidth();


    //painting the waveform
    const int ampHeight = static_cast<int>(audioProcessor.amplitude * (localHeight / 2)); // add padding here
    const int indexToPaint = static_cast<int>(audioProcessor.historyBuffer[0].getProgress() * double(localWidth)) - 1; // todo: static cast instead of function style cast
    
    if (previousIndexToPaint <= indexToPaint)
    {
        for (int i = previousIndexToPaint + 1; i <= indexToPaint; i++)
        {
            // linear interpolation between the two points
            const double progress = double(i - previousIndexToPaint) / double(indexToPaint - previousIndexToPaint);
            const int lerpAmp = linearInterpolation(rectArray[previousIndexToPaint], ampHeight, progress);
		    rectArray[i] = lerpAmp;
	    }  
    }
    else // TODO: i have no clue if the lerp for these works
    {
        // paint the end of the buffer
        for (int i = previousIndexToPaint + 1; i <= localWidth; i++)
        {
            const double progress = double(i - previousIndexToPaint) / double(localWidth - previousIndexToPaint); // TODO: i have no clue of this works
            const int lerpAmp = linearInterpolation(rectArray[previousIndexToPaint], ampHeight, progress);
            rectArray[i] = lerpAmp;
        }

        // paint the beginning of the buffer
        for (int i = 0; i <= indexToPaint; i++)
        {
			const double progress = double(i) / double(indexToPaint);
			const int lerpAmp = linearInterpolation(rectArray[localWidth], ampHeight, progress);
			rectArray[i] = lerpAmp;
		}
    }


    for (int i = 0; i < localWidth; i++)
    {
        g.setColour(juce::Colour::fromFloatRGBA(0.34f, 0.34f, 0.34f, 1.0f));
        //g.fillRect(i, topPanelHeight - rectArray[i], 1, rectArray[i]);

        int x1 = i;
        int y1 = (localHeight / 2) - rectArray[i];
        int x2 = i;
        int y2 = (localHeight / 2) + rectArray[i];

        if (i <= localWidth)
        {
            x2 = i + 1;
            y2 = (localHeight / 2) + rectArray[i + 1];
        }

        g.drawLine(x1, y1, x2, y2, 1);
    }

    previousIndexToPaint = indexToPaint;
    // we want to not increment at all sometimes to allow the whole history buffer to be shown
    //if (audioProcessor.sampleCounter >= audioProcessor.detectionLength) // this is where we can put a condition to only increment the index when we want to
    if(true)
    {
        if (audioProcessor.peakIndexToPaint < localWidth)
        {
		    audioProcessor.peakIndexToPaint++;
	    }
        else
        {
		    audioProcessor.peakIndexToPaint = 0;
	    }
    }
}

void LoopLoopLoopAudioProcessorEditor::paintGrainWindow(juce::Graphics& g)
{
    auto bounds = getBounds();
    auto waveformBounds = bounds.removeFromTop(bounds.getHeight() * 0.75);
    const int localHeight = waveformBounds.getHeight();
    const int localWidth = waveformBounds.getWidth();

    auto settings = audioProcessor.getSettings(audioProcessor.treeState);

    const double grainStartPercentage = settings.grainStart;
    const int grainStart = static_cast<int>(localWidth * grainStartPercentage);

    const double grainEndPercentage = (settings.grainSize / audioProcessor.historyBuffer[0].getUserSizeInSeconds()) + grainStartPercentage; // grainSize was a percentage but now it's a duration in seonds
    const int grainEnd = static_cast<int>(localWidth * grainEndPercentage) % localWidth;


    const int writeHead = audioProcessor.historyBuffer[0].getProgress() * localWidth;


    // TODO: Make stereo
    // paint each grain window read index, height is determined by the gain factor and is centered around the middle of the window
    for (int i = 0; i < audioProcessor.historyBuffer[0].userNumGrains; i++)
    {
        //const int playHead = static_cast<int>(grainStart + (width * (grainEndPercentage - grainStartPercentage) * audioProcessor.historyBuffer[0].grains[i].getProgress())) % width;

        double readIndex = audioProcessor.historyBuffer[0].grains[i].getReadIndex();
        wrapReadIndexToBuffer(readIndex, secondsToSamples(settings.historyBufferSize));
        const int playHead = readIndex / secondsToSamples(settings.historyBufferSize) * double(localWidth);

        const int playHeadHeightY2 = (localHeight / 2) + (audioProcessor.historyBuffer[0].grains[i].getGainFactor() * (localHeight / 2)) * .8;
        const int playHeadHeightY1 = (localHeight / 2) - (audioProcessor.historyBuffer[0].grains[i].getGainFactor() * (localHeight / 2)) * .8;

        //const int playHeadHeightY1 = (localHeight) - (audioProcessor.historyBuffer[0].grains[i].getGainFactor() * (localHeight / 2));
        //const int playHeadHeightY2 = (audioProcessor.historyBuffer[0].grains[i].getGainFactor() * (localHeight / 2)) + (localHeight);

        g.setColour(playheadColors[i % playheadColors.size()]);
        g.drawLine(playHead, playHeadHeightY1, playHead, playHeadHeightY2, 3);
	}

    // blue playhead
    g.setColour(juce::Colour::fromFloatRGBA(0.380, 0.654f, 0.910f, 1.0f));

    // horizontal line
    g.drawLine(grainStart, localHeight - 1, grainEnd, localHeight - 1, 3); // - 1 so it doesnt overlap with the horizontal separator line
    // vertical line
    g.drawLine(grainStart, 0, grainStart, localHeight, 3);

    // playhead triangles
    juce::Path triangle;
    triangle.addTriangle(grainStart - 9, 0, grainStart - 3, 0, grainStart - 3, 10);
	g.fillPath(triangle);
    triangle.addTriangle(grainStart + 9, 0, grainStart + 3, 0, grainStart + 3, 10);
    g.fillPath(triangle);

    // paint the write index
    g.setColour(juce::Colours::white);
    g.drawLine(writeHead, 0, writeHead, localHeight, 2);
}

double LoopLoopLoopAudioProcessorEditor::getFPS()
{
    double elapsedTime = juce::Time::getMillisecondCounterHiRes() - startTime;
    double elapsedMillis = elapsedTime - previousTime;
    previousTime = elapsedTime;

    return 1000.0 / elapsedMillis; // Calculate FPS
}

void LoopLoopLoopAudioProcessorEditor::paintFPS(juce::Graphics& g)
{
	g.setColour(juce::Colours::white);
	g.setFont(60.0f);
	g.drawFittedText("FPS: " + std::to_string(getFPS()), getLocalBounds(), juce::Justification::left, 1);
}

std::vector<juce::Component*> LoopLoopLoopAudioProcessorEditor::getComponents()
{
    return
    {
        &grainPitchSlider,
        &grainSizeSlider,
        &grainSpreadSlider,
        &grainCountSlider
    };
}
