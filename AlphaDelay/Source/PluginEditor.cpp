/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AlphaDelayAudioProcessorEditor::AlphaDelayAudioProcessorEditor (AlphaDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), delay(p)
{

    setSize (555, 250);
    addAndMakeVisible(&delay);
}

AlphaDelayAudioProcessorEditor::~AlphaDelayAudioProcessorEditor()
{
}

//==============================================================================
void AlphaDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::dimgrey);

}

void AlphaDelayAudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds();
    
    const int componentWidth = 555;
    const int componentHeight = 250;
    
    delay.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
}
