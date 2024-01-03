/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utilities.h"
 
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
}

LoopLoopLoopAudioProcessor::~LoopLoopLoopAudioProcessor()
{
}

Settings getSettings(juce::AudioProcessorValueTreeState& treeState)
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
    settings.grainReverse = treeState.getRawParameterValue("grainReverse")->load();

	return settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout LoopLoopLoopAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    // create parameters
    auto pGain = std::make_unique<juce::AudioParameterFloat>("gain", "Master Gain", 0.0, 1.0, 1.0);
    auto pGrainSize = std::make_unique<juce::AudioParameterFloat>("grainSize", "Grain Size", 0.0, 1.0, 0.2);
    auto pGrainPitch = std::make_unique<juce::AudioParameterFloat>("grainPitch", "Grain Pitch", -24.0, 24.0, 0.0);
    auto pGrainStart = std::make_unique<juce::AudioParameterFloat>("grainStart", "Grain Start", 0.0, 1.0, 0.0);
    auto pHistoryBufferSize = std::make_unique<juce::AudioParameterFloat>("historyBufferSize", "History Buffer Size", 0.01, 10.0, 2.0);
    auto pMix = std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0, 1.0, 0.8);
    auto pGrainAttack = std::make_unique<juce::AudioParameterFloat>("grainAttack", "Grain Attack", 0.0, 1.0, 0.1);
    auto pGrainDecay = std::make_unique<juce::AudioParameterFloat>("grainDecay", "Grain Decay", 0.0, 1.0, 1.0);
    auto pGrainReverse = std::make_unique<juce::AudioParameterBool>("grainReverse", "Grain Reverse", false);

    // add parameters to vector
    parameters.push_back(std::move(pGain));
    parameters.push_back(std::move(pGrainSize));
    parameters.push_back(std::move(pGrainPitch));
    parameters.push_back(std::move(pGrainStart));
    parameters.push_back(std::move(pHistoryBufferSize));
    parameters.push_back(std::move(pMix));
    parameters.push_back(std::move(pGrainAttack));
    parameters.push_back(std::move(pGrainDecay));
    parameters.push_back(std::move(pGrainReverse));

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
    const int numGrains = 1;

    // resize history buffer
    for(int channel = 0; channel < historyBuffer.size(); channel++)
	{
		historyBuffer[channel].setSize(settings.historyBufferSize);
		historyBuffer[channel].clear();
        historyBuffer[channel].grains.resize(numGrains);

        // set grain parameters
        for (int grain = 0; grain < historyBuffer[channel].getNumGrains(); grain++)
        {
            historyBuffer[channel].grains[grain].setGrainSize(settings.grainSize);
            historyBuffer[channel].grains[grain].setStartIndex(settings.historyBufferSize * settings.grainStart);
            historyBuffer[channel].grains[grain].setGrainPitch(settings.grainPitch);
        }
	}

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
        historyBuffer[channel].setSize(settings.historyBufferSize);
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
            // write sample to history buffer
            historyBuffer[channel].write(channelData[sample]);
            // apply mix
            channelData[sample] *= (1.0 - settings.mix);

            // read each grain from history buffer
            for(int grain = 0; grain < historyBuffer[channel].getNumGrains(); grain++)
			{
                // set grain parameters
                historyBuffer[channel].grains[grain].setGrainSize(settings.grainSize);
                historyBuffer[channel].grains[grain].setStartIndex(settings.historyBufferSize * settings.grainStart);
                historyBuffer[channel].grains[grain].setGrainPitch(settings.grainPitch);
                historyBuffer[channel].grains[grain].setAttack(settings.grainAttack);
                historyBuffer[channel].grains[grain].setDecay(settings.grainDecay);
                historyBuffer[channel].grains[grain].setReverse(settings.grainReverse);

				// get read index for grain
				const int grainReadIndex = historyBuffer[channel].grains[grain].getReadIndex();

				// read sample from history buffer
                double attackMultiplier = 1.0;
                double decayMultiplier = 1.0;

                // if not passed point of attack, apply attack multiplier
                if (historyBuffer[channel].grains[grain].getAttack() > historyBuffer[channel].grains[grain].getProgress())
                {
                    const double attackProgress = 1.0 - (historyBuffer[channel].grains[grain].getAttack() - historyBuffer[channel].grains[grain].getProgress());
                    attackMultiplier = linearInterpolation(0.0, 1.0, attackProgress);
                }

                // if passed point of decay, apply decay multiplier
                if (historyBuffer[channel].grains[grain].getDecay() < historyBuffer[channel].grains[grain].getProgress())
                {
                    const double decayProgress = (historyBuffer[channel].grains[grain].getProgress() - historyBuffer[channel].grains[grain].getDecay()) / (1.0 - historyBuffer[channel].grains[grain].getDecay());
                    decayMultiplier = linearInterpolation(1.0, 0.0, decayProgress);
				}

				const double grainSample = (historyBuffer[channel].read(grainReadIndex) * attackMultiplier * decayMultiplier) * settings.mix;

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
    //return new LoopLoopLoopAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
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

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LoopLoopLoopAudioProcessor();
}
