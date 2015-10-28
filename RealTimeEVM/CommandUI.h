#ifndef COMMANDUI_H
#define COMMANDUI_H

#include <thread>
#include <atomic>
#include "opencv2\opencv.hpp"

#include "Buffer.hpp"
#include "EvmSettings.hpp"
#include "SlidingGraph.hpp"

using namespace std;


class CommandUI {
private:


	//Running thread that fetches frames from kinect
	std::thread workerThread;
	atomic<bool> isRunning;

	//Color frames to preview
	//Buffer<cv::Mat> &magnifiedFrames;
	//Buffer<vector<cv::Point>> &face_points;
	//Buffer<int> &numberFrames;
	//Facepoints to display
	//Buffer<vector<cv::Point>> &samplingPoints;

	//Holds current settings
	EvmSettings &evmSettings;

	//Heartbeat Graph
	//SlidingGraph heartbeat_graph = SlidingGraph(800, 400, 0, 255, 200);

public:
	Buffer<vector<cv::Rect>> &Points;
	CommandUI( EvmSettings &settings, Buffer<vector<cv::Rect>> &points);

	//starts previewing thread
	void startPreviewing();

	//Displays magnified frames, tracking points, graph
	void preview();

	//stops previewing thread
	void stopPreviewing2();
};


#endif