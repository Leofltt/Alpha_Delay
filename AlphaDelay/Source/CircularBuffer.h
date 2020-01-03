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
    void processDelay(AudioBuffer<float>& buffer, int total_num_channels, float *fb, float *spread, float *delaytime, float *drywet);
    void initParameters(float *fb, float *spread, float *delaytime, float *drywet, int sr, int samplesperblock, int maxdel);
    
    AudioBuffer<float> delayBuffer; // originally this was private, but I wanted to be able to process the delay buffer externally so I made it public
    
private:
    
    int writePosition { 0 };
    int sampleRate { 44100 };
    int maxDel { 5 };
    
    void writeDelayBuffer(AudioBuffer<float>& buffer, int channel,float* drywet);
    void readDelayBuffer(AudioBuffer<float>& buffer, int channel, float* dt,float* drywet,float* spread);
    void delayFeedback(AudioBuffer<float>& buffer, int channel, float* fb, float* spread, float* dt);
    
    std::atomic<double> m_delayTime;
    std::atomic<double> m_feedback;
    std::atomic<double> m_spread;
    std::atomic<double> m_drywet;
    static_assert(std::atomic<double>::is_always_lock_free);
    
};



