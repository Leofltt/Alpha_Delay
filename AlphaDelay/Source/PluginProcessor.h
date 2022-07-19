/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
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
class AlphaDelayAudioProcessor  : public AudioProcessor
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

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    AudioProcessorValueTreeState m_parameters;
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    Delay c;
    StateVariableFilter filter;
    
    LinearSmoothedValue<float> smoothDelayTime { 500.0f };
    LinearSmoothedValue<float> smoothDelayFeedback { 0.0f };
    LinearSmoothedValue<float> smoothCF { 20000.0f };
    LinearSmoothedValue<float> smoothQ { 0.5f };
    LinearSmoothedValue<float> smoothDelayDryWet { 0.0f };
    
private:
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaDelayAudioProcessor)
};
