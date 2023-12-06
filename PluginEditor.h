/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
 We add a listener here
*/
class GainTutorialAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                          public juce::Slider::Listener
{
public:
    GainTutorialAudioProcessorEditor (GainTutorialAudioProcessor&);
    ~GainTutorialAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    // Must override the pure virtual function. Also have to call it in
    // PluginEditor.cpp
    void sliderValueChanged (juce::Slider* slider) override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    // We can use this object to change values using the GUI.
    // So we will use this to change rawVolume as declared in PluginProcessor.h
    GainTutorialAudioProcessor& audioProcessor;
    
    // Create a slider to control the volume, we have also added a listener
    // above to get the volume to respond to the slider
    juce::Slider gainSlider;
public:
    // Need to create a slider attachment from the AudioProcessorValueTreeState class
    // This will correspond to the treeState object
    // Make it a pointer, because we want to use it to modify a value in real-time
    // Make it std::unique_ptr because using a raw pointer would mean having to
    // delete the object when finished with it. This way, it will delete automatically
    // when out of scope
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sliderValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainTutorialAudioProcessorEditor)
};
