/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "StyleSheet.h"
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
/**
*/
class EstrogenAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    EstrogenAudioProcessorEditor (EstrogenAudioProcessor&);
    ~EstrogenAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EstrogenAudioProcessor& audioProcessor;
    juce::ToggleButton orderButton {"Swap Process Order"};
    CustomLNF myCustomLNF;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EstrogenAudioProcessorEditor)
};
