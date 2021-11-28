/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 2 Jan 2020 5:26:42pm
    Author:  Leonardo Foletto

  ==============================================================================
*/

#include "Delay.h"


void Delay::writeDelayBuffer(AudioBuffer<float>& buffer, int channel,float drywet)
{
    float gain = m_drywet;

    const int b = buffer.getNumSamples();
    const int d = delayBuffer.getNumSamples();
    const float* data = buffer.getReadPointer(channel);

    if (d > b + writePosition)
    {
        delayBuffer.copyFromWithRamp(channel, writePosition, data, b, gain, gain);
    }
    else
    {
        const int remaining_data = d - writePosition;
        delayBuffer.copyFromWithRamp(channel, writePosition, data, remaining_data, gain, gain);
        delayBuffer.copyFromWithRamp(channel, 0, data, b - remaining_data, gain, gain);

    }
};

void Delay::readDelayBuffer(AudioBuffer<float>& buffer, int channel, float dt,float drywet, float spread)
{
    auto delt = m_delayTime;  // in millis
    auto spr = m_spread;
    auto gain = m_drywet;
    auto gainDry = 1 - gain;
    
    
    if (gain != drywet)
    {
        gain = drywet;
        m_drywet = gain;
    }

    if (spread != spr) m_spread = spread;
    
    if (channel % 2 == 1) spread *= -1;

    const int d = delayBuffer.getNumSamples();
    const int b = buffer.getNumSamples();
    auto w = buffer.getWritePointer(channel);
    const int readPosition = static_cast<int>(d + writePosition - (sampleRate * (delt + spr) / 1000)) % d;
    const float* delay_data = delayBuffer.getReadPointer(channel);
    float increment;
    
    if (delt != dt)
    {
        increment = (dt - delt) / b;
    }

    buffer.applyGain(channel, 0, b, gainDry);

    if (d > b + readPosition)
    {
        buffer.addFrom(channel, 0, delay_data + readPosition, b);
        
        for (int n = 0; n != b; n ++)
        {
            const int readPosition = static_cast<int>(d + writePosition - (sampleRate * (delt + spr) / 1000)) % d;
            w[n] +=  delay_data[readPosition + n];
            delt += increment;
        }
    }
    else
    {
        const int remaining_data = d - readPosition;
        for (int n = 0; n != b; n ++)
        {
            const int readPosition = static_cast<int>(d + writePosition - (sampleRate * (delt + spr) / 1000)) % d;
            if (n < remaining_data)
            {
                w[n] +=  delay_data[readPosition + n];
            }
            if ( remaining_data <= n && n < b)
            {
                w[n] +=  delay_data[n];
            }

            delt += increment;
        }
    }
    
    m_delayTime = delt;
};

void Delay::delayFeedback(AudioBuffer<float>& buffer, int channel, float fb, float spread, float dt)
{
    auto gain = m_feedback;
    
    if (fb != gain) m_feedback = fb;

    const int b = buffer.getNumSamples();
    const int d = delayBuffer.getNumSamples();

    const float* delay_data = delayBuffer.getReadPointer(channel);

    auto delt = m_delayTime;  // in millis
    auto spr = m_spread;

    
    if (channel & (1<<0)) spread *= -1;
    
    const int readPosition = static_cast<int>(d + writePosition - (sampleRate * (delt + spr) / 1000)) % d;

    if (d > b + writePosition)
    {
        delayBuffer.addFromWithRamp(channel, writePosition, delay_data + readPosition, b, gain, gain);
    }
    else
    {
        const int remaining_data = d - writePosition;

        delayBuffer.addFromWithRamp(channel, remaining_data, delay_data, remaining_data, gain, gain);
        delayBuffer.addFromWithRamp(channel, 0, delay_data + readPosition, b - remaining_data, gain, gain);
    }
    
    m_delayTime = delt;
};

void Delay::initParameters(float fb, float spread, float delaytime, float drywet, int sr, int samplesPerBlock, int maxDel)
{
    const int delayBSize = sampleRate * maxDel + samplesPerBlock;
    delayBuffer.setSize(2, delayBSize);
    delayBuffer.clear();


    if (fb != m_feedback) m_feedback = fb;

    if (spread != m_spread) m_spread = spread;

    if (delaytime != m_delayTime) m_delayTime = delaytime;

    if (drywet != m_drywet) m_drywet = drywet;

    if (sampleRate != sr) sampleRate = sr;
};

void Delay::processDelay(AudioBuffer<float>& buffer, int total_num_channels, float fb, float spread, float delaytime, float drywet)
{
    const int bufferLength = buffer.getNumSamples();
    const int delayLength = delayBuffer.getNumSamples();

    for (int channel = 0; channel < total_num_channels; ++channel)
    {
        writeDelayBuffer(buffer, channel, drywet);
        readDelayBuffer(buffer, channel, delaytime, drywet, spread);
        delayFeedback(buffer, channel, fb, spread, delaytime);

    }

    writePosition += bufferLength;
    if (writePosition >= delayLength)
    {
        writePosition -= delayLength;
    }};
