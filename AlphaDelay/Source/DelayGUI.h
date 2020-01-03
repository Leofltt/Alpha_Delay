/*
  ==============================================================================

    DelayGUI.h
    Created: 26 Dec 2019 1:30:25pm
    Author:  Leonardo Foletto

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class DelayGUI    : public Component
{
public:
    DelayGUI(AlphaDelayAudioProcessor&);
    ~DelayGUI();

    void paint (Graphics&) override;
    void resized() override;

private:
    AlphaDelayAudioProcessor& processor;
    
    Slider m_fbSlider;
    Slider m_delayTimeSlider;
    Slider m_dryWetSlider;
    Slider m_spreadSlider;
    Slider m_cutoffSlider;
    
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> p_fbValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> p_delayTimeValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> p_drywetValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> p_spreadValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> p_cutoffValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayGUI)
};
