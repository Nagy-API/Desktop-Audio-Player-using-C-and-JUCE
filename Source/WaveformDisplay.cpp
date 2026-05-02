#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay() :
    formatManager(),
    thumbnailCache(5),
    thumbnail(512, formatManager, thumbnailCache),
    position(0.0),
    fileLoaded(false)
{
    formatManager.registerBasicFormats();
    thumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
    thumbnail.removeChangeListener(this);
}

void WaveformDisplay::paint(juce::Graphics& g)
{

    g.fillAll(juce::Colours::black);


    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    if (fileLoaded && thumbnail.getTotalLength() > 0)
    {

        g.setColour(juce::Colours::cyan);
        thumbnail.drawChannels(g,
            getLocalBounds(),
            0,
            thumbnail.getTotalLength(),
            1.0f);

        g.setColour(juce::Colours::red);
        int xPosition = position * getWidth();
        g.drawLine(xPosition, 0, xPosition, getHeight(), 2.0f);

         
        g.setColour(juce::Colours::yellow);
        g.fillEllipse(xPosition - 3, getHeight() - 6, 6, 6);
    }
    else
    {
        
        g.setColour(juce::Colours::white);
        g.setFont(16.0f);
        g.drawText("Load audio file to display waveform",
            getLocalBounds(),
            juce::Justification::centred,
            true);
    }
}

void WaveformDisplay::resized()
{
    
}

void WaveformDisplay::loadFile(const juce::File& audioFile)
{
    if (audioFile.existsAsFile())
    {
        thumbnail.clear();
        fileLoaded = thumbnail.setSource(new juce::FileInputSource(audioFile));
        repaint();
    }
}

void WaveformDisplay::setPosition(double relativePosition)
{
    if (position != relativePosition)
    {
        position = relativePosition;
        repaint();
    }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
    {
         
        repaint();
    }
}
