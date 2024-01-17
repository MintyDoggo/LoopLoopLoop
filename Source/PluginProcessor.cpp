/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utilities.cpp"
 
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
    log.open("C:\\Users\\dog1\\Desktop\\funny-log-out.txt");

}

LoopLoopLoopAudioProcessor::~LoopLoopLoopAudioProcessor()
{
    log.close();
}

Settings LoopLoopLoopAudioProcessor::getSettings(juce::AudioProcessorValueTreeState& treeState)
{
	Settings settings;

	settings.gain = treeState.getRawParameterValue("gain")->load();
    settings.grainSize = treeState.getRawParameterValue("grainSize")->load();
    settings.grainPitch = treeState.getRawParameterValue("grainPitch")->load();
    settings.grainStart = treeState.getRawParameterValue("grainStart")->load();
    settings.historyBufferSize = treeState.getRawParameterValue("historyBufferSize")->load();
    settings.mix = treeState.getRawParameterValue("mix")->load();
    settings.grainAttack = treeState.getRawParameterValue("grainAttack")->load();
    settings.grainDecay = treeState.getRawParameterValue("grainDecay")->load();
    settings.grainCount = treeState.getRawParameterValue("grainCount")->load();
    settings.grainReverse = treeState.getRawParameterValue("grainReverse")->load();
    settings.writePause = treeState.getRawParameterValue("writePause")->load();
    settings.spread = treeState.getRawParameterValue("spread")->load();

	return settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout LoopLoopLoopAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    // create parameters
    auto pMix = std::make_unique<juce::AudioParameterFloat>("mix", "Mix", juce::NormalisableRange<float>(0.0, 1.0), 1.0);
    auto pGrainStart = std::make_unique<juce::AudioParameterFloat>("grainStart", "Grain Start", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0);
    auto pGrainSize = std::make_unique<juce::AudioParameterFloat>("grainSize", "Grain Size", juce::NormalisableRange<float>(0.0, 15.0), 0.4);
    auto pGrainPitch = std::make_unique<juce::AudioParameterFloat>("grainPitch", "Grain Pitch", juce::NormalisableRange<float>(-24.0, 24.0), 0.0);
    auto pGrainAttack = std::make_unique<juce::AudioParameterFloat>("grainAttack", "Grain Attack", juce::NormalisableRange<float>(0.0, 1.0), 0.15);
    auto pGrainDecay = std::make_unique<juce::AudioParameterFloat>("grainDecay", "Grain Decay", juce::NormalisableRange<float>(0.0, 1.0), 0.85);
    auto pGrainCount = std::make_unique<juce::AudioParameterInt>("grainCount", "Grain Count", 1, 16, 2);
    auto pSpread = std::make_unique<juce::AudioParameterFloat>("spread", "Spread", juce::NormalisableRange<float>(0.0, 1.0), 0.0);
    auto pWritePause = std::make_unique<juce::AudioParameterBool>("writePause", "Write Pause", false);
    auto pGrainReverse = std::make_unique<juce::AudioParameterBool>("grainReverse", "Grain Reverse", false);
    auto pHistoryBufferSize = std::make_unique<juce::AudioParameterFloat>("historyBufferSize", "History Buffer Size", juce::NormalisableRange<float>(0.01, 30.0), 6.0);
    auto pGain = std::make_unique<juce::AudioParameterFloat>("gain", "Master Gain", juce::NormalisableRange<float>(0.0, 1.0), 1.0);

    // add parameters to vector
    parameters.push_back(std::move(pMix));
    parameters.push_back(std::move(pGrainStart));
    parameters.push_back(std::move(pGrainSize));
    parameters.push_back(std::move(pGrainPitch));
    parameters.push_back(std::move(pGrainAttack));
    parameters.push_back(std::move(pGrainDecay));
    parameters.push_back(std::move(pSpread));
    parameters.push_back(std::move(pGrainCount));
    parameters.push_back(std::move(pGrainReverse));
    parameters.push_back(std::move(pWritePause));
    parameters.push_back(std::move(pHistoryBufferSize));
    parameters.push_back(std::move(pGain));

    return { parameters.begin(), parameters.end() };
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

//==============================================================================
// Use this method as the place to do any pre-playback
// initialisation that you need..
void LoopLoopLoopAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto settings = getSettings(treeState);

    // store changed sample rate
    LoopLoopLoopAudioProcessor::sampleRate = sampleRate;
     
    // set constants
    const int maxGrains = 16;

    // resize history buffer
    for(int channel = 0; channel < historyBuffer.size(); channel++)
	{   
        historyBuffer[channel].setSize(30.0); // necessary to prevent subscript out of range error
		historyBuffer[channel].setUserSize(settings.historyBufferSize);
		historyBuffer[channel].clear();
        historyBuffer[channel].grains.resize(maxGrains);
        historyBuffer[channel].setUserNumGrains(settings.grainCount);

        // set grain parameters
        for (int grain = 0; grain < maxGrains; grain++)
        {
            historyBuffer[channel].grains[grain].setGrainSize(settings.grainSize);
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

    // do the actual processing
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
            historyBuffer[channel].setUserSize(settings.historyBufferSize);
            historyBuffer[channel].writePause = settings.writePause;
            historyBuffer[channel].setUserNumGrains(settings.grainCount);
            //log << "settings.grainCount: " << settings.grainCount << std::endl;

            setDrawingPeaks(channelData[sample]);

            // write sample to history buffer
            historyBuffer[channel].write(channelData[sample]);
            // apply mix
            channelData[sample] *= (1.0 - settings.mix);

            // TODO: make callback for grain count change, update grain parameters so that the offsets offset in regards to the first grain

            // read each grain from history buffer (right now l and r channel use same grain settings so this can be optimized)
            for(int grain = 0; grain < historyBuffer[channel].getUserNumGrains(); grain++)
			{
                double spreadFactor;
                
                if(grain % 2 == 0)
                {
                    spreadFactor = settings.spread * double(grain);
				}
                else
                {
					spreadFactor = settings.spread * double(grain) * -1;
				}

                //log << "GRAIN: " << grain << std::endl;

                // set grain parameters
                historyBuffer[channel].grains[grain].setGrainSize(settings.grainSize);
                historyBuffer[channel].grains[grain].setStartIndex((settings.historyBufferSize * settings.grainStart) - spreadFactor);
                historyBuffer[channel].grains[grain].setGrainPitch(settings.grainPitch);
                historyBuffer[channel].grains[grain].setAttack(settings.grainAttack);
                historyBuffer[channel].grains[grain].setDecay(settings.grainDecay);
                historyBuffer[channel].grains[grain].setReverse(settings.grainReverse); 
                historyBuffer[channel].grains[grain].setReadOffset(double(grain));

				// get read index for grain
				double grainReadIndex = historyBuffer[channel].grains[grain].getReadIndex();
                wrapReadIndexToBuffer(grainReadIndex, secondsToSamples(historyBuffer[channel].getUserSizeInSeconds()));

                const double grainGainFactor = historyBuffer[channel].grains[grain].getGainFactor();

				// read sample from history buffer
				const double grainSample = (historyBuffer[channel].read(grainReadIndex) * grainGainFactor) * settings.mix;

				// write sample to output buffer
				channelData[sample] += grainSample;

				// increment read index for grain
				historyBuffer[channel].grains[grain].incrementReadIndex();
			}

            // apply gain
            channelData[sample] *= settings.gain;
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
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void LoopLoopLoopAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void LoopLoopLoopAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void LoopLoopLoopAudioProcessor::setDrawingPeaks(double sample)
{
    const float absoluteSample = std::abs(sample);

    // get max peak of buffer
    if (absoluteSample > maxPeak)
    {
        maxPeak = sample;
    }
    ++sampleCounter;

    if (sampleCounter / 2 >= detectionLength)
    {
        amplitude = maxPeak;
        maxPeak = 0.0;
        sampleCounter = 0;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LoopLoopLoopAudioProcessor();
}

