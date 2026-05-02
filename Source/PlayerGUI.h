#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"


using namespace juce;
using namespace std;

class MarkerListModel : public juce::ListBoxModel
{
public:
    std::vector<std::pair<int, double>>* markers = nullptr;
    std::function<void(int)> onMarkerClicked;

    int getNumRows() override
    {
        return markers ? (int)markers->size() : 0;
    }

    void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool rowIsSelected) override
    {
        if (!markers || row >= (int)markers->size()) return;

        if (rowIsSelected) g.fillAll(juce::Colours::darkgrey);
        else g.fillAll(juce::Colours::black);

        g.setColour(juce::Colours::white);
        auto [num, time] = (*markers)[row];
        int minutes = (int)(time) / 60;
        int seconds = (int)(time) % 60;
        juce::String text = "Marker " + juce::String(num)
            + " (" + juce::String::formatted("%02d:%02d", minutes, seconds) + ")";
        g.drawText(text, 5, 0, width - 10, height, juce::Justification::centredLeft);
    }

    void listBoxItemClicked(int row, const juce::MouseEvent&) override
    {
        if (onMarkerClicked) onMarkerClicked(row);
    }
};


class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::ListBoxModel,
    public juce::Timer
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void paint(juce::Graphics& g) override;
    int getNumRows() override;
    void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent& e) override;
    void listBoxItemDoubleClicked(int row, const juce::MouseEvent& e) override;
    void selectedRowsChanged(int lastRowSelected) override;
    void timerCallback() override;
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;
    MarkerListModel markerModel;


    juce::AudioTransportSource& getAudioSource() { return playerAudio.getTransportSource(); }


private:
    PlayerAudio playerAudio;

    juce::TextButton playPauseButton{ "Play/Pause" };
    juce::TextButton startButton{ "Start" };
    juce::TextButton endButton{ "End" };
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::Label titleLabel{ {}, "Name: -" };
    juce::Label durationLabel{ {}, "Time: 00:00" };
    juce::Slider volumeSlider;
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton loopButton{ "Loop" };
    juce::TextButton setAButton{ "Set A" };
    juce::TextButton setBButton{ "Set B" };
    juce::TextButton abLoopButton{ "A-B Loop" };
    bool isLooping = false;
    std::vector<juce::File> playlist;
    double pointA = 0.0;
    double pointB = 0.0;
    bool abLoopEnabled = false;

    juce::ListBox playlistBox{ "List", this };
    juce::TextButton forwardButton{ " +10s" };
    juce::TextButton backwardButton{ " -10s" };
    juce::TextButton shuffleButton{ "Shuffle: OFF" };
    bool isShuffle = false;

    juce::Slider positionSlider;
    juce::Label positionLabel{ {}, "00:00 / 00:00" };

    juce::TextButton addMarkerButton{ "Add Marker" };
    juce::ListBox markerListBox{ "Markers", this };
    std::vector<std::pair<int, double>> markers;
    juce::TextButton deleteMarkerButton{ "Delete Marker" };

    juce::Slider speedSlider;
    juce::Label speedLabel{ {}, "Speed: 1.0x" };


    juce::AudioFormatManager thumbnailFormatManager;
    juce::AudioThumbnailCache thumbnailCache{ 5 };
    juce::AudioThumbnail thumbnail{ 512, thumbnailFormatManager, thumbnailCache };
    bool thumbnailLoaded = false;


    std::unique_ptr<juce::FileChooser> fileChooser;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    void saveSession();
    void loadSession();


    bool isPlaying = false;

    bool isMuted = false;
    float previousGain = 0.5f;
    juce::Rectangle<int> thumbnailArea;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
