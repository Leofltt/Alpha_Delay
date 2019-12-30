/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define FB_ID "feedback"
#define FB_NAME "Feedback"
#define SPREAD_ID "spread"
#define SPREAD_NAME "Spread"
#define DRYWET_ID "drywet"
#define DRYWET_NAME "Dry Wet"
#define DELAYTIME_ID "delayTime"
#define DELAYTIME_NAME "Delay Time"

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

    void writeDelayBuffer(AudioBuffer<float>& buffer, AudioBuffer<float>& delayBuffer, int writePosition, int channel);
    void readDelayBuffer(AudioBuffer<float>& buffer, AudioBuffer<float>& delayBuffer, int writePosition, int channel);
    void delayFeedback(AudioBuffer<float>& buffer, AudioBuffer<float>& delayBuffer, int writePosition, int channel);
    
    void setDelayParameters(float *fb, float *spread, float *delaytime, float *drywet);
    
    float smooth (float start, float end, float factor);
    
    AudioProcessorValueTreeState m_parameters;
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    
private:
    
    AudioBuffer<float> m_DelayBuffer;
    int m_writePosition { 0 };
    int m_sampleRate { 44100 };
    
    std::atomic<double> m_delayTime;
    std::atomic<double> m_feedback;
    std::atomic<double> m_spread;
    std::atomic<double> m_drywet;
    static_assert(std::atomic<double>::is_always_lock_free);
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaDelayAudioProcessor)
};
