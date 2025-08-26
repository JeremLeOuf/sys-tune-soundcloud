#pragma once

#include <string>
#include <vector>
#include <memory>
#include "http_client.hpp"

namespace tune::impl {

struct SoundCloudTrack {
    std::string id;
    std::string title;
    std::string artist;
    std::string stream_url;
    std::string artwork_url;
    int duration_ms = 0;
    bool streamable = false;
};

struct SoundCloudPlaylist {
    std::string id;
    std::string title;
    std::string description;
    std::vector<SoundCloudTrack> tracks;
};

class SoundCloudAPI {
public:
    static Result Initialize();
    static void Exit();
    
    // Search for tracks
    static Result SearchTracks(const std::string& query, std::vector<SoundCloudTrack>& tracks, int limit = 20);
    
    // Get track by ID
    static Result GetTrack(const std::string& track_id, SoundCloudTrack& track);
    
    // Get stream URL for a track
    static Result GetStreamURL(const std::string& track_id, std::string& stream_url);
    
    // Get user's playlists (requires authentication)
    static Result GetUserPlaylists(const std::string& user_id, std::vector<SoundCloudPlaylist>& playlists);
    
private:
    static std::string s_client_id;
    static bool s_initialized;
    
    static Result ParseTrackJSON(const std::string& json, SoundCloudTrack& track);
    static Result ParseTracksJSON(const std::string& json, std::vector<SoundCloudTrack>& tracks);
    static std::string UrlEncode(const std::string& value);
};

} // namespace tune::impl
