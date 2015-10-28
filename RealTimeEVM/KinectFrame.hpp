#ifndef KINECTFRAME_HPP
#define KINECTFRAME_HPP


#include "opencv2\opencv.hpp"
#include <Kinect.h>
#include <Kinect.Face.h>

using namespace std;

class KinectFrame {
private:
	cv::Mat colorFrame;
	vector<CameraSpacePoint> facePoints;

public:
	KinectFrame() {
	}


	void setColorFrame(cv::Mat &frame) {
		colorFrame = frame;
	}

	void getColorFrame(cv::Mat &dst) {
		dst = colorFrame;
	}

	void setFacePoints(vector<CameraSpacePoint> &points) {
		facePoints = points;
	}

	vector<CameraSpacePoint> getFacePoints() {
		return facePoints;
	}
};

#endif