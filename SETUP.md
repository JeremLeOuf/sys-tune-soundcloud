# Setup Guide for sys-tune YouTube Music

## Quick Start

### 1. Get YouTube API Key

1. Go to [Google Cloud Console](https://console.developers.google.com/)
2. Create a new project or select an existing one
3. Enable **YouTube Data API v3**
4. Go to **Credentials** → **Create Credentials** → **API Key**
5. Copy your API key

### 2. Configure API Key

```bash
# Copy the example file
cp common/config/api_keys.hpp.example common/config/api_keys.hpp

# Edit the file and replace YOUR_YOUTUBE_API_KEY_HERE with your actual key
nano common/config/api_keys.hpp
```

### 3. Build

```bash
# Build sys-tune service
cd sys-tune
make

# Build overlay
cd ../overlay
make
```

### 4. Install

Copy the built files to your Nintendo Switch:
- `sys-tune/sys-tune.nsp` → `/atmosphere/contents/`
- `overlay/sys-tune-overlay.ovl` → `/.overlays/`

### 5. Enjoy!

Press **L + DPad_Down + RStick** to open overlay, then select **🎵 YouTube Music**

## Security Notes

- **Never commit your API key** - it's automatically ignored by Git
- **Keep your `api_keys.hpp` file private**
- **Monitor your API usage** on Google Cloud Console

## Troubleshooting

### "No API key configured"
- Make sure you copied and edited `api_keys.hpp`
- Rebuild after changing the API key

### "Mock data only"
- Your API key is not valid or quota exceeded
- Check Google Cloud Console for API status

---

**That's it! Your Switch now has YouTube Music streaming!** 🎵
