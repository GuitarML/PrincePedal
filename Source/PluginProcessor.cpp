/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
#include <fstream>

//==============================================================================
PrinceAudioProcessor::PrinceAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),    
    treeState(*this, nullptr, "PARAMETER", { std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f),
                        std::make_unique<AudioParameterFloat>(TONE_ID, TONE_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f),
                        std::make_unique<AudioParameterFloat>(MASTER_ID, MASTER_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5) })

#endif
{
    setMode();

    driveParam = treeState.getRawParameterValue (GAIN_ID);
    masterParam = treeState.getRawParameterValue (MASTER_ID);
    toneParam = treeState.getRawParameterValue (TONE_ID);
}


PrinceAudioProcessor::~PrinceAudioProcessor()
{
}

//==============================================================================
const String PrinceAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PrinceAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PrinceAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PrinceAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PrinceAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PrinceAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PrinceAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PrinceAudioProcessor::setCurrentProgram (int index)
{
}

const String PrinceAudioProcessor::getProgramName (int index)
{
    return {};
}

void PrinceAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PrinceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    //LSTM.reset();
    //LSTM2.reset();

    // prepare resampler for target sample rate: 48 kHz
    constexpr double targetSampleRate = 44100.0;
    resampler.prepareWithTargetSampleRate ({ sampleRate, (uint32) samplesPerBlock, 2 }, targetSampleRate);

    // load 44.1 kHz sample rate model
    //MemoryInputStream jsonInputStream(BinaryData::od_json, BinaryData::od_jsonSize, false);
    //nlohmann::json weights_json = nlohmann::json::parse(jsonInputStream.readEntireStreamAsString().toStdString());

    //LSTM.load_json3(weights_json);
    //LSTM2.load_json3(weights_json);

    setMode();

    // set up DC blocker
    //dcBlocker.coefficients = dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 35.0f);
    //dsp::ProcessSpec spec{ sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
    //dcBlocker.prepare(spec);
}

void PrinceAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PrinceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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


void PrinceAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    // Setup Audio Data
    const int numSamples = buffer.getNumSamples();

    auto driveValue = static_cast<float> (driveParam->load());
    auto toneValue = static_cast<float> (toneParam->load());
    auto masterValue = static_cast<float> (masterParam->load());

    // Amp =============================================================================
    if (fw_state == 1) {
		//Apply default gain
        //buffer.applyGain(3.0);
		
        // resample to target sample rate
        dsp::AudioBlock<float> block(buffer);
        //auto block = dsp::AudioBlock<float>(buffer.getArrayOfWritePointers(), 1, numSamples);
        auto block44k = resampler.processIn(block);

        // Apply LSTM model
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            // Apply LSTM model
            if (ch == 0) {
                LSTM.process(block44k.getChannelPointer(0), driveValue, toneValue, block44k.getChannelPointer(0), (int) block44k.getNumSamples());
            }
            else if (ch == 1) {
                LSTM2.process(block44k.getChannelPointer(1), driveValue, toneValue, block44k.getChannelPointer(1), (int) block44k.getNumSamples());
            }
        }

        // resample back to original sample rate
        resampler.processOut(block44k, block);

        // Master Volume 
        // Apply ramped changes for gain smoothing
        if (masterValue == previousMasterValue)
        {
            buffer.applyGain(masterValue);
        }
        else {
            buffer.applyGainRamp(0, (int) buffer.getNumSamples(), previousMasterValue, masterValue);
            previousMasterValue = masterValue;
        }
    }
}

//==============================================================================
bool PrinceAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PrinceAudioProcessor::createEditor()
{
    return new PrinceAudioProcessorEditor (*this);
}

//==============================================================================
void PrinceAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    xml->setAttribute ("fw_state", fw_state);
    xml->setAttribute ("current_tone", current_model_index);
    copyXmlToBinary (*xml, destData);
}

void PrinceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (treeState.state.getType()))
        {
            treeState.replaceState (juce::ValueTree::fromXml (*xmlState));
            current_model_index = xmlState->getIntAttribute ("current_tone");
            setMode();
            fw_state = xmlState->getBoolAttribute ("fw_state");
            if (auto* editor = dynamic_cast<PrinceAudioProcessorEditor*> (getActiveEditor()))
                editor->resetImages();
        }
    }
}

void PrinceAudioProcessor::setMode()
{
    if (current_model_index ==0) {
        MemoryInputStream jsonInputStream(BinaryData::od_json, BinaryData::od_jsonSize, false);
        nlohmann::json weights_json = nlohmann::json::parse(jsonInputStream.readEntireStreamAsString().toStdString());
        LSTM.reset();
        LSTM2.reset();
        LSTM.load_json3(weights_json);
        LSTM2.load_json3(weights_json);

    } else if (current_model_index == 1) {
        MemoryInputStream jsonInputStream(BinaryData::boost_json, BinaryData::boost_jsonSize, false);
        nlohmann::json weights_json = nlohmann::json::parse(jsonInputStream.readEntireStreamAsString().toStdString());
        LSTM.reset();
        LSTM2.reset();
        LSTM.load_json3(weights_json);
        LSTM2.load_json3(weights_json);

    } else if (current_model_index == 2) {
        MemoryInputStream jsonInputStream(BinaryData::dist_json, BinaryData::dist_jsonSize, false);
        nlohmann::json weights_json = nlohmann::json::parse(jsonInputStream.readEntireStreamAsString().toStdString());
        LSTM.reset();
        LSTM2.reset();
        LSTM.load_json3(weights_json);
        LSTM2.load_json3(weights_json);

    }
  
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PrinceAudioProcessor();
}
