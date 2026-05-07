#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <whisper.h>

struct TranscriptionSegment
{
    int64_t startMs;
    int64_t endMs;
    std::string text;
};

class WhisperTranscriber
{
public:
    WhisperTranscriber() = default;

    bool LoadModel(const std::string& modelPath);

    std::vector<TranscriptionSegment> Transcribe(
        const std::vector<float>& samples,
        const std::string& language,
        int threads
    );

    void Release();

private:
    whisper_context* context = nullptr;

};