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
    
    void processFilter(AudioBuffer<float>& buffer, int total_num_channels);
    void setLowPass();
    void setHighPass();
    void updateFilter(float *cf, int sr);
    
private:
    
    int sampleRate { 44100 };
    
    float a0, a1, b1;
    float x1 {0}, y1 {0};
    
    std::atomic<double> m_cutoff;
    static_assert(std::atomic<double>::is_always_lock_free);
    
};
