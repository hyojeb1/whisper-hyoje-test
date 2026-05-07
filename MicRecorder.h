#pragma once


#include <string>
#include <vector>
#include <cstdint>
#include <miniaudio.h>

class MicRecorder
{
public:
    bool recordToFile(const std::string& outputPath, int seconds);

private:
    static void dataCallback(
        ma_device* device,
        void* output,
        const void* input,
        ma_uint32 frameCount
    );

    bool writeWavFile(const std::string& outputPath);

private:
    std::vector<int16_t> m_samples;

    ma_uint32 m_sampleRate = 16000;
    ma_uint32 m_channels = 1;
    ma_format m_format = ma_format_s16;
};