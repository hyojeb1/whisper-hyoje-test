#pragma once

#include <string>
#include <vector>
#include "WhisperTranscriber.h"

bool SaveTxt(
    const std::string& outputPath,
    const std::vector<TranscriptionSegment>& segments
);

std::string FormatTime(int64_t t);

bool SaveSrt(
    const std::string& outputPath,
    const std::vector<TranscriptionSegment>& segments
);