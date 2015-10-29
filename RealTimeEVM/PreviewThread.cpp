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
#include "iostream"
#include <mex.h>
#include "fstream"
#include <engine.h>
#include <string.h>
#include <matrix.h>
#include <cmath>
#include "PreviewThread.hpp"
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


cv::Point2f point;
bool addRemovePt = false;

static void onMouse(int event, int x, int y, int /*flags*/, void* /*param*/)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		point = cv::Point2f((float)x, (float)y);
		addRemovePt = true;
	}
}

//PreviewThread::PreviewThread(Buffer<cv::Mat> &colorBuffer, Buffer<vector<cv::Point>> &sampling_points, EvmSettings &settings) : magnifiedFrames(colorBuffer), samplingPoints(sampling_points), evmSettings(settings){
//}
PreviewThread::PreviewThread(Buffer<cv::Mat> &colorBuffer, Buffer<vector<cv::Point>> &sampling_points, EvmSettings &settings) : magnifiedFrames(colorBuffer), samplingPoints(sampling_points), evmSettings(settings) {
}

//starts previewing thread
void PreviewThread::startPreviewing() {
	isRunning = true;
	//start worker thread
	workerThread = std::thread(&PreviewThread::preview, this);
}



void PreviewThread::preview() {
	cv::namedWindow("Result", CV_WINDOW_NORMAL);
	//cv::Mat currentFrame;
	int currentmode = evmSettings.getmode();
	//if (currentmode == 2){
	//CASCADE FACE DETECTION : INICIALIZAR VARIAVEIS E LOAD DE FICHEIROS XML:


	while (isRunning) {

		cv::Mat currentFrame = magnifiedFrames.dequeue();


		imshow("Result", currentFrame);

		cv::waitKey(1);
	}
	//}
	//else if (currentmode == 1)
	//{
	//cv::namedWindow("Result", CV_WINDOW_NORMAL);
	//inicializar face cascade, ler ficheiros xml etc
	//if (evmSettings.getface() == 2){
	//	cv::CascadeClassifier face_cascade;
	//	if (!face_cascade.load("haarcascade_frontalface_alt2.xml")) {
	//		printf("Error loading cascade file for face");
	//		return;

	//	}
	//	cv::Mat gray_img, frame;
	//	std::vector<cv::Point> points[2]; //inicializar vector de pontos
	//	//INICIALIZAR VECTORES DE RECTANGULOS(CARA)
	//	vector<cv::Rect> faces;
	//}



	//while (isRunning) {
	//cv::Mat currentFrame = magnifiedFrames.dequeue();
	//
	//cv::resize(currentFrame, currentFrame, cv::Size(), 0.5, 0.5);	//Very slow

	//imshow("Face Detection", currentFrame); cvtColor(currentFrame, gray_img, CV_BGR2GRAY);
	//if (evmSettings.getface() == 2){
	//cv::equalizeHist(gray_img, gray_img);
	//face_cascade.detectMultiScale(gray_img, faces, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0, 0), cvSize(300, 300));
	////std::vector<cv::Point> vec;
	//for (int i = 0; i < faces.size(); i++)

	//{
	//	//PARA CADA CARA DETETCADA
	//	//2 PONTOS PARA DEFINIR RECTANGULO DA CARA
	//	//PONTO 1: 
	//	//POSISÇÃO X DA CARA+ POSIÇÃO
	//	cout << "i can go here";
	//	//iniciar um vector de pontos


	//	cv::Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);

	//	cv::Point pt2(faces[i].x, faces[i].y);
	//	cv::Point pt3(faces[i].x + faces[i].width / 3, faces[i].y + faces[i].height / 2); //2,11

	//	cv::Point pt4(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);//unique

	//	cv::Point pt5(faces[i].x + faces[i].width / 3, faces[i].y + faces[i].height / 2);//4 13
	//	cv::Point pt6(faces[i].x + faces[i].width / 4, faces[i].y + faces[i].height / 2);//5 12
	//	cv::Point pt7(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 3);//6 10
	//	cv::Point pt8(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height * 2 / 3);
	//	//Point pt7(faces[i].x + faces[i].width / 3, faces[i].y+faces[i].y/2);//7 
	//	cv::Point pt9(faces[i].x + faces[i].width / 4, faces[i].y + faces[i].y*0.7);

	//	//Point pt9(faces[i].width + faces[i].x, faces[i].y + faces[i].height);

	//	//Point pt10(faces[i].x+faces[i].width / 2, faces[i].y);
	//	cv::Point pt10(faces[i].x + faces[i].width * 2 / 3, faces[i].y + faces[i].height / 2);
	//	cv::Point pt11(faces[i].x + faces[i].width * 3 / 4, faces[i].y + faces[i].height / 2);
	//	cv::Point pt12(faces[i].x + faces[i].width * 2 / 3, faces[i].y + faces[i].height / 2);
	//	//Point pt14(faces[i].x , faces[i].y + faces[i].height / 3);
	//	//Point pt15(faces[i].x , faces[i].y + faces[i].y / 2);
	//	cv::Point pt13(faces[i].x + faces[i].width * 3 / 4, faces[i].y + faces[i].y*0.7);

	//	cv::Point pt14(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].y*0.8);
	//	cv::Point pt15(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].y*0.1);
	//	cv::Mat faceROI = gray_img(faces[i]);

	//	rectangle(currentFrame, pt1, pt2, cvScalar(0, 255, 0), 2, 8, 0);
	//	circle(currentFrame, pt3, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt4, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt5, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt6, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt7, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt8, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt9, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt10, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt11, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt12, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt13, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt14, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	circle(currentFrame, pt15, 4, cv::Scalar(0, 0, 255), 1, 8, 0);
	//	/*vec.push_back(pt1);
	//	vec.push_back(pt2);
	//	vec.push_back(pt3);
	//	vec.push_back(pt4);
	//	vec.push_back(pt5);
	//	vec.push_back(pt6);
	//	vec.push_back(pt7);
	//	vec.push_back(pt8);
	//	vec.push_back(pt9);
	//	vec.push_back(pt10);
	//	vec.push_back(pt11);
	//	vec.push_back(pt12);
	//	vec.push_back(pt13);
	//	vec.push_back(pt14);
	//	vec.push_back(pt15);
	//	cout << faces.size();*/
	//	/*diff = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	//	cout << "printf: " << diff << '\n';*/
	//	//totalpoints.enqueue(vec);
	//}
	//}
	//cv::imshow("Result", currentFrame);
	//cv::waitKey(1);		//needed ?
	////fdPoints.enqueue(vectorCircles);
	////fdFrames.enqueue(currentFrame);
}

//cv::waitKey(1);		//needed ?

////4. Display graph
//cv::Mat graph;
//heartbeat_graph.getGraph(graph);
//cv::imshow("Heartbeat", graph);
//cv::waitKey(1);
//}

//}



//stops previewing thread
void PreviewThread::stopPreviewing() {
	isRunning = false;

	//Wait for thread to finish
	workerThread.join();
}