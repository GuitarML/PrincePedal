/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "myLookAndFeel.h"
#include <stdlib.h>

//==============================================================================
/**
*/
class PrinceAudioProcessorEditor  : public AudioProcessorEditor,
                                      private Button::Listener,
                                      private Slider::Listener
{
public:
    PrinceAudioProcessorEditor (PrinceAudioProcessor&);
    ~PrinceAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void resetImages();
    void colorSelectClicked();

    myLookAndFeel blackHexKnobLAF;
    myLookAndFeel gainLAF;
    myLookAndFeel volLAF;
    myLookAndFeel toneLAF;
    ImageButton odFootSw;
    ImageButton odLED;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PrinceAudioProcessor& processor;

    Image background = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    // Amp Widgets
    Slider ampGainKnob;
    Slider ampMasterKnob;
    Slider ampToneKnob;
    Label versionLabel;
    
    virtual void buttonClicked(Button* button) override;

    //void updateToggleState(juce::Button* button, juce::String name);
    virtual void sliderValueChanged(Slider* slider) override;

    //AudioProcessorParameter* getParameter(const String& paramId);

    //float getParameterValue(const String& paramId);
    //void setParameterValue(const String& paramId, float value);


public:
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> gainSliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> toneSliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> masterSliderAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PrinceAudioProcessorEditor)
};
