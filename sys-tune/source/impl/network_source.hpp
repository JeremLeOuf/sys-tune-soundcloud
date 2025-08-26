#pragma once

#include "source.hpp"
#include "http_client.hpp"
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

namespace tune::impl {

class NetworkSource : public Source {
private:
    struct AudioChunk {
        std::vector<u8> data;
        size_t offset = 0;
        
        size_t remaining() const { return data.size() - offset; }
        const u8* current_data() const { return data.data() + offset; }
    };

    std::string m_url;
    std::unique_ptr<Source> m_decoder; // Decoder for the downloaded chunks
    
    // Streaming infrastructure
    std::thread m_download_thread;
    std::mutex m_chunk_mutex;
    std::condition_variable m_chunk_cv;
    std::queue<AudioChunk> m_chunk_queue;
    std::atomic<bool> m_download_complete{false};
    std::atomic<bool> m_should_stop{false};
    
    // Current chunk being processed
    AudioChunk m_current_chunk;
    
    // Audio format info (will be determined once we start decoding)
    int m_sample_rate = 0;
    int m_channels = 0;
    bool m_format_detected = false;
    
    void DownloadWorker();
    bool LoadNextChunk();

public:
    NetworkSource(const std::string& url);
    virtual ~NetworkSource();

    bool IsOpen() override;
    size_t Decode(size_t sample_count, s16 *data) override;
    std::pair<u32, u32> Tell() override;
    bool Seek(u64 target) override;

    int GetSampleRate() override;
    int GetChannelCount() override;

    // Network-specific methods
    bool StartStreaming();
    void StopStreaming();
    bool IsStreaming() const;
    
    // Override file operations to work with network data
    size_t ReadFile(void *buffer, size_t read_size) override;
    s64 TellFile() override;
    bool SeekFile(s64 offset, int origin) override;
};

} // namespace tune::impl
