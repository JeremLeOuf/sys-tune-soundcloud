#!/bin/bash
# Build script for sys-tune YouTube Music

echo "🎵 Building sys-tune YouTube Music..."

# Check if API key is configured
if [ ! -f "common/config/api_keys.hpp" ]; then
    echo "⚠️  API key not configured!"
    echo "📋 Run: cp common/config/api_keys.hpp.example common/config/api_keys.hpp"
    echo "✏️  Then edit the file with your YouTube API key"
    exit 1
fi

# Build nxExt library first
echo "🔧 Building nxExt library..."
make -C sys-tune/nxExt || exit 1

# Build sys-tune
echo "🔧 Building sys-tune sysmodule..."
make -C sys-tune -j4 || exit 1

# Build overlay
echo "🔧 Building Tesla overlay..."
make -C overlay -j4 || exit 1

# Create dist folder structure
echo "📂 Creating distribution folder structure..."
mkdir -p dist/atmosphere/contents/4200000000000000/
mkdir -p dist/switch/.overlays/

# Copy files to dist folders
echo "📦 Copying files to distribution folders..."
cp sys-tune/sys-tune.nsp dist/atmosphere/contents/4200000000000000/exefs.nsp
cp sys-tune/toolbox.json dist/atmosphere/contents/4200000000000000/
cp overlay/sys-tune-overlay.ovl dist/switch/.overlays/

echo ""
echo "✅ Build complete!"
echo "📁 Output files in dist folder:"
echo "   📄 atmosphere/contents/4200000000000000/exefs.nsp"
echo "   📄 switch/.overlays/sys-tune-overlay.ovl"
echo ""
echo "🚀 Ready to copy dist folder to your SD card!"
