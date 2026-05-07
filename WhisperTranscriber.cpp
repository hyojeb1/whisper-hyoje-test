// whisper.cpp의 API를 사용하여 모델을 로드하고, 오디오 샘플을 입력으로 받아 텍스트로 변환하는 기능을 구현합니다.
#include "WhisperTranscriber.h"
#include <iostream>

#include "WavLoader.h"
#ifndef DEBUG
#include "SubtitleWriter.h"
#endif // DEBUG
using namespace std;

bool WhisperTranscriber::LoadModel(const std::string& modelPath)
{
	whisper_context_params params = whisper_context_default_params();
	context = whisper_init_from_file_with_params(modelPath.c_str(), params);

	if (context == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::vector<TranscriptionSegment> WhisperTranscriber::Transcribe(const std::vector<float>& samples, const std::string& language, int threads)
{
    whisper_full_params fullParams = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);

    fullParams.language = language.c_str();
    fullParams.translate = false;
    fullParams.print_progress = false;
    fullParams.print_special = false;
    fullParams.print_realtime = false;
    fullParams.print_timestamps = false;
    fullParams.n_threads = threads;


    int result = whisper_full(
        context,
        fullParams,
        &samples[0],
        static_cast<int>(samples.size())
    );

    if (result != 0)
    {
        cout << "whisper_full failed. result = " << result << endl;

        Release();
        return std::vector<TranscriptionSegment>();
    }

    cout << "whisper_full succeeded" << endl;

    int segmentCount = whisper_full_n_segments(context);

    cout << "Segments: " << segmentCount << endl;

	std::vector<TranscriptionSegment> mTranscriptionSegment{};

    for (int i = 0; i < segmentCount; ++i)
    {
        int64_t t0 = whisper_full_get_segment_t0(context, i);
        int64_t t1 = whisper_full_get_segment_t1(context, i);
        const char* text = whisper_full_get_segment_text(context, i);

#ifndef DEBUG

        cout << i + 1 << endl
            << FormatTime(t0) << " --> " << FormatTime(t1) << endl
            << text << endl
            << endl;

#endif // DEBUG


        mTranscriptionSegment.push_back({ t0 * 10, t1 * 10, text });
    }

	return mTranscriptionSegment;
}

void WhisperTranscriber::Release()
{
    if (context)
    {
        whisper_free(context);
    }

	
}
