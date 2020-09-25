/*
  ==============================================================================

    StateVariableFilter.h
    Created: 6 Jan 2020 7:02:57pm
    Author:  Leonardo Foletto

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class StateVariableFilter
{
public:
        
        void processFilter(AudioBuffer<float>& buffer, int total_num_channels, int processBlockLength);
        void updateFilter(float cf, int sr, float ft, float q);
        void calculateCoeff();
        
private:
       
    int sampleRate { 44100 };
    float integrator_z[2] {0, 0};
          
          
    float m_cutoff;
    float m_filterType;
    float m_Q;
    
    float alpha0, alpha, rho, analogMatchSigma;
    
};
