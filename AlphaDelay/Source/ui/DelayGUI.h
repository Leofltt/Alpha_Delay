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
class DelayGUI    : public juce::Component
{
public:
    DelayGUI(AlphaDelayAudioProcessor&);
    ~DelayGUI();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AlphaDelayAudioProcessor& processor;
    
    juce::Slider m_fbSlider;
    juce::Slider m_delayTimeSlider;
    juce::Slider m_dryWetSlider;
    juce::Slider m_spreadSlider;
    juce::Slider m_cutoffSlider;
    juce::Slider m_resoSlider;
    
    juce::ComboBox m_filterType;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> p_fbValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> p_delayTimeValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> p_drywetValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> p_spreadValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> p_cutoffValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> p_resoValue;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> p_filterType;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayGUI)
};
