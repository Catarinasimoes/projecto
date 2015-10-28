

//#include <gl\GL.h>
//#include <gl\GLU.h>
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

#include <Kinect.h>
#include <sys\timeb.h>

#include "CaptureThreadC.hpp"
#include "Buffer.hpp"
#include "Sampling.hpp"
#include<math.h>
#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<opencv2\objdetect\objdetect.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<vector>
#include <ctime>
#include <cstdio>



CaptureThreadC::CaptureThreadC(Buffer<cv::Mat> &buffer, Buffer<cv::Mat> &clonedbuffer, Buffer<cv::Rect> &face_area, Buffer<vector<cv::Point>> &sampling_points, EvmSettings &settings) :
framesBuffer(buffer), clonedframesBuffer(clonedbuffer), faceArea(face_area), samplingPoints(sampling_points), evmSettings(settings) {
}



void CaptureThreadC::CaptureCamera() {
	
	bool colorFrameAdded = false;
	cv::VideoCapture capture(0);
	//keep getting last frame until told to stop
	int frame_count = 0;
	while (isRunning) {

		//time stuff
		struct timeb start;
		ftime(&start);
		
		capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

		int frameheight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
		int framewidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);

		cv::Mat bufferMat(frameheight, framewidth, CV_8UC3);
		
		capture >> bufferMat;
		cv::resize(bufferMat, bufferMat, CvSize(1200, 600));
		
		framesBuffer.enqueue(bufferMat);
		
		colorFrameAdded = true;
		if (colorFrameAdded == true)
		{
			cv::Mat  currentFrame = bufferMat.clone();
			cv::Mat gray_img;
			vector<cv::Point> points;	//to hold this frame face points
			
			int topLeftx = framewidth / 8;
			int topLefty = frameheight / 3;
			int bottomRightx = framewidth * 2;
			int bottomRighty = frameheight;
			
			samplingPoints.enqueue(points);
	
			cv::Rect ROI(cv::Point(topLeftx, topLefty), cv::Point(bottomRightx, bottomRighty));
			
			faceArea.enqueue(ROI);
		}
	}

	colorFrameAdded = false;

	//regulate fps
	struct timeb end;
	ftime(&end);

}



bool CaptureThreadC::startCapturing() {
	isRunning = true;
	//start worker thread
	workerThread = std::thread(&CaptureThreadC::CaptureCamera, this);

	return true;
}

bool CaptureThreadC::stopCapturing(){
	isRunning = false;

	//Wait for thread to finish
	workerThread.join();

	return true;
}