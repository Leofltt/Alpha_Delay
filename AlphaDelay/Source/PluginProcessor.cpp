/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AlphaDelayAudioProcessor::AlphaDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
m_parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    m_parameters.state = ValueTree("savedParams");

    
}
AudioProcessorValueTreeState::ParameterLayout AlphaDelayAudioProcessor::createParameterLayout(){
    
    std::vector<std::unique_ptr<RangedAudioParameter>> t_params;
    
//    auto range = NormalisableRange<float> (10.0f, 19000.0f);
//    range.setSkewForCentre(1000);

    auto fbParam = std::make_unique<AudioParameterFloat>(FB_ID, FB_NAME, 0.0f, 1.0f,0.2f);
    auto spreadParam = std::make_unique<AudioParameterFloat>(SPREAD_ID, SPREAD_NAME, 0.0f, 50.0f,0.0f);
    auto dwParam = std::make_unique<AudioParameterFloat>(DRYWET_ID, DRYWET_NAME, 0.0f, 1.0f,0.7f);
    auto dtParam = std::make_unique<AudioParameterFloat>(DELAYTIME_ID, DELAYTIME_NAME, 1.0f, 5000.0f,500.0f);
    
    t_params.push_back(std::move(fbParam));
    t_params.push_back(std::move(spreadParam));
    t_params.push_back(std::move(dwParam));
    t_params.push_back(std::move(dtParam));

    return { t_params.begin(), t_params.end() };
}

AlphaDelayAudioProcessor::~AlphaDelayAudioProcessor()
{
}

//==============================================================================
const String AlphaDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AlphaDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AlphaDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AlphaDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AlphaDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AlphaDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AlphaDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AlphaDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String AlphaDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void AlphaDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AlphaDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const int inputChn = getTotalNumInputChannels();
    const int delayBSize = sampleRate * 5 + samplesPerBlock;
    
    m_DelayBuffer.setSize(inputChn, delayBSize);
    m_DelayBuffer.clear();
    m_sampleRate = sampleRate;
    
    setDelayParameters(m_parameters.getRawParameterValue(FB_ID), m_parameters.getRawParameterValue(SPREAD_ID), m_parameters.getRawParameterValue(DELAYTIME_ID), m_parameters.getRawParameterValue(DRYWET_ID));
}
void AlphaDelayAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AlphaDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AlphaDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    const int bufferLength = buffer.getNumSamples();
    const int delayBuffer = m_DelayBuffer.getNumSamples();
    
    auto bufferCopy = buffer;
    
    
    setDelayParameters(m_parameters.getRawParameterValue(FB_ID), m_parameters.getRawParameterValue(SPREAD_ID), m_parameters.getRawParameterValue(DELAYTIME_ID), m_parameters.getRawParameterValue(DRYWET_ID));
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        
        writeDelayBuffer(bufferCopy, m_DelayBuffer, m_writePosition, channel);
        readDelayBuffer(buffer, m_DelayBuffer, m_writePosition, channel);
        delayFeedback(buffer, m_DelayBuffer, m_writePosition, channel);
        
    }
    
    m_writePosition += bufferLength;
    m_writePosition %= delayBuffer;
}

void AlphaDelayAudioProcessor::writeDelayBuffer(AudioBuffer<float>& buffer, AudioBuffer<float>& delayBuffer, int writePosition, int channel )
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
    
}

void AlphaDelayAudioProcessor::readDelayBuffer(AudioBuffer<float>& buffer, AudioBuffer<float>& delayBuffer, int writePosition, int channel)
{
    auto dt = m_delayTime.load();  // in millis
    auto spread = m_spread.load();
    
    
    if (channel % 2 == 1) spread *= -1;
    
    const int d = delayBuffer.getNumSamples();
    const int b = buffer.getNumSamples();
    const int readPosition = static_cast<int>(d + writePosition - (m_sampleRate * (dt + spread) / 1000)) % d;
    const float* delay_data = delayBuffer.getReadPointer(channel);
    
    auto gainDry = 1 - m_drywet.load();
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
}

void AlphaDelayAudioProcessor::delayFeedback(AudioBuffer<float>& buffer, AudioBuffer<float>& delayBuffer, int writePosition, int channel)
{
    auto feedback = m_feedback.load();
    float gain = feedback;
    
    const int b = buffer.getNumSamples();
    const int d = delayBuffer.getNumSamples();
    
    const float* delay_data = delayBuffer.getReadPointer(channel);
    
    auto dt = m_delayTime.load();  // in millis
    auto spread = m_spread.load();
    const int readPosition = static_cast<int>(d + writePosition - (m_sampleRate * (dt + spread) / 1000)) % d;
    
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
}

void AlphaDelayAudioProcessor::setDelayParameters(float* fb, float* spread, float* delaytime, float* drywet)
{
    auto old_feedback = m_feedback.load();
    auto old_dt = m_delayTime.load();
    auto old_spread = m_spread.load();
    auto old_drywet = m_drywet.load();
    
    if (*fb != old_feedback)
    {
        float smoothedChange = smooth(old_feedback, *fb, 0.6);
        m_feedback.store(smoothedChange);
    }
    if (*spread != old_spread)
    {
        float smoothedChange = smooth(old_spread, *spread, 0.6);
        m_spread.store(smoothedChange);
    }
    if (*delaytime != old_dt)
    {
        float smoothedChange = smooth(old_dt, *delaytime, 0.707);
        float smoothDelayT = smoothedChange - 0.001 * (smoothedChange - *delaytime);
        m_delayTime.store(smoothDelayT);
    }
    if (*drywet != old_drywet)
    {
        float smoothedChange = smooth(old_drywet, *drywet, 0.707);
        m_drywet.store(smoothedChange);
    }
    
}

float AlphaDelayAudioProcessor::smooth (float start, float end, float factor)
{
    float c = start * factor + end * (1 - factor);
    return c;
}

//==============================================================================
bool AlphaDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AlphaDelayAudioProcessor::createEditor()
{
    return new AlphaDelayAudioProcessorEditor (*this);
}

//==============================================================================
void AlphaDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    std::unique_ptr <XmlElement> xml (m_parameters.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AlphaDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr <XmlElement> t_parameters(getXmlFromBinary(data, sizeInBytes));
       
    if (t_parameters != nullptr) {
        if (t_parameters -> hasTagName(m_parameters.state.getType())) m_parameters.state = ValueTree::fromXml(*t_parameters);
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AlphaDelayAudioProcessor();
}