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

#include "opencv2\opencv.hpp"
#include <Kinect.h>
#include <Kinect.Face.h>

#include "Buffer.hpp"
#include "Capture_VideoFile.h"
#include "MagnifyThread.hpp"
#include "PreviewThread.hpp"
#include "Commands.hpp"
#include "EvmSettings.hpp"
#include "FaceDetectionThread.h"
using namespace std;
using namespace cv;

/// Global Variables
//int Alpha = 100;
//int Alpha;
//int alpha;
//double beta;






int main() {
	



	cout << "Real time EVM!" << endl;

	//Buffer to hold color frames being captured
	Buffer<cv::Mat> framesBuffer;

	Buffer<cv::Mat> clonedframesBuffer;
	//Buffer to hold magnified color frames
	Buffer<cv::Mat> magnifiedFrames;
	/*Buffer<cv::Mat> fdbuffer;
	Buffer<int>numberframes;*/
	//Buffer to hold face area, (area to magnify)
	Buffer<cv::Rect> faceArea;
	//Buffer<vector<cv::Point>> facepoints;
	//Buffer to hold sampling points to generate graph
	Buffer<vector<cv::Point>> samplingPoints;
	//Buffer<cv::Rect> capturePoints;

	Buffer<int> nframes;

	//Hold settings
	EvmSettings settings;
	//Set some default values
	settings.setAlpha(40);
	settings.setFramerate(20);
	settings.setLevels(4);
	settings.setFl(0.8);
	settings.setFh(1);
	settings.setcolow(20);
	settings.setcohigh(40);
	settings.setcowavelength(50);
	settings.setchromattenuation(0);
	settings.setmode(2);
	settings.setface(1);

	//Kinect stuff
	int devicenumber = 0;
	int width = 1000;
	int height = 500;
	Capture_VideoFile cap(framesBuffer, clonedframesBuffer, faceArea, samplingPoints, settings);
	//CaptureThreadC cap(framesBuffer, faceArea, samplingPoints, capturePoints, settings);
	/*if (!cap.connectToCam()) {
		cout << "Error Connecting to kinnect" << endl;
		return -1;
		}
		*/
	cap.startCapturing();
	//FaceDetectionThread fd(clonedframesBuffer, numberframes, fdbuffer, settings);
	MagnifyThread mag(framesBuffer, magnifiedFrames, faceArea, settings);
	mag.startMagnifying();
	//fd.startPreviewing();
	PreviewThread preview(magnifiedFrames, samplingPoints, settings);
	preview.startPreviewing();


	while (1) {
		string input;
		//on_trackbar(alpha_slider, 0);

		//settings.setAlpha(Alpha);
		//waitKey(60);
		getline(cin, input);
		waitKey(10);

		if (input.compare("stop") == 0) {
			break;
		}
		else {

			parseInput(input, settings);
		}
	}

	cap.stopCapturing();
	mag.stopMagnifying();
	preview.stopPreviewing();
	//	cap.disconnectKinect();

	return 0;
}

