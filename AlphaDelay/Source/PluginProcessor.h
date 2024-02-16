/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "Delay.h"
//#include "SimpleFilter.h"
#include "StateVariableFilter.h"

#define FB_ID "feedback"
#define FB_NAME "Feedback"
#define SPREAD_ID "spread"
#define SPREAD_NAME "Spread"
#define DRYWET_ID "drywet"
#define DRYWET_NAME "Dry Wet"
#define DELAYTIME_ID "delayTime"
#define DELAYTIME_NAME "Delay Time"
#define CF_ID "cutoff"
#define CF_NAME "Cutoff Frequency"
#define FT_ID "filterType"
#define FT_NAME "Filter Type"
#define RES_ID "q"
#define RES_NAME "Q"

//==============================================================================
/**
*/
class AlphaDelayAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AlphaDelayAudioProcessor();
    ~AlphaDelayAudioProcessor();

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
    
    juce::AudioProcessorValueTreeState m_parameters;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    Delay c;
    StateVariableFilter filter;
    
    juce::LinearSmoothedValue<float> smoothDelayTime { 500.0f };
    juce::LinearSmoothedValue<float> smoothDelayFeedback { 0.0f };
    juce::LinearSmoothedValue<float> smoothCF { 20000.0f };
    juce::LinearSmoothedValue<float> smoothQ { 0.5f };
    juce::LinearSmoothedValue<float> smoothDelayDryWet { 0.0f };
    
private:
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaDelayAudioProcessor)
};
