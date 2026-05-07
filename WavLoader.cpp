#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include "WavLoader.h"
using namespace std;

bool ReadWav16AsFloat(const string& path, WavData& out)
{
    ifstream file(path, ios::binary);

    if (!file)
    {
        cout << "Failed to open WAV file: " << path << endl;
        return false;
    }

    char riff[4];
    uint32_t riffSize;
    char wave[4];

    file.read(riff, 4);
    file.read(reinterpret_cast<char*>(&riffSize), 4);
    file.read(wave, 4);


    if (strncmp(riff, "RIFF", 4) != 0 || strncmp(wave, "WAVE", 4) != 0)
    {
        cout << "Not a RIFF/WAVE file" << endl;
        return false;
    }

    uint16_t audioFormat = 0;
    uint16_t numChannels = 0;
    uint32_t sampleRate = 0;
    uint16_t bitsPerSample = 0;

    vector<int16_t> pcm16;


    while (file)
    {
        char chunkId[4];
        uint32_t chunkSize = 0;

        file.read(chunkId, 4);
        file.read(reinterpret_cast<char*>(&chunkSize), 4);

        if (!file)
        {
            break; // End of file or read error
        }


        if (strncmp(chunkId, "fmt ", 4) == 0)
        {
            uint32_t byteRate = 0;
            uint16_t blockAlign = 0;

            file.read(reinterpret_cast<char*>(&audioFormat), 2);
            file.read(reinterpret_cast<char*>(&numChannels), 2);
            file.read(reinterpret_cast<char*>(&sampleRate), 4);
            file.read(reinterpret_cast<char*>(&byteRate), 4);
            file.read(reinterpret_cast<char*>(&blockAlign), 2);
            file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

            if (chunkSize > 16)
            {
                file.seekg(chunkSize - 16, ios::cur);
            }
        }
        else if (strncmp(chunkId, "data", 4) == 0)
        {
            if (audioFormat != 1)
            {
                cout << "Only PCM WAV is supported" << endl;
                return false;
            }

            if (bitsPerSample != 16)
            {
                cout << "Only 16-bit WAV is supported" << endl;
                return false;
            }

            const size_t sampleCount = chunkSize / sizeof(int16_t);
            pcm16.resize(sampleCount);

            file.read(reinterpret_cast<char*>(pcm16.data()), chunkSize);
        }
        else
        {
            // Skip unknown chunk
            file.seekg(chunkSize, ios::cur);
        }
    }


    if (pcm16.empty())
    {
        cout << "No audio data found" << endl;
        return false;
    }

    out.sampleRate = static_cast<int>(sampleRate);
    out.channels = static_cast<int>(numChannels);

    if (numChannels == 1)
    {
        out.samples.reserve(pcm16.size());

        for (int16_t s : pcm16)
        {
            out.samples.push_back(static_cast<float>(s) / 32768.0f);
        }
    }
    else if (numChannels == 2)
    {
        const size_t frameCount = pcm16.size() / 2;
        out.samples.reserve(frameCount);

        for (size_t i = 0; i < frameCount; ++i)
        {
            float left = static_cast<float>(pcm16[i * 2 + 0]) / 32768.0f;
            float right = static_cast<float>(pcm16[i * 2 + 1]) / 32768.0f;

            out.samples.push_back((left + right) * 0.5f);
        }
    }
    else
    {
        cout << "Unsupported number of channels: " << numChannels << endl;
        return false;
    }

    return true;

}