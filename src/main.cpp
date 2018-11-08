#include <opencv2/opencv.hpp>
#include <iostream>
#include "SkelRecorder.h"

SkelRecorder recorder;

void onChange(int i, void*)
{
    if (i == 1) recorder.startRecording();
    else if (i == 0) recorder.stopRecording();
}

int main()
{
    recorder.init();
    int value = 0;
    cv::Mat color, depth;
    while(1)
    {
        recorder.getNextFrame(color, depth);
        cv::imshow("color", color);
        cv::imshow("depth", depth);
        cv::createTrackbar("Status:\n1: Recording\n0: Stop", "color", &value, 1, onChange);
        cv::waitKey(1);
    }
}
