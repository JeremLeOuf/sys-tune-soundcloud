#pragma once

#include <string>
#include <vector>
#include <memory>
#include "http_client.hpp"

namespace tune::impl {

struct YouTubeTrack {
    std::string video_id;      // YouTube video ID (e.g., "dQw4w9WgXcQ")
    std::string title;         // Track title
    std::string channel_title; // Artist/Channel name
    std::string thumbnail_url; // Album art URL
    std::string description;   // Video description
    std::string duration;      // Video duration (ISO 8601 format like "PT4M13S")
    bool streamable = true;    // Always true for YouTube
};

struct YouTubePlaylist {
    std::string playlist_id;
    std::string title;
    std::string description;
    std::string thumbnail_url;
    std::vector<YouTubeTrack> tracks;
};

class YouTubeMusicAPI {
public:
    static Result Initialize();
    static void Exit();
    
    // Set API key (call this with your YouTube API key)
    static void SetAPIKey(const std::string& api_key);
    
    // Search for music videos
    static Result SearchTracks(const std::string& query, std::vector<YouTubeTrack>& tracks, int limit = 20);
    
    // Get video details by ID
    static Result GetVideoDetails(const std::string& video_id, YouTubeTrack& track);
    
    // Get video from YouTube URL (extract video ID and get details)
    static Result GetTrackFromURL(const std::string& youtube_url, YouTubeTrack& track);
    
    // Get audio stream URL for a video (placeholder for future implementation)
    static Result GetAudioStreamURL(const std::string& video_id, std::string& stream_url);
    
    // Get playlist contents
    static Result GetPlaylistTracks(const std::string& playlist_id, std::vector<YouTubeTrack>& tracks);
    
private:
    static std::string s_api_key;
    static bool s_initialized;
    
    static Result ParseVideoJSON(const std::string& json, YouTubeTrack& track);
    static Result ParseSearchJSON(const std::string& json, std::vector<YouTubeTrack>& tracks);
    static std::string UrlEncode(const std::string& value);
    static std::string ExtractVideoIdFromURL(const std::string& url);
};

} // namespace tune::impl
