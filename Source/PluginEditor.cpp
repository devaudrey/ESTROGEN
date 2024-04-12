/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EstrogenAudioProcessorEditor::EstrogenAudioProcessorEditor (EstrogenAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize (700, 350);
    
    inputSlider.setBounds(10, 10, 30, 300);
    inputSlider.setRange(-30.0, 30.0, 0.1);
    inputSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    inputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 25);
    addAndMakeVisible(inputSlider);
    inputSlider.addListener(this);
    
    driveSlider.setBounds(50, 75, 200, 200);
    driveSlider.setRange(1.0, 11.0, 0.1);
    driveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 25);
    addAndMakeVisible(driveSlider);
    driveSlider.addListener(this);
    
    satBypassButton.setBounds(150, 300, 30, 30);
    addAndMakeVisible(satBypassButton);
    satBypassButton.addListener(this);
    
    attackSlider.setBounds(270, 10, 90, 90);
    attackSlider.setRange(0.020, 5000.0, 0.1);
    attackSlider.setSkewFactorFromMidPoint(250.0);
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 25);
    addAndMakeVisible(attackSlider);
    attackSlider.addListener(this);
    
    releaseSlider.setBounds(270, 115, 90, 90);
    releaseSlider.setRange(1.0, 10000.0, 1.0);
    releaseSlider.setSkewFactorFromMidPoint(500.0);
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 25);
    addAndMakeVisible(releaseSlider);
    releaseSlider.addListener(this);
    
    thresholdSlider.setBounds(270, 220, 90, 90);
    thresholdSlider.setRange(-30.0, 0.0, 0.1);
    thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 25);
    addAndMakeVisible(thresholdSlider);
    thresholdSlider.addListener(this);
    
    outputSlider.setBounds(660, 10, 30, 300);
    outputSlider.setRange(-30.0, 30.0, 0.1);
    outputSlider.setSkewFactor(1.0);
    outputSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    outputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 25);
    addAndMakeVisible(outputSlider);
    outputSlider.addListener(this);

    
    
    
    //    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
}

EstrogenAudioProcessorEditor::~EstrogenAudioProcessorEditor()
{
}

//==============================================================================
void EstrogenAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    g.setColour (juce::Colours::cornflowerblue);
    g.setFont (15.0f);
    
    
}

void EstrogenAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor...
    
  
}

void EstrogenAudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
 
    if (slider == &inputSlider){
        audioProcessor.inputGainTrim = inputSlider.getValue();
    }
    if (slider == &outputSlider){
        audioProcessor.outputGainTrim = outputSlider.getValue();
    }
    if (slider == &mixSlider){
        audioProcessor.mixGain = mixSlider.getValue();
    }
    if (slider == &attackSlider){
        audioProcessor.attack_ms = attackSlider.getValue();
    }
    if (slider == &releaseSlider){
        audioProcessor.release_ms = releaseSlider.getValue();
    }
    if (slider == &driveSlider){
        audioProcessor.driveAmount = driveSlider.getValue();
    }
    if (slider == &thresholdSlider){
        audioProcessor.threshold_dB = thresholdSlider.getValue();
    }
    
}

void EstrogenAudioProcessorEditor::buttonClicked(juce::Button * button) {
 
    if (button == &satBypassButton){
        audioProcessor.satBypassState = satBypassButton.getToggleState();
    }
    
}
