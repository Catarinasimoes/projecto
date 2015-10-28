#include "FaceDetectionThread.h"
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

using namespace cv;
using namespace std;


FaceDetectionThread::FaceDetectionThread(Buffer<cv::Mat> &clonedbuffer, Buffer<int> &numberFrames, Buffer<cv::Mat> &fdbuffer, EvmSettings &settings) : clonedFrames(clonedbuffer), numberFrames(numberFrames), fdFrames(fdbuffer), evmSettings(settings) {
}

void FaceDetectionThread::startPreviewing() {
	isRunning = true;
	//start worker thread
	workerThread = std::thread(&FaceDetectionThread::preview, this);
}

void FaceDetectionThread::preview() {
	cv::namedWindow("Face Detection", CV_WINDOW_NORMAL);

	cv::Mat image, frame;
	std::vector<cv::Point2f> points[2];
	//INICIALIZAR CASCADECLASSIFIERS
	CascadeClassifier face_cascade, eye_cascade;
	if (!face_cascade.load("haarcascade_frontalface_alt2.xml")) {
		printf("Error loading cascade file for face");
		return;
	}

	//INICIALIZAR MATRIZES : CAPTURE E GRAY
	Mat currentFrame, gray_img;
	//INICIALIZAR VECTORES DE RECTANGULOS(OLHOS E CARA)
	vector<Rect> faces, eyes;
	Buffer <int>numberf;//antes do ciclo começar, iniciar um buffer que para cada frame determina o n de caras
	// buffer de vectores de pontos -> o numero de pontos depende do numero de caras detectado
	while (isRunning) {

		cout << "hey";
		//clock_t start;// relogio começa
		//double diff;
		//start = clock();
		cv::Mat currentFrame = clonedFrames.dequeue(); //frame which will be "facedetected"

		//face_point.enqueue(vec);
		cout << faces.size();
		//numberFrames.enqueue(faces.size());//FOR EACH FRAME CUMULATE THE NUMBER OF FACES FOUND
		//cv::resize(currentFrame, currentFrame, cv::Size(), 0.5, 0.5);	//Very slow

		imshow("Face Detection", currentFrame); cvtColor(currentFrame, gray_img, CV_BGR2GRAY);
		cv::equalizeHist(gray_img, gray_img);
		face_cascade.detectMultiScale(gray_img, faces, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0, 0), cvSize(300, 300));
		std::vector<cv::Point> vec;
		for (int i = 0; i < faces.size(); i++)

		{//Para cada cara
			//PARA CADA CARA DETETCADA
			//2 PONTOS PARA DEFINIR RECTANGULO DA CARA
			//PONTO 1: 
			//POSISÇÃO X DA CARA+ POSIÇÃO
			cout << "i can go here";
			//iniciar um vector de pontos


			Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);

			Point pt2(faces[i].x, faces[i].y);
			Point pt3(faces[i].x + faces[i].width / 3, faces[i].y + faces[i].height / 2); //2,11

			Point pt4(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);//unique

			Point pt5(faces[i].x + faces[i].width / 3, faces[i].y + faces[i].height / 2);//4 13
			Point pt6(faces[i].x + faces[i].width / 4, faces[i].y + faces[i].height / 2);//5 12
			Point pt7(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 3);//6 10
			Point pt8(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height * 2 / 3);
			//Point pt7(faces[i].x + faces[i].width / 3, faces[i].y+faces[i].y/2);//7 
			Point pt9(faces[i].x + faces[i].width / 4, faces[i].y + faces[i].y*0.7);

			//Point pt9(faces[i].width + faces[i].x, faces[i].y + faces[i].height);

			//Point pt10(faces[i].x+faces[i].width / 2, faces[i].y);
			Point pt10(faces[i].x + faces[i].width * 2 / 3, faces[i].y + faces[i].height / 2);
			Point pt11(faces[i].x + faces[i].width * 3 / 4, faces[i].y + faces[i].height / 2);
			Point pt12(faces[i].x + faces[i].width * 2 / 3, faces[i].y + faces[i].height / 2);
			//Point pt14(faces[i].x , faces[i].y + faces[i].height / 3);
			//Point pt15(faces[i].x , faces[i].y + faces[i].y / 2);
			Point pt13(faces[i].x + faces[i].width * 3 / 4, faces[i].y + faces[i].y*0.7);

			Point pt14(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].y*0.8);
			Point pt15(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].y*0.1);
			Mat faceROI = gray_img(faces[i]);

			rectangle(currentFrame, pt1, pt2, cvScalar(0, 255, 0), 2, 8, 0);
			circle(currentFrame, pt3, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt4, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt5, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt6, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt7, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt8, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt9, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt10, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt11, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt12, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt13, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt14, 4, Scalar(0, 0, 255), 1, 8, 0);
			circle(currentFrame, pt15, 4, Scalar(0, 0, 255), 1, 8, 0);
			vec.push_back(pt1);
			vec.push_back(pt2);
			vec.push_back(pt3);
			vec.push_back(pt4);
			vec.push_back(pt5);
			vec.push_back(pt6);
			vec.push_back(pt7);
			vec.push_back(pt8);
			vec.push_back(pt9);
			vec.push_back(pt10);
			vec.push_back(pt11);
			vec.push_back(pt12);
			vec.push_back(pt13);
			vec.push_back(pt14);
			vec.push_back(pt15);
			cout << faces.size();
			/*diff = (std::clock() - start) / (double)CLOCKS_PER_SEC;
			cout << "printf: " << diff << '\n';*/
			//totalpoints.enqueue(vec);

		}
		cv::waitKey(1);		//needed ?
		//fdPoints.enqueue(vectorCircles);
		//fdFrames.enqueue(currentFrame);
	}

}


