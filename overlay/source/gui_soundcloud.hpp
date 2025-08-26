#pragma once

#include "tune.h"
#include <tesla.hpp>
#include <string>
#include <vector>

struct SoundCloudTrack {
    std::string id;
    std::string title;
    std::string artist;
    std::string stream_url;
    int duration_ms = 0;
    bool streamable = false;
};

class SoundCloudGui final : public tsl::Gui {
private:
    std::vector<SoundCloudTrack> m_search_results;
    std::string m_search_query;
    bool m_is_searching = false;
    
public:
    SoundCloudGui();
    
    tsl::elm::Element *createUI() final;
    void update() final;
    
private:
    void performSearch(const std::string& query);
    void addTrackToPlaylist(const SoundCloudTrack& track);
};
