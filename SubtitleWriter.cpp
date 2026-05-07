#include <fstream>
#include <iostream>
#include "SubtitleWriter.h"

using namespace std;

bool SaveTxt(const std::string& outputPath, const std::vector<TranscriptionSegment>& segments)
{
	ofstream file(outputPath);

	if (!file)
	{
		cout << "Failed to open output file: " << outputPath << endl;
		return false;
	}

	int segmentCount = static_cast<int>(segments.size());

	for (int i = 0 ; i < segmentCount ; ++i)
	{
		file << segments[i].text << endl;
	}

    return true;
}

string FormatTime(int64_t t)
{
	int64_t totalSeconds = t / 100;
	int hours = static_cast<int>(totalSeconds / 3600);
	int minutes = static_cast<int>((totalSeconds % 3600) / 60);
	int seconds = static_cast<int>(totalSeconds % 60);
	int milliseconds = static_cast<int>(t % 100) * 10;
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d,%03d", hours, minutes, seconds, milliseconds);
	return string(buffer);
}

bool SaveSrt(const std::string& outputPath, const std::vector<TranscriptionSegment>& segments)
{
	ofstream file(outputPath);

	if (!file)
	{
		cout << "Failed to open output file: " << outputPath << endl;
		return false;
	}

	int segmentCount = static_cast<int>(segments.size());

	for (int i = 0; i < segmentCount; ++i)
	{
		file << i + 1 << endl 
			<< FormatTime(segments[i].startMs) << " --> " << FormatTime(segments[i].endMs) << endl
			<< segments[i].text << endl
			<< endl;
	}

	return true;
}
