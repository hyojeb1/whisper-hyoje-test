cmake_minimum_required(VERSION 3.16)
project(WhisperHyojeTest LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("../whisper.cpp" whisper_build)

add_executable(whisper_hyoje_test
    main.cpp

    WavLoader.h
    WavLoader.cpp

    WhisperTranscriber.h
    WhisperTranscriber.cpp

    SubtitleWriter.h
    SubtitleWriter.cpp

    # 나중에 추가
    MicRecorder.h
    MicRecorder.cpp

    # miniaudio 소스
    ../miniaudio/miniaudio.c
)

target_include_directories(whisper_hyoje_test PRIVATE
    ../miniaudio
)

target_link_libraries(whisper_hyoje_test PRIVATE
    whisper
)