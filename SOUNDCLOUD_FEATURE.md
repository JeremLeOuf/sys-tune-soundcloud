# SoundCloud Streaming Feature Implementation

This branch implements SoundCloud streaming functionality for sys-tune. 

## What's Been Added

### Core Components

1. **HTTP Client** (`impl/http_client.hpp`, `impl/http_client.cpp`)
   - Provides HTTP GET/POST functionality for API calls
   - Currently uses Switch's native socket APIs (simplified implementation)
   - Ready for curl integration when available

2. **Network Source** (`impl/network_source.hpp`, `impl/network_source.cpp`)
   - New audio source type that can stream from URLs
   - Inherits from the existing Source class
   - Generates demo sine wave audio for testing
   - Designed to handle real streaming audio when HTTP client is fully implemented

3. **SoundCloud API** (`impl/soundcloud_api.hpp`, `impl/soundcloud_api.cpp`)
   - Interface for SoundCloud's REST API
   - Supports searching tracks, getting track details, and stream URLs
   - Currently has mock responses for development/testing

4. **SoundCloud UI** (`overlay/source/gui_soundcloud.hpp`, `overlay/source/gui_soundcloud.cpp`)
   - New overlay GUI for browsing and playing SoundCloud tracks
   - Integrated into the main overlay menu
   - Shows demo tracks and allows adding them to the playlist

### Changes to Existing Code

1. **Source System** (`impl/source.hpp`, `impl/source.cpp`)
   - Added `STREAM` source type
   - Added `OpenStream()` function for URL-based sources
   - Added `IsStreamURL()` helper function

2. **Music Player** (`impl/music_player.cpp`)
   - Modified `PlayTrack()` to handle both files and streams
   - Automatically detects URLs vs file paths

3. **Main Application** (`source/main.cpp`)
   - Initializes HTTP client and SoundCloud API on startup
   - Properly cleans up network resources on exit

4. **Overlay UI** (`overlay/source/gui_main.cpp`)
   - Added SoundCloud option to main menu

## Current Status

This is a **proof-of-concept implementation** that demonstrates:
- ✅ Integration points for SoundCloud streaming
- ✅ UI framework for browsing SoundCloud content
- ✅ Audio source architecture that supports streaming
- ✅ Basic network infrastructure

## What's Still Needed

For a production-ready implementation:

1. **Real HTTP Implementation**
   - Integrate libcurl or implement proper HTTP over sockets
   - Handle redirects, SSL/TLS, and error conditions
   - Add proper authentication for SoundCloud API

2. **Audio Format Handling**
   - Detect audio format from streamed data
   - Integrate with existing dr_libs decoders (MP3, FLAC, etc.)
   - Handle progressive/chunked audio loading

3. **SoundCloud API Integration**
   - Get real SoundCloud client ID
   - Implement proper JSON parsing (suggest nlohmann/json)
   - Add authentication for private tracks/playlists
   - Handle API rate limiting

4. **Error Handling & UX**
   - Network error handling and retries
   - Loading indicators in UI
   - Offline/network unavailable states
   - Progress indicators for streaming

5. **Performance & Memory**
   - Optimize buffering strategies
   - Memory usage monitoring
   - Background downloading
   - Cache management

## Building

The code should compile with the existing build system, but you'll need:
- Network access on the target Switch
- SoundCloud API credentials for real functionality

## Testing

Currently the implementation:
- Shows a "SoundCloud" option in the main overlay
- Displays demo tracks when selected
- Generates a sine wave tone when "playing" SoundCloud tracks
- Demonstrates the UI and integration flow

## Future Enhancements

- Support for other streaming services (YouTube, Spotify, etc.)
- Playlist synchronization
- Offline caching of frequently played tracks
- User authentication and personal playlists
- Search functionality with filters
- Track recommendations

## Development Notes

This implementation follows the existing sys-tune architecture:
- Uses the same audio pipeline and resampling
- Integrates with the existing Tesla overlay system
- Maintains compatibility with local file playback
- Follows the project's coding style and patterns

The modular design makes it easy to:
- Swap HTTP implementations
- Add other streaming services
- Extend the UI with more features
- Improve audio format handling
