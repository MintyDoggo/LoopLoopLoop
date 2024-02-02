/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utilities.h"
#include "ParameterRegistration.h"

double LoopLoopLoopAudioProcessor::sampleRate; // Provide a default value if necessary

//==============================================================================
LoopLoopLoopAudioProcessor::LoopLoopLoopAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    historyBuffer.resize(2);
    
    // add listeners 
    addParameterListeners(treeState, this);
    log.open("C:\\Users\\dog1\\Desktop\\funny-log-out.txt");
}

LoopLoopLoopAudioProcessor::~LoopLoopLoopAudioProcessor()
{
    // remove listeners
    removeParameterListeners(treeState, this);
    log.close();
}

//void LoopLoopLoopAudioProcessor::setGrainParameters(const juce::String& parameterID, float newValue)
//{
//}

void LoopLoopLoopAudioProcessor::updateGrainParameters()
{
    auto settings = getSettings(treeState);
    //historyBuffer[channel].grains[grain].setReadOffset(double(grain));

    for (int grain = 0; grain < historyBuffer[0].userNumGrains; grain++)
    {
        historyBuffer[0].grains[grain].size = settings.grainSize;
        historyBuffer[1].grains[grain].size = settings.grainSize;

        if (settings.semitoneMode)
        {
            historyBuffer[0].grains[grain].setGrainPitch(settings.grainPitch);
            historyBuffer[1].grains[grain].setGrainPitch(settings.grainPitch);
        }
        else
        {
            historyBuffer[0].grains[grain].setGrainPitch(remapValue(settings.grainPitch, -24, 24, -8, 8));
            historyBuffer[1].grains[grain].setGrainPitch(remapValue(settings.grainPitch, -24, 24, -8, 8));
        }

        historyBuffer[0].grains[grain].setStartIndex(settings.historyBufferSize * settings.grainStart);
        historyBuffer[1].grains[grain].setStartIndex(settings.historyBufferSize * settings.grainStart);

        historyBuffer[0].grains[grain].attack = settings.grainAttack;
        historyBuffer[1].grains[grain].attack = settings.grainAttack;

        historyBuffer[0].grains[grain].decay = settings.grainDecay;
        historyBuffer[1].grains[grain].decay = settings.grainDecay;

        historyBuffer[0].grains[grain].reverse = settings.grainReverse;
        historyBuffer[1].grains[grain].reverse = settings.grainReverse;

        historyBuffer[0].grains[grain].semitoneMode = settings.semitoneMode;
        historyBuffer[1].grains[grain].semitoneMode = settings.semitoneMode;

        historyBuffer[0].grains[grain].randomPitchMax = settings.pitchRandom;
        historyBuffer[1].grains[grain].randomPitchMax = settings.pitchRandom;

        historyBuffer[0].grains[grain].randomStartOffsetMax = settings.positionRandom;
        historyBuffer[1].grains[grain].randomStartOffsetMax = settings.positionRandom;

        historyBuffer[0].grains[grain].setReadOffset(grain * settings.grainReadOffset * settings.grainSize); // todo: broken
        historyBuffer[1].grains[grain].setReadOffset(grain * settings.grainReadOffset * settings.grainSize);

    }
}

void LoopLoopLoopAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue) // is using float here ok? todo
{
	//log << "parameter changed: " << parameterID << std::endl;
	//log << "new value: " << newValue << std::endl;

    // get settings
	auto settings = getSettings(treeState);

	// set grain parameters
    if (parameterID == "gain")
    {
        settings.gain = newValue;
    }

    if (parameterID == "grainSize")
	{
        updateGrainParameters();
	}
    
    if (parameterID == "grainPitch")
    {
        updateGrainParameters();
    }

    if (parameterID == "grainStart")
    {
        updateGrainParameters();
    }

    if (parameterID == "historyBufferSize")
	{
        historyBuffer[0].setUserSize(settings.historyBufferSize);
        historyBuffer[1].setUserSize(settings.historyBufferSize);

        //settings.historyBufferSize = newValue;
	}

	if (parameterID == "mix")
	{
		settings.mix = newValue;
	}

	if (parameterID == "grainAttack")
	{
        updateGrainParameters();
    }

	if (parameterID == "grainDecay")
	{
        updateGrainParameters();
    }

    if (parameterID == "grainCount")
    {
        historyBuffer[0].userNumGrains = settings.grainCount;
        historyBuffer[1].userNumGrains = settings.grainCount;

        updateGrainParameters();

		//settings.grainCount = newValue;
	}

    if (parameterID == "grainReverse")
    {
        updateGrainParameters();
    }

    if (parameterID == "writePause")
    {
        historyBuffer[0].writePause = settings.writePause;
		historyBuffer[1].writePause = settings.writePause;
		//settings.writePause = newValue;
	}

	if (parameterID == "positionRandom")
	{
        updateGrainParameters();
    }

	if (parameterID == "useSemitones")
	{
        updateGrainParameters();
    }

	if (parameterID == "pitchRandom")
	{
        updateGrainParameters();
    }

}

//==============================================================================
const juce::String LoopLoopLoopAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LoopLoopLoopAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LoopLoopLoopAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LoopLoopLoopAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LoopLoopLoopAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LoopLoopLoopAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LoopLoopLoopAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LoopLoopLoopAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LoopLoopLoopAudioProcessor::getProgramName (int index)
{
    return {};
}

void LoopLoopLoopAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void LoopLoopLoopAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto settings = getSettings(treeState);

    // store changed sample rate
    LoopLoopLoopAudioProcessor::sampleRate = sampleRate;
    std::srand(std::time(nullptr)); // seed random number generator)

    // set seed of each grain random number generator
	for (int grain = 0; grain < MAX_NUM_GRAINS; grain++)
	{
        juce::int64 seed = std::rand();
		historyBuffer[0].grains[grain].pitchRandomizer.setSeed(seed);
        historyBuffer[1].grains[grain].pitchRandomizer.setSeed(seed);
        seed = std::rand();
        historyBuffer[0].grains[grain].startOffsetRandomizer.setSeed(seed);
        historyBuffer[1].grains[grain].startOffsetRandomizer.setSeed(seed);
	}

    // resize history buffer
    for(int channel = 0; channel < historyBuffer.size(); channel++)
	{
        historyBuffer[channel].setSize(30.0); // necessary to prevent subscript out of range error
		historyBuffer[channel].setUserSize(settings.historyBufferSize);
		historyBuffer[channel].clear();
        historyBuffer[channel].grains.resize(MAX_NUM_GRAINS);
        historyBuffer[channel].userNumGrains = settings.grainCount;

        // set grain parameters todo: not all parameters are set here
        for (int grain = 0; grain < MAX_NUM_GRAINS; grain++)
        {
            historyBuffer[channel].grains[grain].size = settings.grainSize;
            historyBuffer[channel].grains[grain].setStartIndex(settings.historyBufferSize * settings.grainStart);
            historyBuffer[channel].grains[grain].setGrainPitch(settings.grainPitch);
        }
	}

    //detectionLength = historyBuffer[0].getUserSize() / 1028; // no

}

void LoopLoopLoopAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LoopLoopLoopAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

// This is the function that does the actual processing
void LoopLoopLoopAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // clear all output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto settings = getSettings(treeState);
    grainGainNormalizer = 1.0 / std::sqrt(double(settings.grainCount));

    // do the actual processing
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            calculateDetectionLength(LoopLoopLoopAudioProcessor::sampleRate, historyBuffer[channel].getUserSizeInSeconds());
            calculateRMS(channelData[sample], sample % 2);

            // write sample to history buffer
            historyBuffer[channel].write(channelData[sample]);
            // apply mix
            channelData[sample] *= (1.0 - settings.mix);
        }
  
        // read each grain from history buffer (right now l and r channel use same grain settings so this can be optimized)
        for (int grain = 0; grain < historyBuffer[channel].userNumGrains; grain++)
        {
            const double grainGainFactor = historyBuffer[channel].grains[grain].getGainFactor() * grainGainNormalizer;
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                //// if grain has reset, set random start offset and pitch random. TODO: MOVE THIS TO LISTENER
                //if (historyBuffer[channel].grains[grain].grainHasReset)
                //{
                //    historyBuffer[channel].grains[grain].setRandomStartOffset(getRandomDouble(0.0, settings.positionRandom * settings.historyBufferSize)); // todo: might be unsafe (out of bounds)
                //    historyBuffer[channel].grains[grain].setStartIndex(settings.historyBufferSize * settings.grainStart);
                //    historyBuffer[channel].grains[grain].pitchRandom = getRandomDouble(0.0, settings.pitchRandom);
                //}


                //log << "settings.grainCount: " << settings.grainCount << std::endl;

                // TODO: make callback for grain count change, update grain parameters so that the offsets offset in regards to the first grain

                // get read index for grain
                double grainReadIndex = historyBuffer[channel].grains[grain].getReadIndex();
                //wrapReadIndexToBuffer(grainReadIndex, secondsToSamples(historyBuffer[channel].getUserSizeInSeconds())); // todo: this can be deleted

                // read sample from history buffer
                const double grainSample = (historyBuffer[channel].read(grainReadIndex) * grainGainFactor) * settings.mix;

                // write sample to output buffer
                channelData[sample] += grainSample * settings.gain; // todo: settings.gain funny man not good man

                // increment read index for grain
                historyBuffer[channel].grains[grain].incrementReadIndex(settings.historyBufferSize);
            }
        }

    }
}

//==============================================================================
bool LoopLoopLoopAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LoopLoopLoopAudioProcessor::createEditor()
{
    return new LoopLoopLoopAudioProcessorEditor (*this);
    // return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void LoopLoopLoopAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    juce::MemoryOutputStream mos(destData, true);
    treeState.state.writeToStream(mos);
}

void LoopLoopLoopAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
	{
        treeState.replaceState(tree);
	}
}

void LoopLoopLoopAudioProcessor::calculateRMS(double sample, int shouldIncrement)
{
    RMSCombinedSum += (sample * sample); // add squared sample to sum

    if (sampleCounter >= detectionLength)
    {
        //RMSCombinedSum *= 0.5; // divide sum by 2
        double meanSquared = RMSCombinedSum / detectionLength; // divide sum by detection length to get mean squared
        RMSValue = std::sqrt(meanSquared); // calculate and set rms
        amplitude = RMSValue; // set amplitude

		RMSCombinedSum = 0.0;
		sampleCounter = 0;
	}
    else if (shouldIncrement == 1)
    {
		sampleCounter++; // increment counter
	}
}

void LoopLoopLoopAudioProcessor::calculateDetectionLength(double sampleRate, double historyBufferSize)
{
    const double scaleFactor = 0.003;
	detectionLength = sampleRate * historyBufferSize * scaleFactor;
    //log << "detectionLength: " << detectionLength << std::endl;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LoopLoopLoopAudioProcessor();
}

