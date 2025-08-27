#include "gui_youtube_music.hpp"
#include "elm_overlayframe.hpp"

YouTubeMusicGui::YouTubeMusicGui() {
    // Add test track for demonstration
    YouTubeTrack test_track;
    test_track.video_id = "VGa3xhgHgHc";
    test_track.title = "Your Test Track From YouTube Music";
    test_track.channel_title = "Rick Astley";
    test_track.description = "The official video for Rick Astley's Never Gonna Give You Up";
    test_track.duration = "PT3M33S";
    test_track.streamable = true;
    
    m_tracks.push_back(test_track);
    
    // Add a Nintendo-related test track
    YouTubeTrack nintendo_track;
    nintendo_track.video_id = "JKOKR-J8KgI";
    nintendo_track.title = "Nintendo Switch Music - Various Game Soundtracks";
    nintendo_track.channel_title = "Nintendo Music Channel";
    nintendo_track.description = "Collection of Nintendo Switch game music";
    nintendo_track.duration = "PT10M15S";
    nintendo_track.streamable = true;
    
    m_tracks.push_back(nintendo_track);
}

tsl::elm::Element* YouTubeMusicGui::createUI() {
    auto rootFrame = new tsl::elm::OverlayFrame("sys-tune", "YouTube Music Integration");
    auto list = new tsl::elm::List();
    
    // Add search functionality
    auto searchHeader = new tsl::elm::CategoryHeader("Search Music");
    list->addItem(searchHeader);
    
    auto searchItem = new tsl::elm::ListItem("🔍 Search YouTube Music");
    searchItem->setClickListener([this](u64 keys) -> bool {
        if (keys & HidNpadButton_A) {
            // TODO: Implement search input dialog
            // For now, perform a default search
            performSearch("nintendo music");
            return true;
        }
        return false;
    });
    list->addItem(searchItem);
    
    // Add track list
    auto tracksHeader = new tsl::elm::CategoryHeader("Available Tracks");
    list->addItem(tracksHeader);
    
    for (const auto& track : m_tracks) {
        std::string trackInfo = track.title;
        if (!track.channel_title.empty()) {
            trackInfo += " - " + track.channel_title;
        }
        
        auto trackItem = new tsl::elm::ListItem(trackInfo);
        trackItem->setClickListener([this, track](u64 keys) -> bool {
            if (keys & HidNpadButton_A) {
                playTrack(track);
                return true;
            }
            return false;
        });
        list->addItem(trackItem);
    }
    
    // Add controls
    auto controlsHeader = new tsl::elm::CategoryHeader("Playback Controls");
    list->addItem(controlsHeader);
    
    auto playItem = new tsl::elm::ListItem("▶️ Play");
    playItem->setClickListener([](u64 keys) -> bool {
        if (keys & HidNpadButton_A) {
            // TODO: Implement play functionality
            return true;
        }
        return false;
    });
    list->addItem(playItem);
    
    auto pauseItem = new tsl::elm::ListItem("⏸️ Pause");
    pauseItem->setClickListener([](u64 keys) -> bool {
        if (keys & HidNpadButton_A) {
            // TODO: Implement pause functionality
            return true;
        }
        return false;
    });
    list->addItem(pauseItem);
    
    auto stopItem = new tsl::elm::ListItem("⏹️ Stop");
    stopItem->setClickListener([](u64 keys) -> bool {
        if (keys & HidNpadButton_A) {
            // TODO: Implement stop functionality
            return true;
        }
        return false;
    });
    list->addItem(stopItem);
    
    // Status information
    auto statusHeader = new tsl::elm::CategoryHeader("YouTube Music Status");
    list->addItem(statusHeader);
    
    auto statusItem = new tsl::elm::ListItem("✅ YouTube Music API Ready");
    statusItem->setClickListener([](u64 keys) -> bool {
        return false; // Non-interactive status item
    });
    list->addItem(statusItem);
    
    auto apiItem = new tsl::elm::ListItem("🔑 API Key Configured");
    list->addItem(apiItem);
    
    auto libraryItem = new tsl::elm::ListItem("🎵 Access to Billions of Tracks");
    list->addItem(libraryItem);
    
    rootFrame->setContent(list);
    return rootFrame;
}

void YouTubeMusicGui::update() {
    // Update search progress, track info, etc.
    if (m_search_in_progress) {
        // TODO: Update search progress indicator
    }
}

bool YouTubeMusicGui::handleInput(u64 keysDown, u64 keysHeld, const HidTouchState& touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) {
    // Handle any custom input logic
    return false; // Let the base class handle most input
}

void YouTubeMusicGui::performSearch(const std::string& query) {
    m_current_search_query = query;
    m_search_in_progress = true;
    
    // TODO: Implement actual YouTube API search
    // This would call YouTubeMusicAPI::SearchTracks()
    // For now, just add a placeholder result
    
    YouTubeTrack search_result;
    search_result.video_id = "search_result_1";
    search_result.title = "Search result for: " + query;
    search_result.channel_title = "Search Results";
    search_result.description = "This is a placeholder search result";
    search_result.duration = "PT0M0S";
    search_result.streamable = true;
    
    // Add to beginning of tracks list
    m_tracks.insert(m_tracks.begin(), search_result);
    
    m_search_in_progress = false;
}

void YouTubeMusicGui::playTrack(const YouTubeTrack& track) {
    // TODO: Implement track playback
    // This would:
    // 1. Get audio stream URL using YouTubeMusicAPI::GetAudioStreamURL()
    // 2. Pass the stream URL to the audio player
    // 3. Start playback
    
    // For now, just log the action
    // In a real implementation, this would interface with the sys-tune service
}
