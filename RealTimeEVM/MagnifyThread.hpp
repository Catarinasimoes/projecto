/**
* Real Time non-envasive heartbeat reader using "Eulerian Video Magnification" algorithm.
* Uses the OpenCV library and Microsoft Kinect One
* Based on the work of:
*      Jens Schindel		<kontakt@jens-schindel.de>
*      Joseph Pan		<https://github.com/wzpan/QtEVM>
*      Nick D'Ademo		<https://github.com/nickdademo/qt-opencv-multithreaded>
* @author Francisco Salgado
* @version: 1.0
*/

#ifndef MAGNIFY_THREAD_HPP
#define MAGNIFY_THREAD_HPP
#include <thread>
#include <atomic>

#include "Magnificator.h"
#include "Buffer.hpp"
#include "EvmSettings.hpp"

class MagnifyThread {
private:
	//Magnificator mag;
	std::thread workerThread;
	atomic<bool> isRunning;


public:
	Magnificator mag;
	/**
	Constructor.
	@param buffer Buffer with color frames captured to magnify.
	@param resultingBuffer Buffer containing magnified frames ready to preview.
	@param face_area Buffer containing ROI recct (area of the frame to process).
	@param settings Settings.
	*/

	//Buffer<cv::Mat> &framesBuffer;
	MagnifyThread(Buffer<cv::Mat> &buffer, Buffer<cv::Mat> &resultingBuffer, Buffer<cv::Rect> &face_area, EvmSettings &settings);


	//Magnifies frames in buffer
	void magnify();

	//void magnify_color();

	//void set_mode(int mode);
	//starts magnifying thread
	bool startMagnifying();

	//stops magnifying thread
	bool stopMagnifying();

	void amplifyWavelet(const vector<cv::Mat> &src, vector<cv::Mat> &dst, int currentLevel);
};
#endif