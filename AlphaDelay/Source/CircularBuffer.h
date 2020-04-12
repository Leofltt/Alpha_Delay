/*
  ==============================================================================

    CircularBuffer.h
    Created: 2 Jan 2020 1:17:52pm
    Author:  Leonardo Foletto

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class CircularBuffer {
public:
    void processDelay(AudioBuffer<float>& buffer, int total_num_channels, float fb, float spread, float delaytime, float drywet);
    void initParameters(float fb, float spread, float delaytime, float drywet, int sr, int samplesperblock, int maxdel);
    
private:
    AudioBuffer<float> delayBuffer;
    
    int writePosition { 0 };
    int sampleRate { 44100 };
    int maxDel { 5 };
    
    void writeDelayBuffer(AudioBuffer<float>& buffer, int channel,float drywet);
    void readDelayBuffer(AudioBuffer<float>& buffer, int channel, float dt,float drywet,float spread);
    void delayFeedback(AudioBuffer<float>& buffer, int channel, float fb, float spread, float dt);
    
    double m_delayTime;
    double m_feedback;
    double m_spread;
    double m_drywet;
    
};



