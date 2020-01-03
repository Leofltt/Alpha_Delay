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
    
    auto range = NormalisableRange<float> (0.0f, 20000.0f);
    range.setSkewForCentre(1000);
    
    auto cfParam = std::make_unique<AudioParameterFloat>(CF_ID, CF_NAME, range, 800.0f);
    
    auto fbParam = std::make_unique<AudioParameterFloat>(FB_ID, FB_NAME, 0.0f, 1.0f,0.2f);
    auto spreadParam = std::make_unique<AudioParameterFloat>(SPREAD_ID, SPREAD_NAME, 0.0f, 50.0f,0.0f);
    auto dwParam = std::make_unique<AudioParameterFloat>(DRYWET_ID, DRYWET_NAME, 0.0f, 1.0f,0.7f);
    auto dtParam = std::make_unique<AudioParameterFloat>(DELAYTIME_ID, DELAYTIME_NAME, 1.0f, 5000.0f,500.0f);
    
    auto filterTypeParam = std::make_unique<AudioParameterFloat>(FT_ID, FT_NAME, 0, 1, 0);
    
    t_params.push_back(std::move(fbParam));
    t_params.push_back(std::move(spreadParam));
    t_params.push_back(std::move(dwParam));
    t_params.push_back(std::move(dtParam));
    t_params.push_back(std::move(cfParam));
    t_params.push_back(std::move(filterTypeParam));

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
    c.initParameters(m_parameters.getRawParameterValue(FB_ID), m_parameters.getRawParameterValue(SPREAD_ID), m_parameters.getRawParameterValue(DELAYTIME_ID), m_parameters.getRawParameterValue(DRYWET_ID), sampleRate, samplesPerBlock, 5);
    filter.updateFilter(m_parameters.getRawParameterValue(CF_ID), sampleRate, m_parameters.getRawParameterValue(FT_ID));
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
    
    filter.updateFilter(m_parameters.getRawParameterValue(CF_ID), getSampleRate(), m_parameters.getRawParameterValue(FT_ID));
    filter.setFilter();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    c.processDelay(buffer, totalNumInputChannels, m_parameters.getRawParameterValue(FB_ID), m_parameters.getRawParameterValue(SPREAD_ID), m_parameters.getRawParameterValue(DELAYTIME_ID), m_parameters.getRawParameterValue(DRYWET_ID));
    filter.processFilter(c.delayBuffer, totalNumInputChannels, buffer.getNumSamples());

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
