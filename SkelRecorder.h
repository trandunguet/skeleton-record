#ifndef SKELRECORDER_H
#define SKELRECORDER_H

#include <opencv2/opencv.hpp>
#include <OpenNI.h>
#include <NiTE.h>

class KinectBVH;

class SkelRecorder
{
public:
    SkelRecorder() {};
    ~SkelRecorder();

    void init();
    void startRecording();
    void stopRecording();
    bool isRecording;
    void getNextFrame(cv::Mat& matColorFrame, cv::Mat& matDepthFrame);

private:
    void calibrateSkeleton();
    void processBonesOrientation(const nite::Skeleton &skel);
    void drawSkeleton(cv::Mat& src, const cv::Point2f* aPoint, const nite::SkeletonJoint* aJoints);

    bool isCalibrated;
    KinectBVH* pKinectBVH;

    openni::Device device;
    openni::VideoStream depthStream, colorStream;

    nite::UserTracker userTracker;
};

#endif
