#include <iostream>
#include <opencv2/opencv.hpp>
#include "OpenNIHelper.h"

int main(int argc, char *argv[])
{
    cv::Mat depth, color;
    ni::openni2_init();
    while (true)
    {
        ni::openni2_getmat(color, depth);

        cv::imshow("color", color);
        cv::Mat adjMap;
        convertScaleAbs(depth, adjMap, 255.0 / 6000);
        cv::imshow("depth", adjMap);
        if (cv::waitKey(1) == 27) break;
    }
    ni::openni2_destroy();
    return 0;
}
