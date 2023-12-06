/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// This is the initialiser
GainTutorialAudioProcessor::GainTutorialAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        // Set initial gainValue to -5.0
        gainValue(-5.0f),
        // Initialise treeState treeState(AudioProcessor &processorToConnectTo, UndoManager *undoManagerToUse, const juce::Identifier &valueTreeType, ParameterLayout parameterLayout)
        // In ParameterLayout, we put std::make_unique<juce::AudioParameterFloat>(GAIN_ID, GAIN_NAME, -48.0f, 0.0f, -15.0f)}. Can keep adding more in the same way

treeState(*this, nullptr, "PARAMETER", {std::make_unique<juce::AudioParameterFloat>(GAIN_ID, GAIN_NAME, -48.0f, 0.0f, -15.0f)})

// This is the constructor
#endif
{
}

GainTutorialAudioProcessor::~GainTutorialAudioProcessor()
{
}

//==============================================================================
const juce::String GainTutorialAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GainTutorialAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GainTutorialAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GainTutorialAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GainTutorialAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainTutorialAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GainTutorialAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GainTutorialAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GainTutorialAudioProcessor::getProgramName (int index)
{
    return {};
}

void GainTutorialAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GainTutorialAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void GainTutorialAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainTutorialAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GainTutorialAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Instantiate rawVolume value
//    rawVolume = 0.015;
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto channelData = buffer.getWritePointer (channel);
        auto sliderGainValue = treeState.getRawParameterValue(GAIN_ID)->load();

        // ..do something to the data...
        // We need to find a sample, take it and multiply it by our volume in order
        // to change it. buffer.getNumSamples() returns the buffer size.
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // We need to multiply all the samples in channelData by our volume variable
            // we use buffer.getSample(int channel, int sampleIndex). We get the channel, which
            // will be 0 or 1 for left and right. We then multiply it by gainValue. This will
            // iterate over the channels and be applied to every single sample
            // This equation makes plugin logarithmic
//            channelData[sample] = buffer.getSample(channel, sample) * (pow(10, gainValue / 20));
            // This is a simpler way of converting to decibels
            channelData[sample] = buffer.getSample(channel, sample) * juce::Decibels::decibelsToGain(sliderGainValue);
        }
    }
}

//==============================================================================
bool GainTutorialAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GainTutorialAudioProcessor::createEditor()
{
    return new GainTutorialAudioProcessorEditor (*this);
}

//==============================================================================
void GainTutorialAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GainTutorialAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainTutorialAudioProcessor();
}
