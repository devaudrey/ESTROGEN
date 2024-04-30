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

    setSize (580, 360);
    
    inputSlider.setBounds(30, 20, 30, 300);
    inputSlider.setRange(-30.0, 30.0, 0.1);
    inputSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    inputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    inputSlider.setTextValueSuffix(" dB");
    inputSlider.setDoubleClickReturnValue(true, 0.f);
    addAndMakeVisible(inputSlider);
    inputSlider.addListener(this);
    
    outputSlider.setBounds(520, 20, 30, 300);
    outputSlider.setRange(-30.0, 30.0, 0.1);
    outputSlider.setSkewFactor(1.0);
    outputSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    outputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    outputSlider.setTextValueSuffix(" dB");
    outputSlider.setDoubleClickReturnValue(true, 0.f);
    addAndMakeVisible(outputSlider);
    outputSlider.addListener(this);

    
    driveSlider.setBounds(70, 95, 200, 200);
    driveSlider.setRange(1.0, 11.0, 0.1);
    driveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 25);
    driveSlider.setDoubleClickReturnValue(true, 1.f);
    addAndMakeVisible(driveSlider);
    driveSlider.addListener(this);
    
    satBypassButton.setBounds(100, 300, 30, 30);
    satBypassButton.setEnabled(true);
    addAndMakeVisible(satBypassButton);
    satBypassButton.addListener(this);
    
    attackSlider.setBounds(270, 20, 90, 90);
    attackSlider.setRange(0.020, 5000.0, 0.1);
    attackSlider.setSkewFactorFromMidPoint(250.0);
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 25);
    attackSlider.setTextValueSuffix(" ms");
    attackSlider.setDoubleClickReturnValue(true, 250.f);
    addAndMakeVisible(attackSlider);
    attackSlider.addListener(this);
    
    
    releaseSlider.setBounds(270, 125, 90, 90);
    releaseSlider.setRange(1.0, 10000.0, 1.0);
    releaseSlider.setSkewFactorFromMidPoint(500.0);
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 25);
    releaseSlider.setTextValueSuffix(" ms");
    releaseSlider.setDoubleClickReturnValue(true, 500.f);
    addAndMakeVisible(releaseSlider);
    releaseSlider.addListener(this);
    
    
    thresholdSlider.setBounds(270, 230, 90, 90);
    thresholdSlider.setRange(-30.0, 0.0, 0.1);
    thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 25);
    thresholdSlider.setTextValueSuffix(" dB");
    thresholdSlider.setDoubleClickReturnValue(true, 0.f);
    addAndMakeVisible(thresholdSlider);
    thresholdSlider.addListener(this);
    
    mixSlider.setBounds(400, 125, 90, 90);
    mixSlider.setRange(0.00, 1.00, 0.01);
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 25);
    mixSlider.setDoubleClickReturnValue(true, 1.f);
    addAndMakeVisible(mixSlider);
    mixSlider.addListener(this);
    
    
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,EstrogenAudioProcessor::KNOB1,inputSlider));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,EstrogenAudioProcessor::KNOB2,outputSlider));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,EstrogenAudioProcessor::KNOB3,driveSlider));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,EstrogenAudioProcessor::KNOB4,attackSlider));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,EstrogenAudioProcessor::KNOB5,releaseSlider));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,EstrogenAudioProcessor::KNOB6,thresholdSlider));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,EstrogenAudioProcessor::KNOB7,mixSlider));
    
    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,EstrogenAudioProcessor::BUTTON1,satBypassButton));
    
    
    
    //    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
}

EstrogenAudioProcessorEditor::~EstrogenAudioProcessorEditor()
{
}

//==============================================================================
void EstrogenAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::forestgreen);
//    g.setColour(juce::TextEditor::outlineColourId, juce::Colours::red.withAlpha(0.0f)); // transparent red
    g.setColour (juce::Colours::beige);
    g.setFont (15.0f);
    g.drawFittedText("INPUT", 25, 330, 60, 25, juce::Justification::left, 1);
    g.drawFittedText("OUTPUT", 520, 330, 60, 25, juce::Justification::left, 1);
    g.drawFittedText("DRIVE", 165, 185, 70, 25, juce::Justification::left, 1);
    g.drawFittedText("ATTACK", 280, 5, 70, 25, juce::Justification::left, 1);
    g.drawFittedText("RELEASE", 280, 110, 70, 25, juce::Justification::left, 1);
    g.drawFittedText("THRESHOLD", 280, 215, 70, 25, juce::Justification::left, 1);
    g.drawFittedText("MIX", 430, 105, 70, 25, juce::Justification::left, 1);
    
    
//    attackSlider.setBounds(270, 20, 90, 90);
//    releaseSlider.setBounds(270, 125, 90, 90);
//    thresholdSlider.setBounds(270, 230, 90, 90);
//     mixSlider.setBounds(400, 125, 90, 90);
//  satBypassButton.setBounds(100, 300, 30, 30);
    
    
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
        audioProcessor.mixRatio = mixSlider.getValue();
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
