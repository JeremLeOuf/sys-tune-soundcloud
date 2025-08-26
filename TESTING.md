# Testing the SoundCloud Feature

## Quick Test Steps

1. **Build the project**:
   ```bash
   cd sys-tune
   make WANT_MP3=1 WANT_FLAC=1 WANT_WAV=1
   cd overlay
   make
   ```

2. **Install on Switch**:
   - Copy the built files to your Switch SD card
   - Install/update the sys-tune sysmodule
   - Install/update the Tesla overlay

3. **Test the feature**:
   - Open Tesla overlay (L + DPad Down + Right Stick)
   - Navigate to sys-tune
   - Select "SoundCloud" from the main menu
   - You should see demo tracks listed
   - Press A on a track to "add to playlist"
   - Go back to Playlist to see the track added
   - Play the track - you'll hear a sine wave tone (demo audio)

## What You Should See

- **Main Menu**: New "SoundCloud" option between "Music browser" and volume controls
- **SoundCloud Menu**: List of demo tracks with artist and duration info
- **Playlist**: SoundCloud URLs appear in playlist (starting with "https://")
- **Audio**: Sine wave tone plays instead of real audio (this is expected for demo)

## Expected Behavior

### Working ✅:
- UI navigation and layout
- Adding streams to playlist
- Audio pipeline integration (with demo sine wave)
- No crashes or system instability

### Not Yet Working ❌:
- Real HTTP requests (shows mock data)
- Actual SoundCloud API calls
- Real audio streaming from URLs
- Network error handling

## Development Testing

For developers working on this feature:

### Testing HTTP Client:
```cpp
// In a test function:
tune::impl::HttpClient::Response response;
Result rc = tune::impl::HttpClient::Get("https://httpbin.org/get", response);
// Check response.status_code and response.body
```

### Testing SoundCloud API:
```cpp
// Test search functionality:
std::vector<tune::impl::SoundCloudTrack> tracks;
Result rc = tune::impl::SoundCloudAPI::SearchTracks("test", tracks);
// Should return mock results currently
```

### Testing Network Source:
```cpp
// Test streaming source creation:
auto source = tune::impl::OpenStream("https://example.com/audio.mp3");
if (source && source->IsOpen()) {
    // Source created successfully
    // Will generate sine wave audio currently
}
```

## Debugging

### Common Issues:

1. **Build Errors**: 
   - Ensure all new source files are in the correct directories
   - Check that `#include` paths are correct
   - Verify Makefile includes new source directories

2. **UI Not Showing**:
   - Check that overlay was rebuilt and reinstalled
   - Verify Tesla is working with other overlays
   - Check for any overlay crashes in logs

3. **No Audio**:
   - This is expected - real streaming not implemented yet
   - Should hear sine wave tone when "playing" SoundCloud tracks
   - If no audio at all, check normal file playback works

4. **Network Issues**:
   - Network functionality is currently mocked
   - Real network calls will need proper Switch network initialization
   - Check Switch network connection for future real implementation

### Logs:

Check these locations for debugging info:
- Tesla overlay logs
- sys-tune service logs  
- Switch system logs

Remember: This is a proof-of-concept showing the integration framework. Real SoundCloud streaming requires additional implementation work as outlined in the main documentation.
