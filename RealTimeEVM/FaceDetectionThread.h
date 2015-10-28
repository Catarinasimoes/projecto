#ifndef FACEDETECTIONTHREAD_HPP
#define FACEDETECTIONTHREAD_HPP

#include <thread>
#include <atomic>
#include "opencv2\opencv.hpp"

#include "Buffer.hpp"
#include "EvmSettings.hpp"
using namespace std;
class FaceDetectionThread
{
public:
	Buffer <cv::Mat>&fdFrames;
	Buffer<int> &numberFrames;
	Buffer<cv::Mat> &clonedFrames;
	//Buffer<vector<cv::Point>> &face_point;
	//Buffer<vector<cv::Point>> &fdPoints;
	//Buffer<int> &nfaces;
	FaceDetectionThread(Buffer<cv::Mat> &clonedbuffer, Buffer<int> &numberFrames, Buffer<cv::Mat> &fdbuffer, EvmSettings &settings);

	void startPreviewing();

	void FaceDetectionThread::preview();
private:
	//Buffer <cv::Mat> &

	std::thread workerThread;
	atomic<bool> isRunning;



	//Buffer<cv::Rect> &capturePoints;

	//Holds current settings
	EvmSettings &evmSettings;
};

#endif