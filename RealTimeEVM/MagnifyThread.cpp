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

#include "MagnifyThread.hpp"

MagnifyThread::MagnifyThread(Buffer<cv::Mat> &buffer, Buffer<cv::Mat> &resultingBuffer, Buffer<cv::Rect> &face_area, EvmSettings &settings) : mag(Magnificator(buffer, resultingBuffer, face_area, settings)) {
}

void MagnifyThread::magnify() {

	while (isRunning) {
		if (mag.evmSettings.getmode() == 2)
			mag.motionMagnify();
		else if (mag.evmSettings.getmode() == 1)
			mag.colorMagnify();
	}
}


//void MagnifyThread::magnify_color() {
//	/*set_mode(1);
//	cout << mode;*/
//	while (isRunning) {
//		
//		mag.colorMagnify();
//
//	}
//}


bool MagnifyThread::startMagnifying() {
	isRunning = true;
	int mode = mag.evmSettings.getmode();
	//start worker thread
	//if (mode == 1){
	workerThread = std::thread(&MagnifyThread::magnify, this);

	//}
	/*else if (mode == 2){
		workerThread = std::thread(&MagnifyThread::magnify_color, this);

		}*/
	return true;
}

bool MagnifyThread::stopMagnifying() {
	isRunning = false;

	//Wait for thread to finish
	workerThread.join();

	return true;
}