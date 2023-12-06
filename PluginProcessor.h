/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
// Define plugin parameter ID and parameter name
#define GAIN_ID "gain"
#define GAIN_NAME "Gain"

//==============================================================================
/**
*/
class GainTutorialAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    GainTutorialAudioProcessor();
    ~GainTutorialAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    /* Create variable that controls gain. The way this will work is that we take
     the incoming values that will be between -1 and 1. They will be multiplied
     by this number and that will be the volume */
    double gainValue;
    
    // To make this automatable, we create a AudioProcessorValueTreeState object here
    // This class manages the AudioProcessor's entire state. This object is
    // initialised in PluginProcessor.cpp
    juce::AudioProcessorValueTreeState treeState;
    
private:
    //==============================================================================
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainTutorialAudioProcessor)
};
