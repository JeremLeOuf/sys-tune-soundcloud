# sys-tune - Nintendo Switch Music Player

A homebrew music player for the Nintendo Switch with YouTube Music streaming support.

## Features

- **Local Music Playback**: MP3, FLAC, WAV audio file support
- **YouTube Music Streaming**: Stream music from YouTube's vast library
- **System Integration**: Fully integrated with Nintendo Switch volume settings
- **Background Operation**: Works while other applications are running
- **Tesla Overlay**: Easy access via overlay interface
- **Flexible Controls**: Browse by folder, playlist, or search

## Installation

### Prerequisites
- Nintendo Switch with custom firmware (Atmosphere)
- Tesla overlay system installed

### Setup Steps

1. **Download**: Get the latest release files
2. **Install**:
   - Copy `sys-tune.nsp` to `/atmosphere/contents/`
   - Copy `sys-tune-overlay.ovl` to `/.overlays/`
3. **Configure YouTube API**:
   - Get a YouTube Data API v3 key from [Google Cloud Console](https://console.developers.google.com/)
   - Copy `common/config/api_keys.hpp.example` to `common/config/api_keys.hpp`
   - Edit `common/config/api_keys.hpp` and replace `"YOUR_YOUTUBE_API_KEY_HERE"` with your API key
4. **Reboot** your Nintendo Switch

## Usage

### Opening the Player
1. Press **L + DPad_Down + RStick** to open Tesla overlay
2. Navigate to **sys-tune**
3. Choose your music source:
   - **SD Card**: Browse local music files
   - **🎵 YouTube Music**: Stream from YouTube

### Controls
- **A**: Select/Play
- **B**: Back/Stop  
- **X**: Shuffle toggle
- **Y**: Repeat mode toggle
- **L/R**: Volume adjustment
- **Left/Right**: Previous/Next track

## YouTube Music Integration

### Features
- Search YouTube's music library
- Browse track details and metadata
- Stream audio directly to your Switch
- No complex authentication (just API key)

### API Setup

1. Go to [Google Cloud Console](https://console.developers.google.com/)
2. Create a project and enable "YouTube Data API v3"
3. Create an API key
4. Copy `common/config/api_keys.hpp.example` to `common/config/api_keys.hpp`
5. Edit the new file and replace `"YOUR_YOUTUBE_API_KEY_HERE"` with your API key
6. Build the project - your API key will be compiled into the application

## Building from Source

### Prerequisites
- DevkitPro with libnx
- Switch development environment
- YouTube API key configured

### Build Commands

```bash
# Build the main sysmodule
cd sys-tune
make

# Build the Tesla overlay  
cd overlay
make
```

### Output Files
- `sys-tune/sys-tune.nsp` - Main sysmodule
- `overlay/sys-tune-overlay.ovl` - Tesla overlay

## Screenshots

![Main](/sample/libtesla_1586882452.jpg)
![Main](/sample/libtesla_1586882672.jpg)
![Main](/sample/libtesla_1586882735.jpg)

## Special Thanks

- [mackron](http://mackron.github.io/) for the awesome [audio decoders](https://github.com/mackron/dr_libs/)
- [WerWolv](https://werwolv.net/) for libtesla UI library
- [TotalJustice](https://github.com/ITotalJustice) for bug fixes and features
- YouTube Data API v3 for music streaming capabilities

## Info for Developers

IPC interface accessible via service wrappers in `/ipc/` directory.
Tesla overlay implementation in `/overlay/source/` uses these bindings.

---

**Enjoy unlimited music on your Nintendo Switch!** 🎵🎮
