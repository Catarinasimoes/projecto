//#include <iostream>
//#include <fstream>
//
//#include <opencv2/core/utility.hpp>
//#include "opencv2/video.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include <thread>
//#include <atomic>
//
//#include "OpticalFlow.h"
//#include "Buffer.hpp"
//#include "EvmSettings.hpp"
//using namespace cv;
//using namespace std;
//
//inline bool isFlowCorrect(Point2f u)
//{
//	return !cvIsNaN(u.x) && !cvIsNaN(u.y) && fabs(u.x) < 1e9 && fabs(u.y) < 1e9;
//};
//
//
//
//
//OpticalFlow::OpticalFlow(Buffer<cv::Mat> &roiBuffer, EvmSettings &settings) : resroiBuffer(roiBuffer),evmSettings(settings) {
//}
//
//
//
//
////
//////starts previewing thread
////void OpticalFlow::startPreviewing() {
////	isRunning = true;
////	//start worker thread
////	workerThread = std::thread(&OpticalFlow::preview, this);
////}
//
//
////static
//Vec3b OpticalFlow::computeColor(float fx, float fy)
//{
//	static bool first = true;
//
//	// relative lengths of color transitions:
//	// these are chosen based on perceptual similarity
//	// (e.g. one can distinguish more shades between red and yellow
//	//  than between yellow and green)
//	const int RY = 15;
//	const int YG = 6;
//	const int GC = 4;
//	const int CB = 11;
//	const int BM = 13;
//	const int MR = 6;
//	const int NCOLS = RY + YG + GC + CB + BM + MR;
//	static Vec3i colorWheel[NCOLS];
//
//	if (first)
//	{
//		int k = 0;
//
//		for (int i = 0; i < RY; ++i, ++k)
//			colorWheel[k] = Vec3i(255, 255 * i / RY, 0);
//
//		for (int i = 0; i < YG; ++i, ++k)
//			colorWheel[k] = Vec3i(255 - 255 * i / YG, 255, 0);
//
//		for (int i = 0; i < GC; ++i, ++k)
//			colorWheel[k] = Vec3i(0, 255, 255 * i / GC);
//
//		for (int i = 0; i < CB; ++i, ++k)
//			colorWheel[k] = Vec3i(0, 255 - 255 * i / CB, 255);
//
//		for (int i = 0; i < BM; ++i, ++k)
//			colorWheel[k] = Vec3i(255 * i / BM, 0, 255);
//
//		for (int i = 0; i < MR; ++i, ++k)
//			colorWheel[k] = Vec3i(255, 0, 255 - 255 * i / MR);
//
//		first = false;
//	}
//	const float rad = sqrt(fx * fx + fy * fy);
//	const float a = atan2(-fy, -fx) / (float)CV_PI;
//
//	const float fk = (a + 1.0f) / 2.0f * (NCOLS - 1);
//	const int k0 = static_cast<int>(fk);
//	const int k1 = (k0 + 1) % NCOLS;
//	const float f = fk - k0;
//
//	Vec3b pix;
//
//	for (int b = 0; b < 3; b++)
//	{
//		const float col0 = colorWheel[k0][b] / 255.f;
//		const float col1 = colorWheel[k1][b] / 255.f;
//
//		float col = (1 - f) * col0 + f * col1;
//
//		if (rad <= 1)
//			col = 1 - rad * (1 - col); // increase saturation with radius
//		else
//			col *= .75; // out of range
//
//		pix[2 - b] = static_cast<uchar>(255.f * col);
//	}
//
//	return pix;
//}
//
//void OpticalFlow::preview()
//
//{
//	while (isRunning){
//		int i = 0;
//		cv::Mat frame1, frame2;
//		if (i = 0){
//			cv::Mat frame1 = magnifiedROIS.dequeue();
//			i++;
//		} else {
//			cv::Mat frame2 = magnifiedROIS.dequeue();
//			i = 0;
//		}
//		Mat_<Point2f> flow;
//		Ptr<DenseOpticalFlow> tvl1 = createOptFlow_DualTVL1();
//
//		const double start = (double)getTickCount();
//		tvl1->calc(frame1, frame2, flow);
//		const double timeSec = (getTickCount() - start) / getTickFrequency();
//		cout << "calcOpticalFlowDual_TVL1 : " << timeSec << " sec" << endl;
//		Mat out;
//		drawOpticalFlow(flow, out,-1);
//		//cv::resize(currentFrame, currentFrame, cv::Size(), 0.5, 0.5);	//Very slow
//
//		imshow("Results", flow);
//		cv::waitKey(1);		//needed ?
//	}
//};
//
//
// void OpticalFlow::drawOpticalFlow(const Mat_<Point2f>& flow, Mat& dst, float maxmotion = -1)
//{
//	dst.create(flow.size(), CV_8UC3);
//	dst.setTo(Scalar::all(0));
//
//	// determine motion range:
//	float maxrad = maxmotion;
//
//	if (maxmotion <= 0)
//	{
//		maxrad = 1;
//		for (int y = 0; y < flow.rows; ++y)
//		{
//			for (int x = 0; x < flow.cols; ++x)
//			{
//				Point2f u = flow(y, x);
//
//				if (!isFlowCorrect(u))
//					continue;
//
//				maxrad = max(maxrad, sqrt(u.x * u.x + u.y * u.y));
//			}
//		}
//	}
//
//	for (int y = 0; y < flow.rows; ++y)
//	{
//		for (int x = 0; x < flow.cols; ++x)
//		{
//			Point2f u = flow(y, x);
//
//			if (isFlowCorrect(u))
//				dst.at<Vec3b>(y, x) = computeColor(u.x / maxrad, u.y / maxrad);
//		}
//	}
//}
//
// //static void OpticalFlow::writeOpticalFlowToFile(const Mat_<Point2f>& flow)
// //{
//	// static const char FLO_TAG_STRING[] = "PIEH";
//
//	// ofstream file(fileName.c_str(), ios_base::binary);
//
//	// file << FLO_TAG_STRING;
//
//	// file.write((const char*)&flow.cols, sizeof(int));
//	// file.write((const char*)&flow.rows, sizeof(int));
//
//	// for (int i = 0; i < flow.rows; ++i)
//	// {
//	//	 for (int j = 0; j < flow.cols; ++j)
//	//	 {
//	//		 const Point2f u = flow(i, j);
//
//	//		 file.write((const char*)&u.x, sizeof(float));
//	//		 file.write((const char*)&u.y, sizeof(float));
//	//	 }
//
//
//
//
//
//
////starts previewing thread
//void OpticalFlow::startPreviewing(){
//	isRunning = false;
//
//	//Wait for thread to finish
//	workerThread.join();
//};
//
////cv::Mat currentFrame = magnifiedFrames.dequeue();
////for (int i = 1; i <= 2; i++){
////	if (i = 1)
////		cv::Mat frame1 = currentFrame.clone();
////	else
////		cv::Mat frame2 = currentFrame.clone();
//
//void OpticalFlow::stopPreviewing() {
//	isRunning = false;
//
//	//Wait for thread to finish
//	workerThread.join();
//};