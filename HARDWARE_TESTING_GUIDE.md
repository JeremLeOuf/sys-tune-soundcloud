# 🎵 Hardware Testing Guide for sys-tune-soundcloud

## ✅ Build Status
**SUCCESS!** All components built successfully:
- ✅ sys-tune sysmodule: `sys-tune.nsp` (238 KB)
- ✅ Tesla overlay: `sys-tune-overlay.ovl` (397 KB)
- ✅ Distribution package: `dist/` directory ready for Switch

Build completed: August 27, 2025

## 📦 What You Have Ready

### Built Files:
```
dist/
├── atmosphere/contents/4200000000000000/
│   ├── exefs.nsp          # Main sys-tune sysmodule
│   ├── flags/boot2.flag   # Auto-start flag
│   └── toolbox.json       # System module config
└── switch/.overlays/
    └── sys-tune-overlay.ovl # Tesla overlay
```

## 🎯 Hardware Testing Steps

### Step 1: Prerequisites
Before testing on hardware, ensure you have:
- [ ] Nintendo Switch with Custom Firmware (Atmosphere)
- [ ] Tesla overlay system installed
- [ ] SD card with proper directory structure
- [ ] Ability to transfer files to Switch SD card

### Step 2: Backup Current Installation
If you have an existing sys-tune installation:
```bash
# Backup your current sys-tune installation
cp -r /path/to/switch/sd/atmosphere/contents/4200000000000000/ ~/sys-tune-backup/
cp /path/to/switch/sd/switch/.overlays/sys-tune-overlay.ovl ~/sys-tune-backup/
```

### Step 3: Install New Build
1. **Copy the distribution to your SD card:**
   ```bash
   # From your build directory:
   cd /mnt/c/Users/jdjao/OneDrive/__DEV/sys-tune-soundcloud/dist
   
   # Copy to Switch SD card (adjust path as needed):
   cp -r atmosphere/ /path/to/switch/sd/
   cp -r switch/ /path/to/switch/sd/
   ```

2. **Verify file structure on SD card:**
   ```
   SD:/atmosphere/contents/4200000000000000/
   ├── exefs.nsp
   ├── flags/boot2.flag
   └── toolbox.json
   
   SD:/switch/.overlays/
   └── sys-tune-overlay.ovl
   ```

### Step 4: First Boot Test
1. **Insert SD card into Switch**
2. **Reboot the Switch completely** (hold power for 15+ seconds, then power on)
3. **Check that system boots normally** - if it hangs, you may need to remove the boot2.flag temporarily

### Step 5: Tesla Overlay Test
1. **Open Tesla overlay**: L + DPad Down + Right Stick
2. **Navigate to sys-tune**
3. **Verify the interface loads**

Expected UI elements:
- ✅ Music browser option
- ✅ **NEW: SoundCloud option** ← This is what we're testing!
- ✅ Volume controls
- ✅ Status bar showing current track

### Step 6: SoundCloud Feature Test

#### 6.1 Basic Navigation
1. **Select "SoundCloud" from main menu**
2. **Expected behavior:**
   - Menu loads without crashes
   - Shows demo track list
   - Can navigate with DPad/stick

#### 6.2 Demo Track Testing  
You should see these demo tracks:
- "Chill Beats" by Lo-Fi Artist (3:45)
- "Electronic Dreams" by Synth Master (4:20) 
- "Acoustic Vibes" by Indie Folk (3:12)

#### 6.3 Add to Playlist Test
1. **Press A on any demo track**
2. **Expected: "Added to playlist" message**
3. **Go back to main menu → Playlist**
4. **Verify: SoundCloud URL appears in playlist** (starts with "https://")

#### 6.4 Audio Playback Test  
1. **In playlist, select the SoundCloud track**
2. **Press A to play**
3. **Expected audio: 440Hz sine wave tone** (not real music - this is the demo)
4. **Volume controls should work**
5. **Should be able to pause/stop**

## 🐛 Troubleshooting

### If System Won't Boot:
1. Remove `boot2.flag` temporarily
2. Test overlay manually first
3. Check Atmosphere logs

### If Overlay Crashes:
1. Check Tesla is working with other overlays
2. Verify file permissions on SD card
3. Try clean Tesla restart

### If No Audio:
- **This is expected!** Demo implementation plays sine wave
- Test that normal music files still work
- Real streaming requires network implementation

### If SoundCloud Menu Missing:
1. Verify overlay file copied correctly
2. Check file size matches build (397 KB)
3. Restart Tesla overlay system

## 📊 What Success Looks Like

### ✅ Full Success:
- System boots normally
- Tesla overlay works
- SoundCloud menu appears and is navigable
- Can add demo tracks to playlist
- Sine wave audio plays when selecting SoundCloud tracks
- No system crashes or stability issues

### ⚠️ Partial Success:
- Overlay works but no audio (check audio system)
- Interface works but crashes on certain actions (check logs)

### ❌ Failure:
- System won't boot (remove boot2.flag)
- Tesla crashes on sys-tune (file corruption/compatibility)

## 🔄 Next Development Steps

After successful hardware testing:

1. **Implement real HTTP client** for actual network requests
2. **Add SoundCloud API integration** with real credentials  
3. **Implement streaming audio** instead of sine wave demo
4. **Add error handling** for network failures
5. **Performance optimization** and memory management

## 📝 Test Results Log

Document your testing results:

**Date:** ________________
**Switch Model:** ________________  
**Atmosphere Version:** ________________
**Tesla Version:** ________________

**Tests:**
- [ ] System boot: ________________
- [ ] Tesla overlay access: ________________  
- [ ] SoundCloud menu: ________________
- [ ] Demo track list: ________________
- [ ] Add to playlist: ________________
- [ ] Audio playback: ________________
- [ ] System stability: ________________

**Issues found:** ________________

**Notes:** ________________

---

**Congratulations!** You're now running your own custom Nintendo Switch homebrew with SoundCloud integration! 🎵🚀
