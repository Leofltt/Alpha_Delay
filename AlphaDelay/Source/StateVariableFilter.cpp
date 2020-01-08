/*
  ==============================================================================

    StateVariableFilter.cpp
    Created: 6 Jan 2020 7:02:57pm
    Author:  Leonardo Foletto

  ==============================================================================
*/

#include "StateVariableFilter.h"
#include <math.h>

void StateVariableFilter::updateFilter(float *cf, int sr, float *ft, float *q)
{
    auto cutoff = m_cutoff.load();
    auto filtertype = m_filterType.load();
    auto res = m_Q.load();
    if (*cf != cutoff) m_cutoff.store(*cf);
    if (sr != sampleRate) sampleRate = sr;
    if (*ft != filtertype) m_filterType.store(*ft);
    if (*q != res) m_Q.store(*q);
    
}

void StateVariableFilter::processFilter(AudioBuffer<float>& buffer, int total_num_channels, int processBlockLength)
{
    
    auto cutoff = m_cutoff.load();
    auto reso = m_Q.load();
    auto filterType = m_filterType.load();
    
    for (int channel = 0; channel < total_num_channels; ++channel)
    {
        auto w = buffer.getWritePointer(channel);
        auto r = buffer.getReadPointer(channel);
        auto f1 = 2 * sin( M_PI * cutoff/float(sampleRate));
        auto q1 = reso;
        float output;
        int n = processBlockLength;
        
        while (--n != 0)
        {
            float x = buffer.getSample(channel, *r + n);
            float lpf = d2 + f1 * d1;
            float hpf = x - lpf - q1*d1;
            float bpf = f1 * hpf + d1;
            float notch = hpf + lpf;
            
            d1 = bpf;
            d2 = lpf;

            if (filterType == 0) output = lpf;
            if (filterType == 1) output = bpf;
            if (filterType == 2) output = hpf;
            if (filterType == 3) output = notch;
            
            buffer.setSample(channel, *w + n, output);
            
        }
    }
}
