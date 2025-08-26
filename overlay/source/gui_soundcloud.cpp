#include "gui_soundcloud.hpp"
#include "elm_overlayframe.hpp"

SoundCloudGui::SoundCloudGui() {
    // Initialize with some mock data for demonstration
    SoundCloudTrack demo_track;
    demo_track.id = "12345";
    demo_track.title = "Demo Song";
    demo_track.artist = "Demo Artist";
    demo_track.stream_url = "https://demo.soundcloud.com/stream.mp3";
    demo_track.duration_ms = 180000; // 3 minutes
    demo_track.streamable = true;
    
    m_search_results.push_back(demo_track);
    
    demo_track.id = "67890";
    demo_track.title = "Another Demo Song";
    demo_track.artist = "Another Artist";
    demo_track.stream_url = "https://demo.soundcloud.com/stream2.mp3";
    demo_track.duration_ms = 240000; // 4 minutes
    demo_track.streamable = true;
    
    m_search_results.push_back(demo_track);
}

tsl::elm::Element *SoundCloudGui::createUI() {
    auto frame = new SysTuneOverlayFrame();
    auto list = new tsl::elm::List();
    
    // Header
    list->addItem(new tsl::elm::CategoryHeader("SoundCloud"));
    
    // Search info
    auto search_info = new tsl::elm::ListItem("Search: Demo tracks shown");
    search_info->setValue("Press A to add to playlist");
    list->addItem(search_info);
    
    // Add search results
    for (const auto& track : m_search_results) {
        auto track_item = new tsl::elm::ListItem(track.title);
        
        // Format duration
        int minutes = track.duration_ms / 60000;
        int seconds = (track.duration_ms % 60000) / 1000;
        char duration_str[16];
        snprintf(duration_str, sizeof(duration_str), "%d:%02d", minutes, seconds);
        
        track_item->setValue(track.artist + " • " + duration_str);
        
        track_item->setClickListener([this, track](u64 keys) {
            if (keys & HidNpadButton_A) {
                this->addTrackToPlaylist(track);
                return true;
            }
            return false;
        });
        
        list->addItem(track_item);
    }
    
    // Instructions
    list->addItem(new tsl::elm::CategoryHeader(""));
    auto instructions = new tsl::elm::ListItem("Instructions");
    instructions->setValue("This is a demo of SoundCloud integration");
    list->addItem(instructions);
    
    frame->setContent(list);
    return frame;
}

void SoundCloudGui::update() {
    // Update search status or results if needed
}

void SoundCloudGui::performSearch(const std::string& query) {
    // In a real implementation, this would call the SoundCloud API
    m_search_query = query;
    m_is_searching = true;
    
    // For demo purposes, we already have some tracks loaded
    m_is_searching = false;
}

void SoundCloudGui::addTrackToPlaylist(const SoundCloudTrack& track) {
    if (!track.streamable) {
        // Show error message
        return;
    }
    
    // Add the stream URL to the playlist
    Result rc = tuneEnqueue(track.stream_url.c_str(), TuneEnqueueType_Back);
    if (R_SUCCEEDED(rc)) {
        // Show success message
        tsl::Overlay::get()->getCurrentGui()->requestFocus(nullptr, tsl::FocusDirection::None);
        // You would typically show a toast here, but we need access to the frame
    }
}
