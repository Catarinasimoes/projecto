/**
* Real Time non-envasive heartbeat reader using "Eulerian Video Magnification" algorithm.
* Uses the OpenCV library and Microsoft Kinect One
* Based on the work of:
*      Jens Schindel		<kontakt@jens-schindel.de>
*      Joseph Pan			<https://github.com/wzpan/QtEVM>
*      Nick D'Ademo			<https://github.com/nickdademo/qt-opencv-multithreaded>
* @author Francisco Salgado <salgfrancisco@gmail.com>
* @version: 1.0
*/

#ifndef CAPTURETHREAD_HPP
#define CAPTURETHREAD_HPP

#include <iostream>
#include <atomic>
#include <thread>
#include "opencv2\opencv.hpp"
#include "Windows.h"
#include "Kinect.h"
#include <Kinect.Face.h>

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


class CaptureThread {
private:
	//Running thread that fetches frames from kinect
	std::thread workerThread;
	atomic<bool> isRunning;

	//Kinect stuff
	IKinectSensor* pSensor;
	//Color
	IColorFrameSource* pColorSource;
	IColorFrameReader* pColorReader;
	IFrameDescription* pDescription;
	int kinect_color_width, kinect_color_height;
	//Body
	IBodyFrameSource* pBodySource;
	IBodyFrameReader* pBodyReader;
	

	unsigned int bufferSize;

	//Buffer frames size
	int width, height;

	//Coordinate mapper
	ICoordinateMapper* pCoordinateMapper;

	//Kinect face stuff
	IHighDefinitionFaceFrameSource* pHDFaceSource[BODY_COUNT];
	IHighDefinitionFaceFrameReader* pHDFaceReader[BODY_COUNT];
	IFaceModelBuilder* pFaceModelBuilder[BODY_COUNT];

	IFaceAlignment* pFaceAlignment[BODY_COUNT];
	IFaceModel* pFaceModel[BODY_COUNT];
	vector<vector<float>> deformations;
	UINT32 vertex = 0;	//vertex count


	//Frames buffer
	Buffer<cv::Mat> &framesBuffer;

	//face points buffer
	Buffer<cv::Rect> &faceArea;

	//Data points buffer
	Buffer<vector<cv::Point>> &samplingPoints;

	//settings(for getting framerate)
	EvmSettings &evmSettings;

public:

	CaptureThread(Buffer<cv::Mat> &buffer, Buffer<cv::Rect> &faceArea, Buffer<vector<cv::Point>> &samplingPoints, EvmSettings &settings);

	/**
	Connects to Microsoft Kinect sensor.
	@return true if connected successfully.
	*/
	bool connectToKinect();

	/**
	Disconnects from Microsoft Kinect sensor
	@return true if disconnected successfully.
	*/
	bool disconnectKinect();

	//Captures color and sampling points coordinates
	void captureCamera();

	//starts capturing thread
	bool startCapturing();

	//stops capturing thread
	bool stopCapturing();
};

#endif