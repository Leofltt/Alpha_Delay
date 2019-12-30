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
    setSize(400, 200);
    m_fbSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_fbSlider.setRange(0.0, 0.9);
    m_fbSlider.setTextBoxStyle(Slider::NoTextBox, true, 40, 20);
    m_fbSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&m_fbSlider);
    p_fbValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.m_parameters, FB_ID,m_fbSlider);
    
    m_dryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_dryWetSlider.setRange(0.0, 1.0);
    m_dryWetSlider.setTextBoxStyle(Slider::NoTextBox, true, 40, 20);
    m_dryWetSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&m_dryWetSlider);
    p_drywetValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.m_parameters, DRYWET_ID,m_dryWetSlider);
    
    m_spreadSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_spreadSlider.setRange(0.0, 50.0);
    m_spreadSlider.setTextBoxStyle(Slider::NoTextBox, true, 40, 20);
    m_spreadSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&m_spreadSlider);
    p_spreadValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.m_parameters, SPREAD_ID,m_spreadSlider);
    
    m_delayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_delayTimeSlider.setRange(1.0, 5000.0);
    m_delayTimeSlider.setTextBoxStyle(Slider::NoTextBox, true, 40, 20);
    m_delayTimeSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&m_delayTimeSlider);
    p_delayTimeValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.m_parameters, DELAYTIME_ID,m_delayTimeSlider);

    

}

DelayGUI::~DelayGUI()
{
}

void DelayGUI::paint (Graphics& g)
{

   Rectangle<int> title (0, 10, getWidth(), 20);
    
    g.fillAll(Colours::black);
    g.setColour(Colours::darkcyan);
    g.drawText("ALPHA DELAY",title, Justification::centredTop);
    g.setColour(Colours::lightcyan);
    g.drawText("Delay Time", 20, 150, 85, 20, Justification::centredTop);
    g.drawText("Feedback", 110, 150, 85, 20, Justification::centredTop);
    g.drawText("Spread", 200, 150, 85, 20, Justification::centredTop);
    g.drawText("Dry/Wet", 290, 150, 85, 20, Justification::centredTop);
    
}

void DelayGUI::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(25);
    
    int sliderWidth = 75;
    int sliderHeight = 75;
    int spacer = 10;
    
    m_delayTimeSlider.setBounds(area.removeFromLeft(sliderWidth+spacer).removeFromTop(sliderHeight).withTrimmedTop(spacer));
    m_fbSlider.setBounds(area.removeFromLeft(sliderWidth+spacer).removeFromTop(sliderHeight).withTrimmedTop(spacer));
    m_spreadSlider.setBounds(area.removeFromLeft(sliderWidth+spacer).removeFromTop(sliderHeight).withTrimmedTop(spacer));
    m_dryWetSlider.setBounds(area.removeFromLeft(sliderWidth+spacer).removeFromTop(sliderHeight).withTrimmedTop(spacer));
    

}
