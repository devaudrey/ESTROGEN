/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "./DSP/CompressorProcess.h"
#include "./DSP/SaturationProcess.h"
#include <juce_dsp/juce_dsp.h>

//==============================================================================
/**
 */
class EstrogenAudioProcessor  : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
, public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    EstrogenAudioProcessor();
    ~EstrogenAudioProcessor() override;
    
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
    //==============================================================================
    
//    enum CompressorMode {
//
//        MONO = 1,   // Default
//        MS = 3,     // Mid–side
//        LR = 2      // Stereo De-link
//
//    };
//
//    // Overall plugin operation variables
//
//    CompressorMode mode = LR;
    
    float inputGainTrim = 0.f;         // input gain or trim
    float outputGainTrim = 0.f;        // output gain or trim
    float mixRatio = 1.f;       // Wet/dry blend of plugin (overall)
    
    
    // Saturator-dependant variables
    
    bool satBypassState = false;    //  tracks saturator bypass state
    float driveAmount = 1;          // drive values, generally 1-10 (but can do more for harder-                                        clipped sound)
    
    // Compressor-dependant variables
    
    float attack_ms = 30.f;
    float release_ms = 1000.f;
    float threshold_dB = 0.f;
    
    
    
private:
    
    SaturationProcess saturator;
    CompressorProcess compressor;
    
    juce::dsp::DryWetMixer<float> dryWetMixer;

    juce::dsp::Oversampling<float> oversampler {static_cast<size_t>(getTotalNumOutputChannels()), 2, juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple, false, true};
    
    void saturationProcess(juce::AudioBuffer<float> & buffer);
    void compressionProcess(juce::AudioBuffer<float> & buffer);
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EstrogenAudioProcessor)
};
