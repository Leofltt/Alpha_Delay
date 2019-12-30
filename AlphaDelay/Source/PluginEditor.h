/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "DelayGUI.h"

//==============================================================================
/**
*/
class AlphaDelayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    AlphaDelayAudioProcessorEditor (AlphaDelayAudioProcessor&);
    ~AlphaDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    AlphaDelayAudioProcessor& processor;
    
    DelayGUI delay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlphaDelayAudioProcessorEditor)
};
