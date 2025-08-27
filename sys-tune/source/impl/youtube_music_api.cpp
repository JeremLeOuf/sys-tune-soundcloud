#include "youtube_music_api.hpp"
#include "http_client.hpp"
#include "../../../common/config/api_keys.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cctype>

namespace tune::impl {

// YouTube Data API v3 key - loaded from config
std::string YouTubeMusicAPI::s_api_key = "";
bool YouTubeMusicAPI::s_initialized = false;

Result YouTubeMusicAPI::Initialize() {
    if (s_initialized) {
        return 0;
    }
    
    // Load API key from config file
    s_api_key = tune::config::YOUTUBE_API_KEY;
    
    if (s_api_key == "YOUR_YOUTUBE_API_KEY_HERE" || s_api_key.empty()) {
        // API key not configured, but that's okay for testing
        // The app will work with mock data
    }
    
    s_initialized = true;
    return 0;
}

void YouTubeMusicAPI::Exit() {
    s_initialized = false;
}

void YouTubeMusicAPI::SetAPIKey(const std::string& api_key) {
    s_api_key = api_key;
}

std::string YouTubeMusicAPI::UrlEncode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
        }
    }

    return escaped.str();
}

std::string YouTubeMusicAPI::ExtractVideoIdFromURL(const std::string& url) {
    // Extract video ID from various YouTube URL formats:
    // https://www.youtube.com/watch?v=VIDEO_ID
    // https://youtu.be/VIDEO_ID
    // https://m.youtube.com/watch?v=VIDEO_ID
    
    size_t v_pos = url.find("v=");
    if (v_pos != std::string::npos) {
        size_t start = v_pos + 2;
        size_t end = url.find('&', start);
        if (end == std::string::npos) {
            return url.substr(start);
        } else {
            return url.substr(start, end - start);
        }
    }
    
    // Handle youtu.be format
    size_t youtu_be = url.find("youtu.be/");
    if (youtu_be != std::string::npos) {
        size_t start = youtu_be + 9;
        size_t end = url.find('?', start);
        if (end == std::string::npos) {
            return url.substr(start);
        } else {
            return url.substr(start, end - start);
        }
    }
    
    return ""; // Could not extract video ID
}

Result YouTubeMusicAPI::SearchTracks(const std::string& query, std::vector<YouTubeTrack>& tracks, int limit) {
    if (!s_initialized) {
        return MAKERESULT(Module_Libnx, LibnxError_NotInitialized);
    }
    
    // Build YouTube Data API v3 search URL
    std::string url = "https://www.googleapis.com/youtube/v3/search";
    url += "?part=snippet";
    url += "&q=" + UrlEncode(query);
    url += "&type=video";
    url += "&videoCategoryId=10";  // Music category
    url += "&maxResults=" + std::to_string(limit);
    url += "&key=" + s_api_key;
    
    HttpClient::Response response;
    Result rc = HttpClient::Get(url, response);
    
    if (R_FAILED(rc)) {
        return rc;
    }
    
    if (response.status_code != 200) {
        return MAKERESULT(Module_Libnx, LibnxError_BadInput);
    }
    
    return ParseSearchJSON(response.body, tracks);
}

Result YouTubeMusicAPI::GetVideoDetails(const std::string& video_id, YouTubeTrack& track) {
    if (!s_initialized) {
        return MAKERESULT(Module_Libnx, LibnxError_NotInitialized);
    }
    
    // Build YouTube Data API v3 videos URL for detailed info
    std::string url = "https://www.googleapis.com/youtube/v3/videos";
    url += "?part=snippet,contentDetails";
    url += "&id=" + video_id;
    url += "&key=" + s_api_key;
    
    HttpClient::Response response;
    Result rc = HttpClient::Get(url, response);
    
    if (R_FAILED(rc)) {
        return rc;
    }
    
    if (response.status_code != 200) {
        return MAKERESULT(Module_Libnx, LibnxError_BadInput);
    }
    
    return ParseVideoJSON(response.body, track);
}

Result YouTubeMusicAPI::GetTrackFromURL(const std::string& youtube_url, YouTubeTrack& track) {
    if (!s_initialized) {
        return MAKERESULT(Module_Libnx, LibnxError_NotInitialized);
    }
    
    // Extract video ID from URL
    std::string video_id = ExtractVideoIdFromURL(youtube_url);
    if (video_id.empty()) {
        return MAKERESULT(Module_Libnx, LibnxError_BadInput);
    }
    
    // Get video details using the extracted ID
    return GetVideoDetails(video_id, track);
}

Result YouTubeMusicAPI::GetAudioStreamURL(const std::string& video_id, std::string& stream_url) {
    // TODO: Implement audio stream extraction
    // This would require additional processing similar to youtube-dl
    // For now, return a placeholder
    stream_url = "https://youtube.com/watch?v=" + video_id;
    return 0;
}

Result YouTubeMusicAPI::GetPlaylistTracks(const std::string& playlist_id, std::vector<YouTubeTrack>& tracks) {
    if (!s_initialized) {
        return MAKERESULT(Module_Libnx, LibnxError_NotInitialized);
    }
    
    // Build YouTube Data API v3 playlistItems URL
    std::string url = "https://www.googleapis.com/youtube/v3/playlistItems";
    url += "?part=snippet";
    url += "&playlistId=" + playlist_id;
    url += "&maxResults=50";
    url += "&key=" + s_api_key;
    
    HttpClient::Response response;
    Result rc = HttpClient::Get(url, response);
    
    if (R_FAILED(rc)) {
        return rc;
    }
    
    if (response.status_code != 200) {
        return MAKERESULT(Module_Libnx, LibnxError_BadInput);
    }
    
    return ParseSearchJSON(response.body, tracks);
}

Result YouTubeMusicAPI::ParseVideoJSON(const std::string& json, YouTubeTrack& track) {
    // Simple JSON parser for YouTube API response
    // This is not robust but works for basic YouTube API responses
    
    // Look for video ID
    size_t id_pos = json.find("\"id\":");
    if (id_pos != std::string::npos) {
        size_t start = json.find('"', id_pos + 5) + 1;
        size_t end = json.find('"', start);
        if (start != std::string::npos && end != std::string::npos) {
            track.video_id = json.substr(start, end - start);
        }
    }
    
    // Look for title in snippet
    size_t title_pos = json.find("\"title\":");
    if (title_pos != std::string::npos) {
        size_t start = json.find('"', title_pos + 8) + 1;
        size_t end = json.find('"', start);
        if (start != std::string::npos && end != std::string::npos) {
            track.title = json.substr(start, end - start);
        }
    }
    
    // Look for channel title
    size_t channel_pos = json.find("\"channelTitle\":");
    if (channel_pos != std::string::npos) {
        size_t start = json.find('"', channel_pos + 15) + 1;
        size_t end = json.find('"', start);
        if (start != std::string::npos && end != std::string::npos) {
            track.channel_title = json.substr(start, end - start);
        }
    }
    
    // Look for description
    size_t desc_pos = json.find("\"description\":");
    if (desc_pos != std::string::npos) {
        size_t start = json.find('"', desc_pos + 14) + 1;
        size_t end = json.find('"', start);
        if (start != std::string::npos && end != std::string::npos) {
            track.description = json.substr(start, end - start);
        }
    }
    
    // Look for high quality thumbnail
    size_t thumb_pos = json.find("\"high\":");
    if (thumb_pos != std::string::npos) {
        size_t url_pos = json.find("\"url\":", thumb_pos);
        if (url_pos != std::string::npos) {
            size_t start = json.find('"', url_pos + 6) + 1;
            size_t end = json.find('"', start);
            if (start != std::string::npos && end != std::string::npos) {
                track.thumbnail_url = json.substr(start, end - start);
            }
        }
    }
    
    // Look for duration in contentDetails
    size_t duration_pos = json.find("\"duration\":");
    if (duration_pos != std::string::npos) {
        size_t start = json.find('"', duration_pos + 11) + 1;
        size_t end = json.find('"', start);
        if (start != std::string::npos && end != std::string::npos) {
            track.duration = json.substr(start, end - start);
        }
    }
    
    track.streamable = true;
    return 0;
}

Result YouTubeMusicAPI::ParseSearchJSON(const std::string& json, std::vector<YouTubeTrack>& tracks) {
    // Simple parser for YouTube search results
    tracks.clear();
    
    // Look for items array in the JSON
    size_t items_pos = json.find("\"items\":");
    if (items_pos == std::string::npos) {
        return MAKERESULT(Module_Libnx, LibnxError_BadInput);
    }
    
    // Find each item in the array
    size_t pos = items_pos;
    while ((pos = json.find("{\"kind\":\"youtube#searchResult\"", pos)) != std::string::npos) {
        // Find the end of this item
        size_t end = json.find("}", pos);
        if (end == std::string::npos) break;
        
        // Extract this item's JSON
        std::string item_json = json.substr(pos, end - pos + 1);
        
        YouTubeTrack track;
        
        // Parse video ID from id.videoId
        size_t video_id_pos = item_json.find("\"videoId\":");
        if (video_id_pos != std::string::npos) {
            size_t start = item_json.find('"', video_id_pos + 10) + 1;
            size_t end_id = item_json.find('"', start);
            if (start != std::string::npos && end_id != std::string::npos) {
                track.video_id = item_json.substr(start, end_id - start);
            }
        }
        
        // Parse title from snippet.title
        size_t title_pos = item_json.find("\"title\":");
        if (title_pos != std::string::npos) {
            size_t start = item_json.find('"', title_pos + 8) + 1;
            size_t end_title = item_json.find('"', start);
            if (start != std::string::npos && end_title != std::string::npos) {
                track.title = item_json.substr(start, end_title - start);
            }
        }
        
        // Parse channel title
        size_t channel_pos = item_json.find("\"channelTitle\":");
        if (channel_pos != std::string::npos) {
            size_t start = item_json.find('"', channel_pos + 15) + 1;
            size_t end_channel = item_json.find('"', start);
            if (start != std::string::npos && end_channel != std::string::npos) {
                track.channel_title = item_json.substr(start, end_channel - start);
            }
        }
        
        // Parse description
        size_t desc_pos = item_json.find("\"description\":");
        if (desc_pos != std::string::npos) {
            size_t start = item_json.find('"', desc_pos + 14) + 1;
            size_t end_desc = item_json.find('"', start);
            if (start != std::string::npos && end_desc != std::string::npos) {
                track.description = item_json.substr(start, end_desc - start);
            }
        }
        
        // Parse thumbnail URL (look for high quality)
        size_t thumb_pos = item_json.find("\"high\":");
        if (thumb_pos != std::string::npos) {
            size_t url_pos = item_json.find("\"url\":", thumb_pos);
            if (url_pos != std::string::npos) {
                size_t start = item_json.find('"', url_pos + 6) + 1;
                size_t end_url = item_json.find('"', start);
                if (start != std::string::npos && end_url != std::string::npos) {
                    track.thumbnail_url = item_json.substr(start, end_url - start);
                }
            }
        }
        
        track.streamable = true;
        
        // Only add if we got a valid video ID
        if (!track.video_id.empty()) {
            tracks.push_back(track);
        }
        
        pos = end + 1;
    }
    
    return 0;
}

} // namespace tune::impl
