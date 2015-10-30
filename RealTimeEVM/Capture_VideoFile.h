#ifndef CAPTURE_VIDEOFILE_HPP
#define CAPTURE_VIDEOFILE_HPP

#include <iostream>
#include <atomic>
#include <thread>
#include "opencv2\opencv.hpp"
#include "Windows.h"
//#include "Kinect.h"
//#include <Kinect.Face.h>

#include "Buffer.hpp"
#include "EvmSettings.hpp"



using namespace std;
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL){
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}


class Capture_VideoFile {
private:
	//Running thread that fetches frames from kinect
	std::thread workerThread;
	atomic<bool> isRunning;


	//Color
	/*cv::VideoCapture capture;
	cv::Mat Frame;*/
	int frameheight, framewidth;
	//Body



	unsigned int bufferSize;



	//Frames buffer
	Buffer<cv::Mat> &framesBuffer;
	Buffer<cv::Mat> &clonedframesBuffer;
	//face points buffer
	Buffer<cv::Rect> &faceArea;

	//Data points buffer
	Buffer<vector<cv::Point>> &samplingPoints;

	//Buffer<cv::Rect> &capturePoints;
	//settings(for getting framerate)
	EvmSettings &evmSettings;

	/*cv::VideoCapture &capture;

	cv::Mat &frame;*/

public:

	Capture_VideoFile(Buffer<cv::Mat> &buffer, Buffer<cv::Mat> &clonedbuffer, Buffer<cv::Rect> &faceArea, Buffer<vector<cv::Point>> &samplingPoints, EvmSettings &settings);

	/**
	Connects to Microsoft Kinect sensor.
	@return true if connected successfully.
	*/
	//bool connectToKinect();

	/**
	Disconnects from Microsoft Kinect sensor
	@return true if disconnected successfully.
	*/
	//bool disconnectKinect();

	//Captures color and sampling points coordinates
	void CaptureCamera();

	bool startCapturing();

	bool stopCapturing();

	//starts capturing thread
	//bool startCapturing();

	//stops capturing thread
	//bool stopCapturing();
};

#endif