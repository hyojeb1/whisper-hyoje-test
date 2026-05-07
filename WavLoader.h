#pragma once
// WAV 파일을 읽어서 PCM 데이터를 float 샘플로 변환하는 코드입니다.
#include <string>
#include <vector>

struct WavData
{
    int sampleRate = 0;
    int channels = 0;
    std::vector<float> samples{};
};


bool ReadWav16AsFloat(const std::string& path, WavData& out);