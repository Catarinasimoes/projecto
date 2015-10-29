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

#ifndef PREVIEWTHREAD_HPP
#define PREVIEWTHREAD_HPP

#include <thread>
#include <atomic>
#include "opencv2\opencv.hpp"

#include "Buffer.hpp"
#include "EvmSettings.hpp"
#include "SlidingGraph.hpp"

using namespace std;


class PreviewThread {
private:
	

	//Running thread that fetches frames from kinect
	std::thread workerThread;
	atomic<bool> isRunning;

	//Color frames to preview
	Buffer<cv::Mat> &magnifiedFrames;
	//Buffer<vector<cv::Point>> &face_points;
	//Buffer<int> &numberFrames;
	//Facepoints to display
	Buffer<vector<cv::Point>> &samplingPoints;

	//Holds current settings
	EvmSettings &evmSettings;

	//Heartbeat Graph
	SlidingGraph heartbeat_graph = SlidingGraph(800, 400, 0, 255, 200);

public:
	PreviewThread(Buffer<cv::Mat> &colorBuffer,Buffer<vector<cv::Point>> &sampling_points, EvmSettings &settings);
	
	//starts previewing thread
	void startPreviewing();

	//Displays magnified frames, tracking points, graph
	void preview();

	//stops previewing thread
	void stopPreviewing();
};


#endif