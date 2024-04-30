/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//using Normalise = juce::NormalisableRange<float>;

const juce::StringRef EstrogenAudioProcessor::KNOB1 = "KNOB1";
const juce::StringRef EstrogenAudioProcessor::KNOB2 = "KNOB2";
const juce::StringRef EstrogenAudioProcessor::KNOB3 = "KNOB3";
const juce::StringRef EstrogenAudioProcessor::KNOB4 = "KNOB4";
const juce::StringRef EstrogenAudioProcessor::KNOB5 = "KNOB5";
const juce::StringRef EstrogenAudioProcessor::KNOB6 = "KNOB6";
const juce::StringRef EstrogenAudioProcessor::KNOB7 = "KNOB7";


const juce::StringRef EstrogenAudioProcessor::BUTTON1 = "BUTTON1";

//==============================================================================
EstrogenAudioProcessor::EstrogenAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                  ),
#endif
apvts(*this,nullptr,"Params",createParams())
{
}


EstrogenAudioProcessor::~EstrogenAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout EstrogenAudioProcessor::createParams(){
    
    //    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // ParameterID: tag for DAW
    // String for user/automation lane
    // Min value
    // Max value
    // Starting value
    
    // less typing
    using namespace std;
    using Normalise = juce::NormalisableRange<float>;
    
    
    
    // then for all your params you add like this
    layout.add(make_unique<juce::AudioParameterFloat>(juce::ParameterID{KNOB1,ParameterVersionHint},"Input", -30.f, 30.f, 0.f));
    layout.add(make_unique<juce::AudioParameterFloat>(juce::ParameterID{KNOB2,ParameterVersionHint},"Output", -30.f, 30.f, 0.f));
    layout.add(make_unique<juce::AudioParameterFloat>(juce::ParameterID{KNOB3,ParameterVersionHint},"Drive", 1.f, 11.f, 1.f));
    layout.add(make_unique<juce::AudioParameterFloat>(juce::ParameterID{KNOB4,ParameterVersionHint},"Attack", 0.02f, 5000.f, 250.f));
    layout.add(make_unique<juce::AudioParameterFloat>(juce::ParameterID{KNOB5,ParameterVersionHint},"Release", 1.f, 10000.f, 500.f));
    layout.add(make_unique<juce::AudioParameterFloat>(juce::ParameterID{KNOB6,ParameterVersionHint},"Threshold", -30.f, 0.f, 0.f));
    layout.add(make_unique<juce::AudioParameterFloat>(juce::ParameterID{KNOB7,ParameterVersionHint},"Mix", 0.f, 1.f, 1.f));
    
    layout.add(make_unique<juce::AudioParameterBool>(juce::ParameterID{BUTTON1,ParameterVersionHint},"Bypass Saturation", false));
    
    return layout;
}

//==============================================================================
const juce::String EstrogenAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EstrogenAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool EstrogenAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool EstrogenAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double EstrogenAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EstrogenAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int EstrogenAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EstrogenAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String EstrogenAudioProcessor::getProgramName (int index)
{
    return {};
}

void EstrogenAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void EstrogenAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // oversampling preperation
    double oversampleRate = sampleRate * 4.0;
    
    oversampler.numChannels = getTotalNumInputChannels();
    oversampler.initProcessing(samplesPerBlock);
    oversampler.reset();
    
    saturator.prepareToPlay(oversampleRate);
    compressor.prepareToPlay(sampleRate);
    
    setLatencySamples(oversampler.getLatencyInSamples());
    
    // Prepare the DryWetMixer
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels(); // Assuming same number of channels for input and output
    dryWetMixer.prepare(spec);
    
    // prepare alpha
    alpha = std::exp(-std::log(9.f)/(sampleRate*respTime));
    
}

void EstrogenAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EstrogenAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void EstrogenAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    //    if (totalNumInputChannels == 1) {
    //        mode = MONO;
    //    }
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    saturator.setInputGain(inputGainTrim);
    saturator.setDriveAmount(driveAmount);
    
    compressor.setThreshold(threshold_dB);
    compressor.setRatio(3);
    compressor.setAttack(attack_ms);
    compressor.setRelease(release_ms);
    compressor.setMakeupGain(outputGainTrim);
    
    juce::dsp::AudioBlock<float> block(buffer);
    auto N = buffer.getNumSamples();
    auto N_oversample = buffer.getNumSamples() * 4.0;
    
    
    //==============================================================================
    // Test if Saturator is bypassed to saturate
    //==============================================================================
    
    // Push dry samples into the DryWetMixer
    juce::dsp::AudioBlock<float> dryBlock(buffer);
    dryWetMixer.pushDrySamples(dryBlock);
    
    
    if (!satBypassState) {
        auto oversampledBuffer = oversampler.processSamplesUp(block);
        
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            
            auto* channelData = oversampledBuffer.getChannelPointer(channel);
            saturator.process(channelData, N_oversample, channel);
            
        }
        
        oversampler.processSamplesDown(block);
        
    }
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        
        auto* channelData = buffer.getWritePointer(channel);
        compressor.process(channelData, N, channel);
        
    }
    
    
    dryWetMixer.setWetMixProportion(mixRatio);
    
    // Mix the wet samples with the latency-compensated dry samples
    juce::dsp::AudioBlock<float> wetBlock(buffer);
    dryWetMixer.mixWetSamples(wetBlock);
    
    
    
    //==============================================================================
    //  Compressor Processes
    //==============================================================================
    //
    //
    // // Mono/LR link TBD (broken)
    //
    //    if (mode == MONO) {
    //
    //        if (totalNumInputChannels == 0) {
    //            auto* channelData = buffer.getWritePointer (0);
    //            compressor.process(channelData, N, 0);
    //        }
    //
    //        else {
    //
    //            auto* channelDataL = buffer.getWritePointer(0);
    //            auto* channelDataR = buffer.getWritePointer(1);
    //            compressor.processLrUnlinked(channelDataL, channelDataR, N);
    //
    //        }
    //
    //    }
    //
    //  // MS mode TBD (broken)
    //
    //    else if (mode == MS) {
    //
    //        //tbd
    //
    //    }
    //
    //
    //
    // // LR DELINK MODE (working)
    //
    //    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    //
    //        auto* channelData = buffer.getWritePointer (channel);
    //        compressor.process(channelData, N, channel);
    //
    //    }
    
    
}

//==============================================================================
bool EstrogenAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* EstrogenAudioProcessor::createEditor()
{
    return new EstrogenAudioProcessorEditor (*this);
}

//==============================================================================
void EstrogenAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    auto currentState = apvts.copyState(); // make a duplicate that won't be updated during write to file
    
    std::unique_ptr<juce::XmlElement> xml (currentState.createXml());
    
    copyXmlToBinary(*xml, destData);
    
}

void EstrogenAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary(data, sizeInBytes));
    
    juce::ValueTree newTree = juce::ValueTree::fromXml(*xml);
    
    apvts.replaceState(newTree);
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EstrogenAudioProcessor();
}
