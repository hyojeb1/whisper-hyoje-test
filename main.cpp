#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <whisper.h>

#include "WavLoader.h"
#include "WhisperTranscriber.h"
#include "SubtitleWriter.h"

using namespace std;

// 현재 명령줄 인자 - C:\Dev\35whisper\whisper.cpp\ggml-small.bin C:\Dev\35whisper\MySamples\sub01_01_김유진_광고.wav ko
int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        cout << "Usage:" << endl;
        cout << "  whisper_hyoje_test.exe <model_path> <wav_path> <language>" << endl;
        return 1;
    }

    string modelPath = argv[1];
    string wavPath = argv[2];
    string lang = argv[3];
	string txtOutputPath = wavPath + ".txt";
	string srtOutputPath = wavPath + ".srt";

    cout << "Model path: " << modelPath << endl;
    cout << "WAV path: " << wavPath << endl;
    cout << "Language: " << lang << endl;




    WhisperTranscriber mWhisperTranscriber{};
    if (!mWhisperTranscriber.LoadModel(modelPath.c_str()))
    {
        cout << "Failed to load model" << endl;
        return 1;
    }
    else
    {
        cout << "Model loaded successfully" << endl;
    }



   
    WavData wav;

    if (!ReadWav16AsFloat(wavPath, wav))
    {
        cout << "Failed to read WAV" << endl;
        mWhisperTranscriber.Release();
        return 1;
    }

    cout << "WAV loaded successfully" << endl;

    cout << "Sample rate: " << wav.sampleRate << endl;
    cout << "Channels: " << wav.channels << endl;
    cout << "Samples: " << wav.samples.size() << endl;

    
    if (wav.sampleRate != 16000)
    {
        cout << "Whisper expects 16 kHz audio. Current sample rate: "
            << wav.sampleRate << endl; 
        mWhisperTranscriber.Release();
        return 1;
    }

    vector<TranscriptionSegment> mTranscriptionSegment{};
    mTranscriptionSegment = mWhisperTranscriber.Transcribe( wav.samples, "ko" , 16 );

	SaveTxt(txtOutputPath, mTranscriptionSegment);
	SaveSrt(srtOutputPath, mTranscriptionSegment);

    mWhisperTranscriber.Release();

    return 0;
}