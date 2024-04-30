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
class EstrogenAudioProcessorEditor  : public juce::AudioProcessorEditor,
public juce::Slider::Listener, public juce::ToggleButton::Listener
{
public:
    EstrogenAudioProcessorEditor (EstrogenAudioProcessor&);
    ~EstrogenAudioProcessorEditor() override;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider * slider) override;
    void buttonClicked(juce::Button * button) override;
    
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EstrogenAudioProcessor& audioProcessor;
    CustomLNF myCustomLNF;
    
    juce::Slider inputSlider;
    juce::Slider outputSlider;
    juce::Slider driveSlider;
    juce::Slider attackSlider;
    juce::Slider releaseSlider;
    juce::Slider thresholdSlider;
    juce::Slider mixSlider;
    
    juce::ToggleButton satBypassButton;
    
    
    std::vector<std::unique_ptr<SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr<ButtonAttachment>> buttonAttachments;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EstrogenAudioProcessorEditor)
};
