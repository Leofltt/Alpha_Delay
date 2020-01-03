/*
  ==============================================================================

    SimpleFilter.cpp
    Created: 2 Jan 2020 6:42:26pm
    Author:  Leonardo Foletto

  ==============================================================================
*/

#include "SimpleFilter.h"

void OnePoleOneZero::updateFilter(float *cf, int sr)
{
    auto cutoff = m_cutoff.load();
    if (*cf != cutoff) m_cutoff.store(*cf);
    if (sr != sampleRate) sampleRate = sr;
    
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

void OnePoleOneZero::processFilter(AudioBuffer<float>& buffer, int total_num_channels)
{
    const int bufferLength = buffer.getNumSamples();
    
    for (int channel = 0; channel < total_num_channels; ++channel)
    {
        auto w = buffer.getWritePointer(channel);
        auto r = buffer.getReadPointer(channel);
        
        for (int n = 1; n < bufferLength; n++)
        {
            float x = *r;
            float y = x * a0 + x1 * a1 + y1 * b1;
            x1 = x;
            y1 = y;
            *w = y;
        }
    }
}
