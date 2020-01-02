/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 2 Jan 2020 5:26:42pm
    Author:  Leonardo Foletto

  ==============================================================================
*/

#include "CircularBuffer.h"


void CircularBuffer::writeDelayBuffer(AudioBuffer<float>& buffer, int channel,float* drywet)
{
    float gain = m_drywet.load();

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

void CircularBuffer::readDelayBuffer(AudioBuffer<float>& buffer, int channel, float* dt,float* drywet, float* spread)
{
    auto delt = m_delayTime.load();  // in millis
    auto spr = m_spread.load();
    auto gain = m_drywet.load();
    auto gainDry = 1 - gain;
    
    if (gain != *drywet)
    {
        gain = *drywet;
        m_drywet.store(gain);
    }

    if (*spread != spr) m_spread.store(*spread);
    if (*dt != delt) m_delayTime.store(*dt);
    
    if (channel % 2 == 1) *spread *= -1;

    const int d = delayBuffer.getNumSamples();
    const int b = buffer.getNumSamples();
    const int readPosition = static_cast<int>(d + writePosition - (sampleRate * (delt + spr) / 1000)) % d;
    const float* delay_data = delayBuffer.getReadPointer(channel);

    buffer.applyGain(channel, 0, b, gainDry);

    if (d > b + readPosition)
    {
        buffer.addFrom(channel, 0, delay_data + readPosition, b);
    }
    else
    {
        const int remaining_data = d - readPosition;
        buffer.addFrom(channel, 0, delay_data + readPosition, remaining_data);
        buffer.addFrom(channel, remaining_data, delay_data, b - remaining_data);
    }
};

void CircularBuffer::delayFeedback(AudioBuffer<float>& buffer, int channel, float* fb, float *spread, float *dt)
{
    auto gain = m_feedback.load();
    
    if (*fb != gain) m_feedback.store(*fb);

    const int b = buffer.getNumSamples();
    const int d = delayBuffer.getNumSamples();

    const float* delay_data = delayBuffer.getReadPointer(channel);

    auto delt = m_delayTime.load();  // in millis
    auto spr = m_spread.load();
    if (*spread != spr) m_spread.store(*spread);
    if (*dt != delt) m_delayTime.store(*dt);
    
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
};

void CircularBuffer::initParameters(float *fb, float *spread, float *delaytime, float *drywet, int sr, int samplesPerBlock, int maxDel)
{
    const int delayBSize = sampleRate * maxDel + samplesPerBlock;
    delayBuffer.setSize(2, delayBSize);
    delayBuffer.clear();

    auto old_feedback = m_feedback.load();
    auto old_dt = m_delayTime.load();
    auto old_spread = m_spread.load();
    auto old_drywet = m_drywet.load();

    if (*fb != old_feedback) m_feedback.store(*fb);

    if (*spread != old_spread) m_spread.store(*spread);

    if (*delaytime != old_dt) m_delayTime.store(*delaytime);

    if (*drywet != old_drywet) m_drywet.store(*drywet);

    if (sampleRate != sr) sampleRate = sr;
};

void CircularBuffer::processDelay(AudioBuffer<float>& buffer, int total_num_channels, float *fb, float *spread, float *delaytime, float *drywet)
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
    writePosition %= delayLength;
};
