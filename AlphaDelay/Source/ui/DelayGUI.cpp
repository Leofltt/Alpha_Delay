/*
  ==============================================================================

    DelayGUI.cpp
    Created: 26 Dec 2019 1:30:25pm
    Author:  Leonardo Foletto

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayGUI.h"

//==============================================================================
DelayGUI::DelayGUI(AlphaDelayAudioProcessor& p) : processor(p)
{
    setSize(555, 250);
    m_fbSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_fbSlider.setRange(0.0, 0.9);
    m_fbSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 40, 20);
    m_fbSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&m_fbSlider);
    p_fbValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.m_parameters, FB_ID,m_fbSlider);
    
    m_dryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_dryWetSlider.setRange(0.0, 1.0);
    m_dryWetSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 40, 20);
    m_dryWetSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&m_dryWetSlider);
    p_drywetValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.m_parameters, DRYWET_ID,m_dryWetSlider);
    
    m_spreadSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_spreadSlider.setRange(0.0, 50.0);
    m_spreadSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 40, 20);
    m_spreadSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&m_spreadSlider);
    p_spreadValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.m_parameters, SPREAD_ID,m_spreadSlider);
    
    m_delayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_delayTimeSlider.setRange(1.0, 5000.0);
    m_delayTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 40, 20);
    m_delayTimeSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&m_delayTimeSlider);
    p_delayTimeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.m_parameters, DELAYTIME_ID,m_delayTimeSlider);
    
    m_cutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_cutoffSlider.setRange(1.0, 5000.0);
    m_cutoffSlider.setSkewFactorFromMidPoint(1000.0);
    m_cutoffSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 40, 20);
    m_cutoffSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&m_cutoffSlider);
    p_cutoffValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.m_parameters, CF_ID,m_cutoffSlider);
    
    m_filterType.setJustificationType(juce::Justification::centred);
    m_filterType.addItem("Low Pass Filter", 1);
    m_filterType.addItem("Band Pass Filter", 2);
    m_filterType.addItem("High Pass Filter", 3);
    m_filterType.addItem("Notch Filter", 4);
    addAndMakeVisible(&m_filterType);
    p_filterType = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.m_parameters, FT_ID,m_filterType);
    
    m_resoSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_resoSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 40, 20);
    m_resoSlider.setRange(0, 2.0);
    m_resoSlider.setSkewFactorFromMidPoint(0.5);
    m_resoSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&m_resoSlider);
    p_resoValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.m_parameters, RES_ID,m_resoSlider);

    

}

DelayGUI::~DelayGUI()
{
}

void DelayGUI::paint (juce::Graphics& g)
{

   juce::Rectangle<int> title (0, 10, getWidth(), 20);
    
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::cyan);
    g.drawText("ALPHA DELAY",title, juce::Justification::centredTop);
    g.setColour(juce::Colours::lightcyan);
    g.drawText("Delay Time", 20, 150, 85, 20, juce::Justification::centredTop);
    g.drawText("Feedback", 110, 150, 85, 20, juce::Justification::centredTop);
    g.drawText("Spread", 195, 150, 85, 20, juce::Justification::centredTop);
    g.drawText("Dry/Wet", 285, 150, 85, 20, juce::Justification::centredTop);
    g.drawText("Filter Cutoff", 370, 150, 85, 20, juce::Justification::centredTop);
    g.drawText("Filter Q", 455, 150, 85, 20, juce::Justification::centredTop);
    
}

void DelayGUI::resized()
{
    juce::Rectangle<int> area = getLocalBounds().reduced(25);
    
    int sliderWidth = 75;
    int sliderHeight = 75;
    int spacer = 10;
    
    m_delayTimeSlider.setBounds(area.removeFromLeft(sliderWidth+spacer).removeFromTop(sliderHeight).withTrimmedTop(spacer));
    m_fbSlider.setBounds(area.removeFromLeft(sliderWidth+spacer).removeFromTop(sliderHeight).withTrimmedTop(spacer));
    m_spreadSlider.setBounds(area.removeFromLeft(sliderWidth+spacer).removeFromTop(sliderHeight).withTrimmedTop(spacer));
    m_dryWetSlider.setBounds(area.removeFromLeft(sliderWidth+spacer).removeFromTop(sliderHeight).withTrimmedTop(spacer));
    m_cutoffSlider.setBounds(area.removeFromLeft(sliderWidth+spacer).removeFromTop(sliderHeight).withTrimmedTop(spacer));
    m_resoSlider.setBounds(area.removeFromLeft(sliderWidth+spacer).removeFromTop(sliderHeight).withTrimmedTop(spacer));
    
    juce::Rectangle<int> area1 = getLocalBounds().reduced(25);
    
    m_filterType.setBounds(area1.removeFromBottom(20));

}
