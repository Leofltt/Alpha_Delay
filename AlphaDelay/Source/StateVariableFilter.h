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
        
private:
       
        int sampleRate { 44100 };
        float d1 {0}, d2 {0};
          
          
        double m_cutoff;
        double m_filterType;
        double m_Q;
    
};
