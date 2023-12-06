/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainTutorialAudioProcessorEditor::GainTutorialAudioProcessorEditor (GainTutorialAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set up sliderAttach.
    /**

     
     sliderAttach = std::make_unique<class _Tp>(_Args &&__args...)
     - class _Tp - juce::AudioProcessorValueTreeState::SliderAttachment
     - _Args &&__args... - We need the corresponding object in processort file. In this case,
     treeState. Have to do this using the audioProcessor object that is in PluginEditor.h. Also need to
     use the GAIN_ID set up as a #define in PluginProcessor.h. Finally need The Slider we want this to control. In
     this case gainSlider
     
     
     
     */
//    sliderAttach = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState, GAIN_ID, gainSlider);
    sliderValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, GAIN_ID, gainSlider);
    
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 400);
    
    
    
    // Customise slider and make visible
    gainSlider.setSliderStyle(juce::Slider::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    // Set range of slider to -48.0 to 0.0. This is in decibel sound pressure level.
    // Just like on a DAW gain slider
    gainSlider.setRange(-48.0, 0.0);
    // Initial value of slider
    gainSlider.setValue(-5.0);
    // Add listener
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider);
}

GainTutorialAudioProcessorEditor::~GainTutorialAudioProcessorEditor()
{
}

//==============================================================================
void GainTutorialAudioProcessorEditor::paint (juce::Graphics& g)
{
    
}

void GainTutorialAudioProcessorEditor::resized()
{
    // Position slider
    gainSlider.setBounds(getLocalBounds());
}

// Must also call pure virtual function here
void GainTutorialAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    // This is where we use the audioProcessor object to change the volume using the slider
    // We are using a logarithmic scale, as that is how humans hear.
    // db = 20 * log(x)
    // db/20 = log(x)
    // 10^db/20 = x
    // This is the formula we must implement. audioProcessor.rawVolume is x, so we must
    // find a way of getting 10^db/20 on the other side. Use pow function.
    // pow(double, double) where first arg is base, second is power
    if (slider == &gainSlider)
    {
        audioProcessor.gainValue = pow(10, gainSlider.getValue() / 20);
    }
}
