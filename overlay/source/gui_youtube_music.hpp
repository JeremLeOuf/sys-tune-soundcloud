#pragma once

#include <tesla.hpp>
#include <vector>
#include <string>

// Forward declare the YouTube track structure
struct YouTubeTrack {
    std::string video_id;
    std::string title;
    std::string channel_title;
    std::string thumbnail_url;
    std::string description;
    std::string duration;
    bool streamable = true;
};

class YouTubeMusicGui : public tsl::Gui {
private:
    std::vector<YouTubeTrack> m_tracks;
    std::string m_current_search_query;
    bool m_search_in_progress = false;
    
public:
    YouTubeMusicGui();
    
    virtual tsl::elm::Element* createUI() override;
    virtual void update() override;
    virtual bool handleInput(u64 keysDown, u64 keysHeld, const HidTouchState& touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) override;
    
private:
    void performSearch(const std::string& query);
    void playTrack(const YouTubeTrack& track);
    tsl::elm::Element* createTrackList();
};
