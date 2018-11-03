#include "OpenNIHelper.h"

openni::Device device;
openni::VideoStream color, depth;
openni::VideoFrameRef colorFrame, depthFrame;

void ni::openni2_init()
{
    openni::OpenNI::initialize();
    device.open(openni::ANY_DEVICE);

    color.create(device, openni::SENSOR_COLOR);
    color.start();

    depth.create(device, openni::SENSOR_DEPTH);
    depth.start();

    // device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);
}

void ni::openni2_getmat(cv::Mat &mat_color, cv::Mat &mat_depth)
{
    color.readFrame(&colorFrame);
    mat_color = cv::Mat(colorFrame.getHeight(), colorFrame.getWidth(), CV_8UC3, (uint8_t*)colorFrame.getData());
    cv::cvtColor(mat_color, mat_color, CV_BGR2RGB);

    depth.readFrame(&depthFrame);
    mat_depth = cv::Mat(depthFrame.getHeight(), depthFrame.getWidth(), CV_16UC1, (openni::DepthPixel*)depthFrame.getData());
}

void ni::openni2_destroy()
{
    color.stop();
    color.destroy();
    depth.stop();
    depth.destroy();
    device.close();
}
