/*
  ==============================================================================

    SimpleFilter.h
    Created: 2 Jan 2020 6:42:26pm
    Author:  Leonardo Foletto

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OnePoleOneZero
{
public:
    
    void processFilter(AudioBuffer<float>& buffer, int total_num_channels, int processBlockLength);
    void updateFilter(float cf, int sr, float ft);
    void setFilter();
    
private:
    
    void setLowPass();
    void setHighPass();
    
    int sampleRate { 44100 };
    
    float a0, a1, b1;
    float x1 {0}, y1 {0};
    
    
    double m_cutoff;
    double m_filterType;
    
};
