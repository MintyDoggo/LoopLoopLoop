#include "ParameterRegistration.h"
#include "Utilities.h"

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    // create parameters
    auto pMix = std::make_unique<juce::AudioParameterFloat>("mix", "Mix", juce::NormalisableRange<float>(0.0, 1.0), 1.0);
    auto pGrainStart = std::make_unique<juce::AudioParameterFloat>("grainStart", "Grain Start", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0);
    auto pGrainSize = std::make_unique<juce::AudioParameterFloat>("grainSize", "Grain Size", juce::NormalisableRange<float>(0.0, 15.0), 0.4);
    auto pGrainPitch = std::make_unique<juce::AudioParameterFloat>("grainPitch", "Grain Pitch", juce::NormalisableRange<float>(-24.0, 24.0), 0.0);
    auto pGrainAttack = std::make_unique<juce::AudioParameterFloat>("grainAttack", "Grain Attack", juce::NormalisableRange<float>(0.0, 1.0), 0.15);
    auto pGrainDecay = std::make_unique<juce::AudioParameterFloat>("grainDecay", "Grain Decay", juce::NormalisableRange<float>(0.0, 1.0), 0.85);
    auto pGrainCount = std::make_unique<juce::AudioParameterInt>("grainCount", "Grain Count", 0, MAX_NUM_GRAINS, 1);
    auto pPositionRandom = std::make_unique<juce::AudioParameterFloat>("positionRandom", "Position Random", juce::NormalisableRange<float>(0.0, 1.0), 0.0);
    auto pGrainReverse = std::make_unique<juce::AudioParameterBool>("grainReverse", "Grain Reverse", false);
    auto pUseSemitones = std::make_unique<juce::AudioParameterBool>("useSemitones", "Use Semitones", true);
    auto pWritePause = std::make_unique<juce::AudioParameterBool>("writePause", "Write Pause", false);
    auto pHistoryBufferSize = std::make_unique<juce::AudioParameterFloat>("historyBufferSize", "History Buffer Size", juce::NormalisableRange<float>(0.01, 30.0), 6.0);
    auto pGain = std::make_unique<juce::AudioParameterFloat>("gain", "Master Gain", juce::NormalisableRange<float>(0.0, 1.0), 1.0);
    auto pPitchRandom = std::make_unique<juce::AudioParameterFloat>("pitchRandom", "Pitch Random", juce::NormalisableRange<float>(0.0, 1.0), 0.0);
    auto pGrainReadOffset = std::make_unique<juce::AudioParameterFloat>("grainReadOffset", "Grain Read Offset", juce::NormalisableRange<float>(0.0, 1.0), 0.0);

    // add parameters to vector
    parameters.push_back(std::move(pMix));
    parameters.push_back(std::move(pGrainStart));
    parameters.push_back(std::move(pGrainSize));
    parameters.push_back(std::move(pGrainPitch));
    parameters.push_back(std::move(pGrainAttack));
    parameters.push_back(std::move(pGrainDecay));
    parameters.push_back(std::move(pPositionRandom));
    parameters.push_back(std::move(pGrainCount));
    parameters.push_back(std::move(pGrainReverse));
    parameters.push_back(std::move(pUseSemitones));
    parameters.push_back(std::move(pWritePause));
    parameters.push_back(std::move(pHistoryBufferSize));
    parameters.push_back(std::move(pGain));
    parameters.push_back(std::move(pPitchRandom));
    parameters.push_back(std::move(pGrainReadOffset));

    return { parameters.begin(), parameters.end() };
}

void addParameterListeners(juce::AudioProcessorValueTreeState& treeState, juce::AudioProcessorValueTreeState::Listener* listener)
{
    // add parameter listeners
    treeState.addParameterListener("gain", listener);
    treeState.addParameterListener("grainSize", listener);
    treeState.addParameterListener("grainPitch", listener);
    treeState.addParameterListener("grainStart", listener);
    treeState.addParameterListener("historyBufferSize", listener);
    treeState.addParameterListener("mix", listener);
    treeState.addParameterListener("grainAttack", listener);
    treeState.addParameterListener("grainDecay", listener);
    treeState.addParameterListener("grainCount", listener);
    treeState.addParameterListener("grainReverse", listener);
    treeState.addParameterListener("writePause", listener);
    treeState.addParameterListener("positionRandom", listener);
    treeState.addParameterListener("useSemitones", listener);
    treeState.addParameterListener("pitchRandom", listener);
    treeState.addParameterListener("grainReadOffset", listener);
}

void removeParameterListeners(juce::AudioProcessorValueTreeState& treeState, juce::AudioProcessorValueTreeState::Listener* listener)
{
    // remove parameter listeners
    treeState.removeParameterListener("gain", listener);
    treeState.removeParameterListener("grainSize", listener);
    treeState.removeParameterListener("grainPitch", listener);
    treeState.removeParameterListener("grainStart", listener);
    treeState.removeParameterListener("historyBufferSize", listener);
    treeState.removeParameterListener("mix", listener);
    treeState.removeParameterListener("grainAttack", listener);
    treeState.removeParameterListener("grainDecay", listener);
    treeState.removeParameterListener("grainCount", listener);
    treeState.removeParameterListener("grainReverse", listener);
    treeState.removeParameterListener("writePause", listener);
    treeState.removeParameterListener("positionRandom", listener);
    treeState.removeParameterListener("useSemitones", listener);
    treeState.removeParameterListener("pitchRandom", listener);
    treeState.removeParameterListener("grainReadOffset", listener);
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
    settings.grainCount = treeState.getRawParameterValue("grainCount")->load();
    settings.grainReverse = treeState.getRawParameterValue("grainReverse")->load();
    settings.writePause = treeState.getRawParameterValue("writePause")->load();
    settings.positionRandom = treeState.getRawParameterValue("positionRandom")->load();
    settings.semitoneMode = treeState.getRawParameterValue("useSemitones")->load();
    settings.pitchRandom = treeState.getRawParameterValue("pitchRandom")->load();
    settings.grainReadOffset = treeState.getRawParameterValue("grainReadOffset")->load();

    return settings;
}
