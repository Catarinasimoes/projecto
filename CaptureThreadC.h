#ifndef CAPTURETHREADC_HPP
#define CAPTURETHREADC_HPP


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

class CaptureThreadC {
private:

	std::thread workerThread;
	atomic<bool> isRunning;

	//Kinect stuff
	cv::VideoCapture cap(0);
	//Color
	/*IColorFrameSource* pColorSource;
	IColorFrameReader* pColorReader;
	IFrameDescription* pDescription;*/
	int camera_width, camera_height;
	//Body
	/*IBodyFrameSource* pBodySource;
	IBodyFrameReader* pBodyReader;*/


	unsigned int bufferSize;

	//Buffer frames size
	int width, height;

	//Coordinate mapper
	//ICoordinateMapper* pCoordinateMapper;

	//Kinect face stuff
	/*IHighDefinitionFaceFrameSource* pHDFaceSource[BODY_COUNT];
	IHighDefinitionFaceFrameReader* pHDFaceReader[BODY_COUNT];
	IFaceModelBuilder* pFaceModelBuilder[BODY_COUNT];*/

	//IFaceAlignment* pFaceAlignment[BODY_COUNT];
	//IFaceModel* pFaceModel[BODY_COUNT];
	//vector<vector<float>> deformations;
	//UINT32 vertex = 0;	//vertex count


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
	bool connectToCam();

	/**
	Disconnects from Microsoft Kinect sensor
	@return true if disconnected successfully.
	*/
	//bool disconnectCam();

	//Captures color and sampling points coordinates
	void captureCamera();

	//starts capturing thread
	bool startCapturing();

	//stops capturing thread
	bool stopCapturing();
};

#endif

