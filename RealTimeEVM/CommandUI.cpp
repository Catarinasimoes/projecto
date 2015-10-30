/////**
//* Real Time non - envasive heartbeat reader using "Eulerian Video Magnification" algorithm.
//* Uses the OpenCV library and Microsoft Kinect One
//* Based on the work of :
//*Jens Schindel		<kontakt@jens - schindel.de>
//*      Joseph Pan			<https ://github.com/wzpan/QtEVM>
//*Nick D'Ademo			<https://github.com/nickdademo/qt-opencv-multithreaded>
//* @author Francisco Salgado <salgfrancisco@gmail.com>
//* @version: 1.0
//* /
#include "iostream"
#include <mex.h>
#include "fstream"
#include <engine.h>
#include <string.h>
#include <matrix.h>
#include <cmath>
#include "PreviewThread.hpp"
#include "CommandUI.h"
#include <sys/timeb.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <opencv2/core/utility.hpp>
#include "opencv2/video.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include<stdio.h>
#include<math.h>
#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<opencv2\objdetect\objdetect.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<vector>
#include <ctime>
#include <cstdio>

using namespace std;




//PreviewThread::PreviewThread(Buffer<cv::Mat> &colorBuffer, Buffer<vector<cv::Point>> &sampling_points, EvmSettings &settings) : magnifiedFrames(colorBuffer), samplingPoints(sampling_points), evmSettings(settings){
//}
CommandUI::CommandUI( EvmSettings &settings, Buffer<vector<cv::Rect>> &points) :   evmSettings(settings), Points(points) {
}

//starts previewing thread
void CommandUI::startPreviewing() {
	isRunning = true;
	//start worker thread
	workerThread = std::thread(&CommandUI::preview, this);
}



void CommandUI::preview() {
	while (isRunning){
		vector<cv::Rect> a = Points.dequeue();
		cv::Rect firstface = a[0];
		//showIntGraph("Rotation Angle", a, a[30]);
	}
	
}



//stops previewing thread
void CommandUI::stopPreviewing2() {
	isRunning = false;

	//Wait for thread to finish
	workerThread.join();
}