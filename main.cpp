#include <iostream>
#include "MicRecorder.h"

using namespace std;

int main()
{
    cout << "miniaudio 마이크 녹음 테스트" << endl;

    MicRecorder recorder;

    const bool success = recorder.recordToFile("mic_test.wav", 5);

    if (!success)
    {
        cerr << "녹음 실패" << endl;
        return 1;
    }

    cout << "mic_test.wav 생성 완료" << endl;

    return 0;
}