/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LoopLoopLoopAudioProcessorEditor::LoopLoopLoopAudioProcessorEditor (LoopLoopLoopAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    startTimerHz(240);
    log.open("C:\\Users\\dog1\\Desktop\\funny-log-out.txt");
    setSize(width, height + 10);
    setResizable(false, false);
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
    g.fillAll(juce::Colour::fromFloatRGBA(0.098f, 0.098f, 0.110f, 1.0f));
    paintWaveform(g);
    paintGrainWindow(g);
    //paintVerticalLine(g);
    //paintFPS(g);
    //seizureTest(g);

    paintCount++;
}

void LoopLoopLoopAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void LoopLoopLoopAudioProcessorEditor::paintWaveform(juce::Graphics& g)
{
    //painting the waveform
    const int ampHeight = static_cast<int>(audioProcessor.amplitude * (height / 2 - padding));
    const int indexToPaint = static_cast<int>(audioProcessor.historyBuffer[0].getProgress() * double(width)) - 1; // todo: static cast instead of function style cast
    
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
        for (int i = previousIndexToPaint + 1; i <= width; i++)
        {
            const double progress = double(i - previousIndexToPaint) / double(width - previousIndexToPaint); // TODO: i have no clue of this works
            const int lerpAmp = linearInterpolation(rectArray[previousIndexToPaint], ampHeight, progress);
            rectArray[i] = lerpAmp;
        }

        // paint the beginning of the buffer
        for (int i = 0; i <= indexToPaint; i++)
        {
			const double progress = double(i) / double(indexToPaint);
			const int lerpAmp = linearInterpolation(rectArray[width], ampHeight, progress);
			rectArray[i] = lerpAmp;
		}
    }


    for (int i = 0; i < width; i++)
    {
        g.setColour(juce::Colour::fromFloatRGBA(0.55f, 0.55f, 0.55f, 1.0f));
        //g.fillRect(i, height - rectArray[i], 1, rectArray[i]);

        int x1 = i;
        int y1 = (height / 2 + padding / 2) - rectArray[i];
        int x2 = i;
        int y2 = (height / 2 + padding / 2) + rectArray[i];

        if (i <= width)
        {
            x2 = i + 1;
            y2 = (height / 2 + padding / 2) + rectArray[i + 1];
        }

        g.drawLine(x1, y1, x2, y2, 1);

       /* juce::Line<float> line;

        line.setStart(i, (height / 2 + padding / 2) - rectArray[i]);

        if (i < width - 1)
        {
			line.setEnd(i + 1, (height / 2 + padding / 2) - rectArray[i + 1]);
		}
        else
        {
			line.setEnd(i, (height / 2 + padding / 2) - rectArray[i]);
		}


        g.drawLine(line);*/
    }

    previousIndexToPaint = indexToPaint;
    // we want to not increment at all sometimes to allow the whole history buffer to be shown
    //if (audioProcessor.sampleCounter >= audioProcessor.detectionLength) // this is where we can put a condition to only increment the index when we want to
    if(true)
    {
        if (audioProcessor.peakIndexToPaint < width)
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
    auto settings = audioProcessor.getSettings(audioProcessor.treeState);

    const double grainStartPercentage = settings.grainStart;
    const int grainStart = static_cast<int>(width * grainStartPercentage);

    const double grainEndPercentage = (settings.grainSize / audioProcessor.historyBuffer[0].getUserSizeInSeconds()) + grainStartPercentage; // grainSize was a percentage but now it's a duration in seonds
    const int grainEnd = static_cast<int>(width * grainEndPercentage) % width;


    const int writeHead = audioProcessor.historyBuffer[0].getProgress() * width;

    // draw area where grains are read from
    g.setColour(juce::Colour::fromFloatRGBA(0.447f, 0.537f, 0.855f, 1.0f));
    g.drawLine(grainStart, height + padding / 2, grainEnd, height + padding / 2, 2);

    // TODO: Make stereo
    // paint each grain window read index, height is determined by the gain factor and is centered around the middle of the window
    for (int i = 0; i < audioProcessor.historyBuffer[0].getUserNumGrains(); i++)
    {
        //const int playHead = static_cast<int>(grainStart + (width * (grainEndPercentage - grainStartPercentage) * audioProcessor.historyBuffer[0].grains[i].getProgress())) % width;
        
        double readIndex = audioProcessor.historyBuffer[0].grains[i].getReadIndex();
        wrapReadIndexToBuffer(readIndex, secondsToSamples(settings.historyBufferSize));
        const int playHead = readIndex / secondsToSamples(settings.historyBufferSize) * double(width);

        // account for padding
        const int playHeadHeightY1 = (height / 2) - (audioProcessor.historyBuffer[0].grains[i].getGainFactor() * (height / 2)) + padding;
        const int playHeadHeightY2 = (height / 2) + (audioProcessor.historyBuffer[0].grains[i].getGainFactor() * (height / 2));

        g.setColour(playheadColors[i % playheadColors.size()]);
        g.drawLine(playHead, playHeadHeightY1, playHead, playHeadHeightY2, 3);

	}


    // paint the write index
    g.setColour(juce::Colours::white);
    g.drawLine(writeHead, 0, writeHead, height + padding, 2);
}

void LoopLoopLoopAudioProcessorEditor::paintVerticalLine(juce::Graphics& g)
{
	g.setColour(juce::Colours::white);
    g.drawVerticalLine(paintCount % width, padding, height);
}

void LoopLoopLoopAudioProcessorEditor::seizureTest(juce::Graphics& g)
{
 //   if (paintCount % 4 == 0)
 //   {
 //       g.fillAll(juce::Colours::black);
	//}
 //   else if (paintCount % 4 == 1)
 //   {
	//	g.setColour(juce::Colours::red);
	//}
 //   else if (paintCount % 4 == 2)
 //   {
	//	g.setColour(juce::Colours::green);
	//}
	//else if (paintCount % 4 == 3)
 //   {
	//	g.setColour(juce::Colours::blue);
 //   }
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
