#include "MicRecorder.h"

#include <iostream>
#include <thread>
#include <chrono>

void MicRecorder::dataCallback(
    ma_device* device,
    void* output,
    const void* input,
    ma_uint32 frameCount
)
{
    (void)output;

    auto* recorder = static_cast<MicRecorder*>(device->pUserData);

    if (recorder == nullptr || input == nullptr)
    {
        return;
    }

    const auto* inputSamples = static_cast<const int16_t*>(input);

    const size_t sampleCount =
        static_cast<size_t>(frameCount) * recorder->m_channels;

    recorder->m_samples.insert(
        recorder->m_samples.end(),
        inputSamples,
        inputSamples + sampleCount
    );
}

bool MicRecorder::recordToFile(const std::string& outputPath, int seconds)
{
    m_samples.clear();

    const size_t reserveSampleCount =
        static_cast<size_t>(m_sampleRate) *
        m_channels *
        static_cast<size_t>(seconds);

    m_samples.reserve(reserveSampleCount);

    ma_device_config config = ma_device_config_init(ma_device_type_capture);

    config.capture.format = m_format;
    config.capture.channels = m_channels;
    config.sampleRate = m_sampleRate;
    config.dataCallback = MicRecorder::dataCallback;
    config.pUserData = this;

    ma_device device;

    ma_result result = ma_device_init(nullptr, &config, &device);
    if (result != MA_SUCCESS)
    {
        std::cerr << "마이크 장치 초기화 실패" << std::endl;
        return false;
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS)
    {
        std::cerr << "마이크 녹음 시작 실패" << std::endl;
        ma_device_uninit(&device);
        return false;
    }

    std::cout << "마이크 녹음 시작: " << seconds << "초" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(seconds));

    ma_device_stop(&device);
    ma_device_uninit(&device);

    std::cout << "녹음 완료. 샘플 수: " << m_samples.size() << std::endl;

    return writeWavFile(outputPath);
}

bool MicRecorder::writeWavFile(const std::string& outputPath)
{
    ma_encoder_config encoderConfig = ma_encoder_config_init(
        ma_encoding_format_wav,
        m_format,
        m_channels,
        m_sampleRate
    );

    ma_encoder encoder;

    ma_result result = ma_encoder_init_file(
        outputPath.c_str(),
        &encoderConfig,
        &encoder
    );

    if (result != MA_SUCCESS)
    {
        std::cerr << "WAV 파일 생성 실패: " << outputPath << std::endl;
        return false;
    }

    const ma_uint64 frameCount =
        static_cast<ma_uint64>(m_samples.size() / m_channels);

    ma_uint64 framesWritten = 0;

    result = ma_encoder_write_pcm_frames(
        &encoder,
        m_samples.data(),
        frameCount,
        &framesWritten
    );

    ma_encoder_uninit(&encoder);

    if (result != MA_SUCCESS)
    {
        std::cerr << "WAV 파일 쓰기 실패" << std::endl;
        return false;
    }

    std::cout << "WAV 저장 완료: " << outputPath << std::endl;
    std::cout << "저장된 프레임 수: " << framesWritten << std::endl;

    return true;
}