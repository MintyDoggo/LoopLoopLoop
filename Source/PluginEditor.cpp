/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utilities.h"
#include "ParameterRegistration.h"

//==============================================================================
LoopLoopLoopAudioProcessorEditor::LoopLoopLoopAudioProcessorEditor(LoopLoopLoopAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    grainPitchSlider(*audioProcessor.treeState.getParameter("grainPitch"), "st"),
    grainSizeSlider(*audioProcessor.treeState.getParameter("grainSize"), "Sec"),
    grainPositionRandomSlider(*audioProcessor.treeState.getParameter("positionRandom"), "%"),
    grainCountSlider(*audioProcessor.treeState.getParameter("grainCount"), ""),
    grainStartSlider(*audioProcessor.treeState.getParameter("grainStart"), "%"),
    historyBufferSizeSlider(*audioProcessor.treeState.getParameter("historyBufferSize"), "Sec"),
    pitchRandomSlider(*audioProcessor.treeState.getParameter("pitchRandom"), "%"),
    grainReadOffsetSlider(*audioProcessor.treeState.getParameter("grainReadOffset"), "%"),
    grainAttackSlider(*audioProcessor.treeState.getParameter("grainAttack"), "%"),
    grainDecaySlider(*audioProcessor.treeState.getParameter("grainDecay"), "%"),
    mixSlider(*audioProcessor.treeState.getParameter("mix"), "%"),

    grainPitchSliderAttachment(audioProcessor.treeState, "grainPitch", grainPitchSlider),
    grainSizeSliderAttachment(audioProcessor.treeState, "grainSize", grainSizeSlider),
    grainPositionRandomSliderAttachment(audioProcessor.treeState, "positionRandom", grainPositionRandomSlider),
    grainCountSliderAttachment(audioProcessor.treeState, "grainCount", grainCountSlider),
    grainStartSliderAttachment(audioProcessor.treeState, "grainStart", grainStartSlider),
    historyBufferSizeSliderAttachment(audioProcessor.treeState, "historyBufferSize", historyBufferSizeSlider),
    pitchRandomSliderAttachment(audioProcessor.treeState, "pitchRandom", pitchRandomSlider),
    grainReadOffsetSliderAttachment(audioProcessor.treeState, "grainReadOffset", grainReadOffsetSlider),
    grainAttackSliderAttachment(audioProcessor.treeState, "grainAttack", grainAttackSlider),
    grainDecaySliderAttachment(audioProcessor.treeState, "grainDecay", grainDecaySlider),
    mixSliderAttachment(audioProcessor.treeState, "mix", mixSlider)
{
    
    startTimerHz(240);
    //log.open("C:\\Users\\dog1\\Desktop\\funny-log-out.txt");

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

    //make components visible
    addAndMakeVisible(grainStartSlider);
    addAndMakeVisible(grainPitchSlider);
    addAndMakeVisible(grainSizeSlider);
    addAndMakeVisible(grainPositionRandomSlider);
    addAndMakeVisible(grainCountSlider);
    addAndMakeVisible(historyBufferSizeSlider);
    addAndMakeVisible(pitchRandomSlider);
    addAndMakeVisible(grainReadOffsetSlider);
    addAndMakeVisible(grainAttackSlider);
    addAndMakeVisible(grainDecaySlider);
    addAndMakeVisible(grainReadOffsetSlider);
    addAndMakeVisible(mixSlider);

}

LoopLoopLoopAudioProcessorEditor::~LoopLoopLoopAudioProcessorEditor()
{
    //log.close();
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
    auto bottomPanelBounds = bounds.removeFromBottom(bounds.getHeight() * 0.25);


    // position start slider
    grainStartSlider.setBounds(bounds);

    grainPitchSlider.setName("Pitch");
    grainSizeSlider.setName("Size");
    grainPositionRandomSlider.setName("Pos Ran");
    grainCountSlider.setName("Grains");
    historyBufferSizeSlider.setName("Buffer Size");
    pitchRandomSlider.setName("Pitch Ran");
    grainReadOffsetSlider.setName("Offset");
    grainAttackSlider.setName("Attack");
    grainDecaySlider.setName("Decay");
    mixSlider.setName("Mix");

    // Calculate the width for each component based on the total number of components
    const float numBottomComponents = 10.0;
    
    const float spacing = bounds.getWidth() / numBottomComponents;

    const float offset = spacing / 4.0f;

    // position bottom panel sliders
    grainCountSlider.setBounds(spacing * 0 + offset, bottomPanelBounds.getY(), grainCountSlider.getSliderMinWidth(), bottomPanelBounds.getHeight());
    grainSizeSlider.setBounds(spacing * 1 + offset, bottomPanelBounds.getY(), grainSizeSlider.getSliderMinWidth(), bottomPanelBounds.getHeight());
    grainPositionRandomSlider.setBounds(spacing * 2 + offset, bottomPanelBounds.getY(), grainPositionRandomSlider.getSliderMinWidth(), bottomPanelBounds.getHeight());
    grainPitchSlider.setBounds(spacing * 3 + offset, bottomPanelBounds.getY(), grainPitchSlider.getSliderMinWidth(), bottomPanelBounds.getHeight());
    pitchRandomSlider.setBounds(spacing * 4 + offset, bottomPanelBounds.getY(), pitchRandomSlider.getSliderMinWidth(), bottomPanelBounds.getHeight());
    grainAttackSlider.setBounds(spacing * 5 + offset, bottomPanelBounds.getY(), grainAttackSlider.getSliderMinWidth(), bottomPanelBounds.getHeight());
    grainDecaySlider.setBounds(spacing * 6 + offset, bottomPanelBounds.getY(), grainDecaySlider.getSliderMinWidth(), bottomPanelBounds.getHeight());
    grainReadOffsetSlider.setBounds(spacing * 7 + offset, bottomPanelBounds.getY(), grainReadOffsetSlider.getSliderMinWidth(), bottomPanelBounds.getHeight());
    mixSlider.setBounds(spacing * 8 + offset, bottomPanelBounds.getY(), mixSlider.getSliderMinWidth(), bottomPanelBounds.getHeight());
    historyBufferSizeSlider.setBounds(spacing * 9 + offset, bottomPanelBounds.getY(), historyBufferSizeSlider.getSliderMinWidth(), bottomPanelBounds.getHeight());

}

void LoopLoopLoopAudioProcessorEditor::paintWaveform(juce::Graphics& g)
{
    auto bounds = getBounds();
    auto waveformBounds = bounds.removeFromTop(bounds.getHeight() * 0.75);
    const int localHeight = waveformBounds.getHeight();
    const int localWidth = waveformBounds.getWidth();

    auto settings = getSettings(audioProcessor.treeState);

    const double grainStartPercentage = settings.grainStart;
    const int grainStart = static_cast<int>(localWidth * grainStartPercentage);
    const double grainEndPercentage = (settings.grainSize / audioProcessor.historyBuffer[0].getUserSizeInSeconds()) + grainStartPercentage; // grainSize was a percentage but now it's a duration in seonds
    const int grainEnd = static_cast<int>(localWidth * grainEndPercentage) % localWidth;

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
        if(i >= grainStart && i <= grainEnd)
		{
			g.setColour(juce::Colour::fromFloatRGBA(0.380, 0.654f, 0.910f, 1.0f));
		}
		else
		{
			g.setColour(juce::Colour::fromFloatRGBA(0.34f, 0.34f, 0.34f, 1.0f));
		}

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

    auto settings = getSettings(audioProcessor.treeState);

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
        readIndex = wrapReadIndexToBuffer(readIndex, secondsToSamples(settings.historyBufferSize));
        const int playHead = readIndex / secondsToSamples(settings.historyBufferSize) * double(localWidth);

        const int playHeadHeightY2 = (localHeight / 2) + (audioProcessor.historyBuffer[0].grains[i].getGainFactor() * (localHeight / 2)) * .8;
        const int playHeadHeightY1 = (localHeight / 2) - (audioProcessor.historyBuffer[0].grains[i].getGainFactor() * (localHeight / 2)) * .8;

        //const int playHeadHeightY1 = (localHeight) - (audioProcessor.historyBuffer[0].grains[i].getGainFactor() * (localHeight / 2));
        //const int playHeadHeightY2 = (audioProcessor.historyBuffer[0].grains[i].getGainFactor() * (localHeight / 2)) + (localHeight);

        g.setColour(playheadColors[i % playheadColors.size()]);
        g.drawLine(playHead, playHeadHeightY1, playHead, playHeadHeightY2, 3);
	}

 //   // blue playhead
 //   g.setColour(juce::Colour::fromFloatRGBA(0.380, 0.654f, 0.910f, 1.0f));

 //   //// horizontal line
 //   //g.drawLine(grainStart, localHeight - 1, grainEnd, localHeight - 1, 3); // - 1 so it doesnt overlap with the horizontal separator line
 //   // vertical line
 //   g.drawLine(grainStart, 0, grainStart, localHeight, 3);

 //   // playhead triangles
 //   juce::Path triangle;
 //   triangle.addTriangle(grainStart - 9, 0, grainStart - 3, 0, grainStart - 3, 10);
	//g.fillPath(triangle);
 //   triangle.addTriangle(grainStart + 9, 0, grainStart + 3, 0, grainStart + 3, 10);
 //   g.fillPath(triangle);

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
        &grainPositionRandomSlider,
        &grainCountSlider
    };
}
