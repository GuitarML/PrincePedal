/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <nlohmann/json.hpp>
#include "RTNeuralLSTM.h"

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define GAIN_ID "drive"
#define GAIN_NAME "Drive"
#define MASTER_ID "level"
#define MASTER_NAME "Level"
#define TONE_ID "tone"
#define TONE_NAME "Tone"


//==============================================================================
class PrinceAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    PrinceAudioProcessor();
    ~PrinceAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setMode();

    // Pedal/amp states
    int fw_state = 1; // 0 = off, 1 = on

    bool lstm_state = true;
    int current_model_index = 0;
    int fromUpDown = 0;

    RT_LSTM LSTM;
    RT_LSTM LSTM2;

    AudioProcessorValueTreeState treeState;

private:

    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* toneParam = nullptr;
    std::atomic<float>* masterParam = nullptr;

    float previousMasterValue = 0.5;

    chowdsp::ResampledProcess<chowdsp::ResamplingTypes::SRCResampler<>> resampler;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PrinceAudioProcessor)
};
