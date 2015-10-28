
//#ifndef OPTICALFLOW_HPP
//#define OPTICALFLOW_HPP
//
//#include <thread>
//#include <atomic>
//#include "opencv2\opencv.hpp"
//
//#include "Buffer.hpp"
//#include "EvmSettings.hpp"
////#include "SlidingGraph.hpp"
//
//using namespace std;
//
//
//class OpticalFlow {
//private:
//
//
//	//Running thread that fetches frames from kinect
//	//std::thread workerThread;
//	//atomic<bool> isRunning;
//
//	////Color frames to preview
//	//Buffer<cv::Mat> &magnifiedROIS;
//
//
//	////Holds current settings
//	//EvmSettings &evmSettings;
//
//
//public:
//	OpticalFlow(Buffer<cv::Mat> &roiBuffer, EvmSettings &settings);
//
//	//starts previewing thread
//	void startPreviewing();
//
//	//Displays magnified frames, tracking points, graph
//	void preview();
//
//	//stops previewing thread
//	void stopPreviewing();
//
//	//inline bool isFlowCorrect(Point2f u);
//	Vec3b computeColor(float fx, float fy);
//
// 	 void drawOpticalFlow(const Mat_<Point2f>& flow, Mat& dst, float maxmotion);
//
//	//static void writeOpticalFlowtoFile(const Mat_<Point2f>& flow);
//	 std::thread workerThread;
//	 atomic<bool> isRunning;
//	 Buffer<cv::Mat> &resroiBuffer;
//
//	 //Holds current settings
//
//	 EvmSettings &evmSettings;
//
//
//
//};
//
//
//#endif