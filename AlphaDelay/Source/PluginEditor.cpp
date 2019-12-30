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

    setSize (400, 200);
    addAndMakeVisible(&delay);
}

AlphaDelayAudioProcessorEditor::~AlphaDelayAudioProcessorEditor()
{
}

//==============================================================================
void AlphaDelayAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);

}

void AlphaDelayAudioProcessorEditor::resized()
{
    Rectangle<int> area = getLocalBounds();
    
    const int componentWidth = 400;
    const int componentHeight = 200;
    
    delay.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
}
