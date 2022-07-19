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
    
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    auto range = NormalisableRange<float> (0.0f, 20000.0f);
    range.setSkewForCentre(1000);
    
    auto range2 = NormalisableRange<float> (0.0f, 2.0f);
    range2.setSkewForCentre(0.5);
    
    auto cfParam = std::make_unique<AudioParameterFloat>(CF_ID, CF_NAME, range, 1000.0f);
    auto resParam = std::make_unique<AudioParameterFloat>(RES_ID, RES_NAME, range2, 0.7f);
    
    auto fbParam = std::make_unique<AudioParameterFloat>(FB_ID, FB_NAME, 0.0f, 1.0f, 0.2f);
    auto spreadParam = std::make_unique<AudioParameterFloat>(SPREAD_ID, SPREAD_NAME, 0.0f, 50.0f, 0.0f);
    auto dwParam = std::make_unique<AudioParameterFloat>(DRYWET_ID, DRYWET_NAME, 0.0f, 1.0f, 0.7f);
    auto dtParam = std::make_unique<AudioParameterFloat>(DELAYTIME_ID, DELAYTIME_NAME, 1.0f, 5000.0f, 500.0f);
    
    auto filterTypeParam = std::make_unique<AudioParameterInt>(FT_ID, FT_NAME, 0, 3, 0);
    
    layout.add(std::move(cfParam));
    layout.add(std::move(resParam));
    layout.add(std::move(fbParam));
    layout.add(std::move(spreadParam));
    layout.add(std::move(dwParam));
    layout.add(std::move(dtParam));
    layout.add(std::move(filterTypeParam));
    
    return layout;
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
    c.initParameters(m_parameters.getRawParameterValue(FB_ID)->load(), m_parameters.getRawParameterValue(SPREAD_ID)->load(), m_parameters.getRawParameterValue(DELAYTIME_ID)->load(), m_parameters.getRawParameterValue(DRYWET_ID)->load(), sampleRate, samplesPerBlock, 5);
    filter.updateFilter(m_parameters.getRawParameterValue(CF_ID)->load(), sampleRate,m_parameters.getRawParameterValue(FT_ID)->load(),m_parameters.getRawParameterValue(RES_ID)->load());
    filter.calculateCoeff();
    
    smoothCF.reset(sampleRate, 0.0005);
    smoothQ.reset(sampleRate, 0.0005);
    smoothDelayTime.reset(sampleRate, 0.0005);
    smoothDelayFeedback.reset(sampleRate, 0.0005);
    smoothDelayDryWet.reset(sampleRate, 0.0005);
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
    
    auto cutoff = m_parameters.getRawParameterValue(CF_ID)->load();
    auto Q = m_parameters.getRawParameterValue(RES_ID)->load();
    
    smoothCF.setTargetValue(cutoff);
    smoothQ.setTargetValue(Q);
    
    filter.updateFilter(smoothCF.getNextValue(), getSampleRate(),m_parameters.getRawParameterValue(FT_ID)->load(),smoothQ.getNextValue());
    
    auto delayTime = m_parameters.getRawParameterValue(DELAYTIME_ID)->load();
    auto delayFeedback = m_parameters.getRawParameterValue(FB_ID)->load();
    auto delayDryWet = m_parameters.getRawParameterValue(DRYWET_ID)->load();
    
    smoothDelayTime.setTargetValue(delayTime);
    smoothDelayFeedback.setTargetValue(delayFeedback);
    smoothDelayDryWet.setTargetValue(delayDryWet);
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    c.processDelay(buffer, totalNumInputChannels, smoothDelayFeedback.getNextValue(), m_parameters.getRawParameterValue(SPREAD_ID)->load(),
        smoothDelayTime.getNextValue(),
        smoothDelayDryWet.getNextValue());
    filter.processFilter(buffer, totalNumInputChannels, buffer.getNumSamples());

}

//==============================================================================
bool AlphaDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AlphaDelayAudioProcessor::createEditor()
{
    return new AlphaDelayAudioProcessorEditor (*this);
//    return new juce::GenericAudioProcessorEditor(*this);
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
