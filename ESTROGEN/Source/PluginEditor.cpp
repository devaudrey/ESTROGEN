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
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    orderButton.setToggleable(true);
    addAndMakeVisible(orderButton);
    setSize (400, 300);

    
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
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 2);
}

void EstrogenAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor...
    
    orderButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 25, 100, 50);
    
}
