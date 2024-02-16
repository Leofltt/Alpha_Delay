/*
  ==============================================================================

    StateVariableFilter.cpp
    Created: 6 Jan 2020 7:02:57pm
    Author:  Leonardo Foletto

  ==============================================================================
*/

#include "StateVariableFilter.h"
#include <math.h>

#define TWOPI M_PI*2

void StateVariableFilter::updateFilter(float cf, int sr, int ft, float q)
{
    if (cf != m_cutoff) m_cutoff = cf;
    if (sr != sampleRate) sampleRate = sr;
    if (ft != m_filterType) m_filterType = ft;
    if (q != m_Q) m_Q = q;
    calculateCoeff();
    
}

void StateVariableFilter::calculateCoeff()
{
    float wd = TWOPI * m_cutoff;
    float T = 1.0 / sampleRate;
    float wa = (2.0 / T)*tan(wd*T / 2.0);
    float g = wa * T / 2.0;
    float R = 1.0 / (2.0 * m_Q);
    
    alpha0 = 1.0 / (1.0 + 2.0 * R * g + g * g);
    alpha = g;
    rho = 2.0 * R + g;
    
    float f_o = (sampleRate / 2.0) / m_cutoff;
    analogMatchSigma = 1.0 / (alpha * f_o * f_o);
}

void StateVariableFilter::processFilter(juce::AudioBuffer<float>& buffer, int total_num_channels, int processBlockLength)
{
    auto filterType = m_filterType;
    
    for (int channel = 0; channel < total_num_channels; ++channel)
    {
        float lpf, hpf, bpf, notch;
        auto w = buffer.getWritePointer(channel);
        auto r = buffer.getReadPointer(channel);
        float output;
        
        
        for (int n = 0; n != processBlockLength; ++n)
        {
            float x = r[n];
            hpf = alpha0 * (x - rho * integrator_z[0] - integrator_z[1]);
            bpf = alpha * hpf + integrator_z[0];
            lpf = alpha * bpf + integrator_z[1];
            notch = hpf + lpf;
            
            float sn = integrator_z[0];
            
            integrator_z[0] = alpha * hpf + bpf;
            integrator_z[1] = alpha * bpf + lpf;
            
            lpf  += analogMatchSigma * sn;
            
            switch (filterType)
            {
                case 0:
                    output = lpf;
                    break;
                case 1:
                    output = bpf;
                    break;
                case 2:
                    output = hpf;
                    break;
                case 3:
                    output = notch;
                    break;
                default:
                    output = lpf;
                    break;
            }
            
            w[n] = output;
            
        }
    }
}
