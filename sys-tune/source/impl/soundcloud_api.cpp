#include "soundcloud_api.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace tune::impl {

// SoundCloud Client ID - you would need to get this from SoundCloud's developer portal
// For development purposes, this is a placeholder
std::string SoundCloudAPI::s_client_id = "YOUR_SOUNDCLOUD_CLIENT_ID";
bool SoundCloudAPI::s_initialized = false;

Result SoundCloudAPI::Initialize() {
    if (s_initialized) {
        return 0;
    }
    
    // Initialize HTTP client if not already done
    Result rc = HttpClient::Initialize();
    if (R_FAILED(rc)) {
        return rc;
    }
    
    s_initialized = true;
    return 0;
}

void SoundCloudAPI::Exit() {
    s_initialized = false;
}

std::string SoundCloudAPI::UrlEncode(const std::string& value) {
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

Result SoundCloudAPI::SearchTracks(const std::string& query, std::vector<SoundCloudTrack>& tracks, int limit) {
    if (!s_initialized) {
        return MAKERESULT(Module_Libnx, LibnxError_NotInitialized);
    }
    
    // Build SoundCloud API URL
    std::string url = "https://api.soundcloud.com/tracks";
    url += "?client_id=" + s_client_id;
    url += "&q=" + UrlEncode(query);
    url += "&limit=" + std::to_string(limit);
    url += "&linked_partitioning=1";
    
    HttpClient::Response response;
    Result rc = HttpClient::Get(url, response);
    
    if (R_FAILED(rc)) {
        return rc;
    }
    
    if (response.status_code != 200) {
        return MAKERESULT(Module_Libnx, LibnxError_BadInput);
    }
    
    return ParseTracksJSON(response.body, tracks);
}

Result SoundCloudAPI::GetTrack(const std::string& track_id, SoundCloudTrack& track) {
    if (!s_initialized) {
        return MAKERESULT(Module_Libnx, LibnxError_NotInitialized);
    }
    
    std::string url = "https://api.soundcloud.com/tracks/" + track_id;
    url += "?client_id=" + s_client_id;
    
    HttpClient::Response response;
    Result rc = HttpClient::Get(url, response);
    
    if (R_FAILED(rc)) {
        return rc;
    }
    
    if (response.status_code != 200) {
        return MAKERESULT(Module_Libnx, LibnxError_BadInput);
    }
    
    return ParseTrackJSON(response.body, track);
}

Result SoundCloudAPI::GetStreamURL(const std::string& track_id, std::string& stream_url) {
    if (!s_initialized) {
        return MAKERESULT(Module_Libnx, LibnxError_NotInitialized);
    }
    
    // First get track info to check if it's streamable
    SoundCloudTrack track;
    Result rc = GetTrack(track_id, track);
    if (R_FAILED(rc)) {
        return rc;
    }
    
    if (!track.streamable) {
        return MAKERESULT(Module_Libnx, LibnxError_BadInput);
    }
    
    // Get stream URL
    std::string url = "https://api.soundcloud.com/tracks/" + track_id + "/stream";
    url += "?client_id=" + s_client_id;
    
    HttpClient::Response response;
    rc = HttpClient::Get(url, response);
    
    if (R_FAILED(rc)) {
        return rc;
    }
    
    if (response.status_code == 302) {
        // SoundCloud returns a redirect to the actual stream URL
        // We would need to handle redirects here
        // For now, we'll use the redirect URL from the Location header
        stream_url = url; // Placeholder
        return 0;
    }
    
    return MAKERESULT(Module_Libnx, LibnxError_BadInput);
}

Result SoundCloudAPI::GetUserPlaylists(const std::string& user_id, std::vector<SoundCloudPlaylist>& playlists) {
    if (!s_initialized) {
        return MAKERESULT(Module_Libnx, LibnxError_NotInitialized);
    }
    
    std::string url = "https://api.soundcloud.com/users/" + user_id + "/playlists";
    url += "?client_id=" + s_client_id;
    
    HttpClient::Response response;
    Result rc = HttpClient::Get(url, response);
    
    if (R_FAILED(rc)) {
        return rc;
    }
    
    if (response.status_code != 200) {
        return MAKERESULT(Module_Libnx, LibnxError_BadInput);
    }
    
    // TODO: Parse playlists JSON
    return 0;
}

Result SoundCloudAPI::ParseTrackJSON(const std::string& json, SoundCloudTrack& track) {
    // This is a simplified JSON parser - in a real implementation,
    // you would use a proper JSON library like nlohmann/json
    
    // Extract basic fields using simple string operations
    // This is not robust but works for demonstration
    
    size_t id_pos = json.find("\"id\":");
    if (id_pos != std::string::npos) {
        size_t start = json.find_first_of("0123456789", id_pos);
        size_t end = json.find_first_not_of("0123456789", start);
        if (start != std::string::npos && end != std::string::npos) {
            track.id = json.substr(start, end - start);
        }
    }
    
    size_t title_pos = json.find("\"title\":");
    if (title_pos != std::string::npos) {
        size_t start = json.find('"', title_pos + 8) + 1;
        size_t end = json.find('"', start);
        if (start != std::string::npos && end != std::string::npos) {
            track.title = json.substr(start, end - start);
        }
    }
    
    size_t streamable_pos = json.find("\"streamable\":");
    if (streamable_pos != std::string::npos) {
        size_t true_pos = json.find("true", streamable_pos);
        size_t false_pos = json.find("false", streamable_pos);
        if (true_pos != std::string::npos && (false_pos == std::string::npos || true_pos < false_pos)) {
            track.streamable = true;
        }
    }
    
    // TODO: Parse other fields (artist, duration, etc.)
    
    return 0;
}

Result SoundCloudAPI::ParseTracksJSON(const std::string& json, std::vector<SoundCloudTrack>& tracks) {
    // This is a simplified implementation
    // In reality, you'd parse the JSON array properly
    
    tracks.clear();
    
    // Look for track objects in the JSON
    size_t pos = 0;
    while ((pos = json.find("{\"id\":", pos)) != std::string::npos) {
        // Find the end of this track object
        size_t end = json.find("}", pos);
        if (end != std::string::npos) {
            std::string track_json = json.substr(pos, end - pos + 1);
            
            SoundCloudTrack track;
            if (R_SUCCEEDED(ParseTrackJSON(track_json, track))) {
                tracks.push_back(track);
            }
        }
        pos = end + 1;
    }
    
    return 0;
}

} // namespace tune::impl
