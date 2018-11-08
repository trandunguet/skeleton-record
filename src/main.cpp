#include <opencv2/opencv.hpp>
#include "SkelRecorder.h"

int main()
{
    SkelRecorder recorder;
    recorder.init();
    cv::Mat color, depth;
    while(1)
    {
        recorder.getNextFrame(color, depth);
        cv::imshow("color", color);
        cv::imshow("depth", depth);
        cv::waitKey(1);
    }
}
