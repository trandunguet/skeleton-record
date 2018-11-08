#include "SkelRecorder.h"
#include "kinectbvh.h"

#define DEVICE_WIDTH 512
#define DEVICE_HEIGHT 424

void SkelRecorder::startRecording()
{
    if (this->isRecording) return;

    this->pKinectBVH = new KinectBVH();
    this->isRecording = true;
}

void SkelRecorder::stopRecording()
{
    if (!this->isRecording) return;

    this->isRecording = false;
    time_t nowtime = time(NULL);
    struct tm *local = localtime(&nowtime);
    char buf[256];
    sprintf(buf, "./data/%d-%d-%d-%d-%d-%d.bvh", local->tm_year+1900, local->tm_mon+1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
    this->pKinectBVH->SaveToBVHFile(buf);
    delete this->pKinectBVH;
    this->pKinectBVH = new KinectBVH();
    this->isCalibrated = false;
}

void SkelRecorder::calibrateSkeleton()
{
    this->pKinectBVH->CalibrateSkeleton();
    this->isCalibrated = true;
}

void SkelRecorder::processBonesOrientation(const nite::Skeleton &skel)
{
    vector<Joint> joints(JOINT_SIZE);
    // Fill joints
    for (int i = 0; i < JOINT_SIZE; i++)
    {
        nite::Point3f pos = skel.getJoint((nite::JointType)i).getPosition();
        joints[i].pos.x = pos.x;
        joints[i].pos.y = pos.y;
        // convert to right hand coordinate
        joints[i].pos.z = -pos.z;
        joints[i].tracked = skel.getJoint((nite::JointType)i).getPositionConfidence() > 0.5f;
    }

    // Add the positions of all joints.
    this->pKinectBVH->AddAllJointsPosition(&joints[0]);

    // Increase the frame number.
    this->pKinectBVH->IncrementNbFrames();
}

void SkelRecorder::drawSkeleton(cv::Mat& src, const cv::Point2f* aPoint, const nite::SkeletonJoint* aJoints)
{
    cv::line( src, aPoint[ 0], aPoint[ 1], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[ 1], aPoint[ 2], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[ 1], aPoint[ 3], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[ 2], aPoint[ 4], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[ 3], aPoint[ 5], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[ 4], aPoint[ 6], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[ 5], aPoint[ 7], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[ 1], aPoint[ 8], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[ 8], aPoint[ 9], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[ 8], aPoint[10], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[ 9], aPoint[11], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[10], aPoint[12], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[11], aPoint[13], cv::Scalar( 255, 0, 0 ), 3 );
    cv::line( src, aPoint[12], aPoint[14], cv::Scalar( 255, 0, 0 ), 3 );

    // p4f. draw joint
    for( int  s = 0; s < 15; ++ s )
    {
        if( aJoints[s].getPositionConfidence() > 0.5 )
            cv::circle( src, aPoint[s], 3, cv::Scalar( 0, 0, 255 ), 2 );
        else
            cv::circle( src, aPoint[s], 3, cv::Scalar( 0, 255, 0 ), 2 );
    }
}

void SkelRecorder::init()
{
    this->isCalibrated = false;
    this->isRecording = false;
    
    float tile_angle = 0.0f;
    ifstream rf("tilt-angle.txt");
    if (rf.is_open()) {
        rf >> tile_angle;
    }
    rf.close();

    // o2. Initial OpenNI
    openni::OpenNI::initialize();

    // o3. Open Device
    this->device.open( openni::ANY_DEVICE );

    // o4. create depth stream
    // this->depthStream.create( this->device, openni::SENSOR_DEPTH );

    // // o4a. set video mode
    // openni::VideoMode depthVideoMode;
    // depthVideoMode.setResolution( 512, 424 );
    // depthVideoMode.setFps( 30 );
    // depthVideoMode.setPixelFormat( openni::PIXEL_FORMAT_DEPTH_1_MM );
    // this->depthStream.setVideoMode(depthVideoMode);

    // o5. Create color stream
    this->colorStream.create( this->device, openni::SENSOR_COLOR );

    // o5a. set video mode
    openni::VideoMode colorVideoMode;
    colorVideoMode.setResolution(DEVICE_WIDTH, DEVICE_HEIGHT);
    colorVideoMode.setFps( 30 );
    colorVideoMode.setPixelFormat( openni::PIXEL_FORMAT_RGB888 );
    this->colorStream.setVideoMode( colorVideoMode);

    // o6. image registration
    this->device.setImageRegistrationMode( openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR );

    // n2. Initial NiTE
    nite::NiTE::initialize();

    // n3. create user tracker
    this->userTracker.create( &this->device );
    this->userTracker.setSkeletonSmoothingFactor( 0.0f );

    // p1. start
    this->colorStream.start();
    // this->depthStream.start();

    this->pKinectBVH = new KinectBVH();
}

void SkelRecorder::getNextFrame(cv::Mat& matColorFrame, cv::Mat& matDepthFrame)
{
    openni::VideoFrameRef colorFrame, depthFrame;
    this->colorStream.readFrame(&colorFrame);
    // this->depthStream.readFrame(&depthFrame);

    // p2b. convert data to OpenCV format
    matColorFrame = cv::Mat(colorFrame.getHeight(), colorFrame.getWidth(),
                          CV_8UC3, (void*)colorFrame.getData());
    // p2c. convert form RGB to BGR
    cv::cvtColor( matColorFrame, matColorFrame, cv::COLOR_RGB2BGR );

    // p3. get user frame
    nite::UserTrackerFrameRef userFrame;
    this->userTracker.readFrame( &userFrame );

    // get cv depth frame
    depthFrame = userFrame.getDepthFrame();
    cv::Mat tmp(depthFrame.getHeight(), depthFrame.getWidth(), CV_16UC1, (openni::DepthPixel*)depthFrame.getData());

    convertScaleAbs(tmp, matDepthFrame, 255.0 / 6000);
    cv::cvtColor(matDepthFrame, matDepthFrame, cv::COLOR_GRAY2BGR);
    matDepthFrame = matDepthFrame(cv::Rect(0, 0, DEVICE_WIDTH, DEVICE_HEIGHT));

    // p4. get users data
    const nite::Array<nite::UserData>& users = userFrame.getUsers();
    for( int i = 0; i < users.getSize(); ++ i )
    {
        const nite::UserData& user = users[i];

        // p4a. check user status
        if( user.isNew() )
        {
            // start tracking for new user
            this->userTracker.startSkeletonTracking( user.getId() );
        }

        if( !user.isVisible() ) continue;

        // p4b. get user skeleton
        const nite::Skeleton& rSkeleton = user.getSkeleton();
        if( rSkeleton.getState() != nite::SKELETON_TRACKED ) continue;

        // start recording
        // if (!this->isRecording)
        // {
        //     this->pKinectBVH->SetTiltAngle(tile_angle);
        // }

        // calibrate T pose
        if (!this->isCalibrated)
        {
            this->calibrateSkeleton();
        }

        // record movement information
        if (this->isRecording && this->isCalibrated)
        {
            this->processBonesOrientation(rSkeleton);
        }

        // p4c. build joints array
        nite::SkeletonJoint aJoints[15];
        aJoints[ 0] = rSkeleton.getJoint( nite::JOINT_HEAD );
        aJoints[ 1] = rSkeleton.getJoint( nite::JOINT_NECK );
        aJoints[ 2] = rSkeleton.getJoint( nite::JOINT_LEFT_SHOULDER );
        aJoints[ 3] = rSkeleton.getJoint( nite::JOINT_RIGHT_SHOULDER );
        aJoints[ 4] = rSkeleton.getJoint( nite::JOINT_LEFT_ELBOW );
        aJoints[ 5] = rSkeleton.getJoint( nite::JOINT_RIGHT_ELBOW );
        aJoints[ 6] = rSkeleton.getJoint( nite::JOINT_LEFT_HAND );
        aJoints[ 7] = rSkeleton.getJoint( nite::JOINT_RIGHT_HAND );
        aJoints[ 8] = rSkeleton.getJoint( nite::JOINT_TORSO );
        aJoints[ 9] = rSkeleton.getJoint( nite::JOINT_LEFT_HIP );
        aJoints[10] = rSkeleton.getJoint( nite::JOINT_RIGHT_HIP );
        aJoints[11] = rSkeleton.getJoint( nite::JOINT_LEFT_KNEE );
        aJoints[12] = rSkeleton.getJoint( nite::JOINT_RIGHT_KNEE );
        aJoints[13] = rSkeleton.getJoint( nite::JOINT_LEFT_FOOT );
        aJoints[14] = rSkeleton.getJoint( nite::JOINT_RIGHT_FOOT );

        // p4d. convert joint position to image
        cv::Point2f aPoint[15];
        for( int  s = 0; s < 15; ++ s )
        {
            const nite::Point3f& rPos = aJoints[s].getPosition();
            this->userTracker.convertJointCoordinatesToDepth(
                                                        rPos.x, rPos.y, rPos.z,
                                                        &(aPoint[s].x), &(aPoint[s].y) );
        }

        // p4e. draw line
        drawSkeleton(matColorFrame, aPoint, aJoints);
        drawSkeleton(matDepthFrame, aPoint, aJoints);
    }
}

SkelRecorder::~SkelRecorder()
{
    if (this->pKinectBVH)
    {
        delete this->pKinectBVH;
        this->pKinectBVH = NULL;
    }

    this->userTracker.destroy();
    this->colorStream.destroy();
    this->depthStream.destroy();
    this->device.close();
    nite::NiTE::shutdown();
    openni::OpenNI::shutdown();
}
