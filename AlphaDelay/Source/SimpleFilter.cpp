/*
  ==============================================================================

    SimpleFilter.cpp
    Created: 2 Jan 2020 6:42:26pm
    Author:  Leonardo Foletto

  ==============================================================================
*/

#include "SimpleFilter.h"

void OnePoleOneZero::updateFilter(float *cf, int sr, float *ft)
{
    auto cutoff = m_cutoff.load();
    auto filtertype = m_filterType.load();
    if (*cf != cutoff) m_cutoff.store(*cf);
    if (sr != sampleRate) sampleRate = sr;
    if (*ft != filtertype) m_filterType.store(*ft);
    
}

void OnePoleOneZero::setLowPass()
{
    auto cutoff = m_cutoff.load();
    auto sr = float(sampleRate);
    a0 = cutoff / (sr + cutoff);
    a1 = a0;
    b1 = (sr - cutoff)/(sr + cutoff);
}

void OnePoleOneZero::setHighPass()
{
    auto cutoff = m_cutoff.load();
    auto sr = float(sampleRate);
    a0 = sr / (sr + cutoff);
    a1 = -a0;
    b1 = (sr - cutoff)/(sr + cutoff);
}

void OnePoleOneZero::setFilter()
{
    auto ft = m_filterType.load();
    
    if (ft == 0) setLowPass();
    if (ft == 1) setHighPass();
}

void OnePoleOneZero::processFilter(AudioBuffer<float>& buffer, int total_num_channels, int processBlockLength)
{
    
    for (int channel = 0; channel < total_num_channels; ++channel)
    {
        auto w = buffer.getWritePointer(channel);
        auto r = buffer.getReadPointer(channel);
        
        
        for (int n = 0; n < processBlockLength; n++)
        {
            float x = buffer.getSample(channel, *r + n);
            float y = x * a0 + x1 * a1 + y1 * b1;
            x1 = x;
            y1 = y;
            buffer.setSample(channel, *w + n, y);
        }
    }
}
