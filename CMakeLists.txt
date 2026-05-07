cmake_minimum_required(VERSION 3.16)
project(WhisperHyojeTest LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# whisper.cpp 저장소 경로
add_subdirectory("../whisper.cpp" whisper_build)

# miniaudio 저장소 경로
add_subdirectory("../miniaudio" miniaudio_build)



add_executable(whisper_hyoje_test
    main.cpp
    WavLoader.h
    WavLoader.cpp
    WhisperTranscriber.h
    WhisperTranscriber.cpp
    SubtitleWriter.h
    SubtitleWriter.cpp
)

target_link_libraries(whisper_hyoje_test PRIVATE
    whisper
    miniaudio
)