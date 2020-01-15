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
    const auto kDenorm = 1.0e-24;
    float hist;
    
    for (int channel = 0; channel < total_num_channels; ++channel)
    {
        float lpf, hpf, bpf, notch;
        auto w = buffer.getWritePointer(channel);
        auto r = buffer.getReadPointer(channel);
        auto f1 = 2 * sin( M_PI * cutoff/float(sampleRate * 2));
        auto q1 = sqrt(1.0 - atan(sqrt(reso)) * 2.0 / M_PI);;
        auto scale = sqrt(q1);
        float output;
        
        for (int n = 0; n != processBlockLength; ++n)
        {
            float x = r[n];
            
            for (int i = 0; i < 2; ++i)
            {
                if (i == 0) hist = x;
                if (i == 1) x = (x + hist) / 2;
                
                lpf = d2 + f1 * d1 - kDenorm;
                hpf = scale * x - lpf - q1*d1;
                bpf = f1 * hpf + d1;
                notch = hpf + lpf;

            }
            
            d1 = bpf;
            d2 = lpf + kDenorm;

            if (filterType == 0) output = lpf;
            if (filterType == 1) output = bpf;
            if (filterType == 2) output = hpf;
            if (filterType == 3) output = notch;
            
            w[n] = output;
            
        }
    }
}
