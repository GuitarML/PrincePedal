/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <stdio.h>
#include <fstream>
#include <iostream>


//==============================================================================
PrinceAudioProcessorEditor::PrinceAudioProcessorEditor (PrinceAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to

    // Set Widget Graphics
    //blackHexKnobLAF.setLookAndFeel(ImageCache::getFromMemory(BinaryData::knob2_png, BinaryData::knob2_pngSize));

    gainLAF.setLookAndFeel(ImageCache::getFromMemory(BinaryData::gain_png, BinaryData::gain_pngSize));
    volLAF.setLookAndFeel(ImageCache::getFromMemory(BinaryData::vol_png, BinaryData::vol_pngSize));
    toneLAF.setLookAndFeel(ImageCache::getFromMemory(BinaryData::tone_png, BinaryData::tone_pngSize));

    addAndMakeVisible(odFootSw);
    odFootSw.setImages(true, true, true,
        ImageCache::getFromMemory(BinaryData::sw1_png, BinaryData::sw1_pngSize), 1.0, Colours::transparentWhite,
        Image(), 1.0, Colours::transparentWhite,
        ImageCache::getFromMemory(BinaryData::sw1_png, BinaryData::sw1_pngSize), 1.0, Colours::transparentWhite,
        0.0);
    odFootSw.addListener(this);

    //addAndMakeVisible(odLED);
    //odLED.setImages(true, true, true,
    //    ImageCache::getFromMemory(BinaryData::led_red_on_png, BinaryData::led_red_on_pngSize), 1.0, Colours::transparentWhite,
    //    Image(), 1.0, Colours::transparentWhite,
    //    ImageCache::getFromMemory(BinaryData::led_red_on_png, BinaryData::led_red_on_pngSize), 1.0, Colours::transparentWhite,
    //    0.0);
    //odLED.addListener(this);

    gainSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, GAIN_ID, ampGainKnob);
    addAndMakeVisible(ampGainKnob);
    ampGainKnob.setLookAndFeel(&gainLAF);
    ampGainKnob.addListener(this);
    ampGainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampGainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampGainKnob.setDoubleClickReturnValue(true, 0.5);
	
    toneSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, TONE_ID, ampToneKnob);
    addAndMakeVisible(ampToneKnob);
    ampToneKnob.setLookAndFeel(&toneLAF);
    ampToneKnob.addListener(this);
    ampToneKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampToneKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20 );
    ampToneKnob.setDoubleClickReturnValue(true, 0.5);

    masterSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, MASTER_ID, ampMasterKnob);
    addAndMakeVisible(ampMasterKnob);
    ampMasterKnob.setLookAndFeel(&volLAF);
    ampMasterKnob.addListener(this);
    ampMasterKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20 );
    ampMasterKnob.setDoubleClickReturnValue(true, 0.5);

    addAndMakeVisible(versionLabel);
    versionLabel.setText("v1.0", juce::NotificationType::dontSendNotification);
    versionLabel.setJustificationType(juce::Justification::left);
    versionLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    auto font = versionLabel.getFont();
    float height = font.getHeight();
    font.setHeight(height); // 0.75
    versionLabel.setFont(font);


    // Size of plugin GUI
    setSize(410, 540);
    resetImages();
}

PrinceAudioProcessorEditor::~PrinceAudioProcessorEditor()
{
    ampGainKnob.setLookAndFeel(nullptr);
    ampToneKnob.setLookAndFeel(nullptr);
    ampMasterKnob.setLookAndFeel(nullptr);
}

//==============================================================================
void PrinceAudioProcessorEditor::paint (Graphics& g)
{
    // Workaround for graphics on Windows builds (clipping code doesn't work correctly on Windows)
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    g.drawImageAt(background, 0, 0);  // Debug Line: Redraw entire background image
    #else
    // Redraw only the clipped part of the background image
    juce::Rectangle<int> ClipRect = g.getClipBounds(); 
    g.drawImage(background, ClipRect.getX(), ClipRect.getY(), ClipRect.getWidth(), ClipRect.getHeight(), ClipRect.getX(), ClipRect.getY(), ClipRect.getWidth(), ClipRect.getHeight());
    #endif
   
}

void PrinceAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Amp Widgets
    ampGainKnob.setBounds(220, 54, 136, 136);
    ampMasterKnob.setBounds(90, 54, 118, 118);
    ampToneKnob.setBounds(90, 170, 130, 130);

    //odLED.setBounds(152, 315, 40, 40);
    odFootSw.setBounds(240, 195, 45, 70);
    versionLabel.setBounds(302, 488, 60, 10);
}

void PrinceAudioProcessorEditor::colorSelectClicked() {
    if (processor.current_model_index == 0) {
        processor.current_model_index = 1;
        processor.fromUpDown = 0;
    }
    else if (processor.current_model_index == 1) {
        if (processor.fromUpDown == 0) {
            processor.current_model_index = 2;
        } else {
            processor.current_model_index = 0;
        }
    }
    else if (processor.current_model_index == 2) {
        processor.current_model_index = 1;
        processor.fromUpDown = 1;
    }
    processor.setMode();
    resetImages(); // Resets the Toggle Switch and LED image based on current settings
    repaint();
}

void PrinceAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (processor.current_model_index == 0) {
        processor.current_model_index = 1;
        processor.fromUpDown = 0;
    }
    else if (processor.current_model_index == 1) {
        if (processor.fromUpDown == 0) {
            processor.current_model_index = 2;
        } else {
            processor.current_model_index = 0;
        }
    }
    else if (processor.current_model_index == 2) {
        processor.current_model_index = 1;
        processor.fromUpDown = 1;
    }
    processor.setMode();
    resetImages(); // Resets the Toggle Switch based on current settings
    repaint();
}


void PrinceAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    // Amp
    /*
    if (slider == &ampGainKnob)
        processor.setDrive(slider->getValue());
    else if (slider == &ampMasterKnob)
        processor.setMaster(slider->getValue());
    else if (slider == &ampToneKnob) 
        processor.setTone(slider->getValue());
        */
}


void PrinceAudioProcessorEditor::resetImages()
{
    if (processor.current_model_index == 0) {
        odFootSw.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::sw3_png, BinaryData::sw3_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::sw3_png, BinaryData::sw3_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
    else if (processor.current_model_index == 1) {
        odFootSw.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::sw2_png, BinaryData::sw2_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::sw2_png, BinaryData::sw2_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
    else {
        odFootSw.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::sw1_png, BinaryData::sw1_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::sw1_png, BinaryData::sw1_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
}