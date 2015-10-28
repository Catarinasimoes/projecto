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

#ifndef MAGNIFICATOR_H
#define MAGNIFICATOR_H

#include "opencv2/opencv.hpp"
#include <Kinect.h>
#include <Kinect.Face.h>

// Local
#include "SpatialFilter.h"
#include "TemporalFilter.h"
#include "Buffer.hpp"
#include "EvmSettings.hpp"
#include "SlidingGraph.hpp"

// C++
#include "cmath"
#include "math.h"


using namespace std;

class Magnificator
{
public:
	/**
	Constructor.
	@param buffer Buffer with color frames captured to magnify.
	@param resultingBuffer Buffer containing magnified frames ready to preview.
	@param face_area Buffer containing ROI recct (area of the frame to process).
	@param settings Settings.
	*/
	Magnificator(Buffer<cv::Mat> &buffer, Buffer<cv::Mat> &resultingBuffer, Buffer<cv::Rect> &face_area, EvmSettings &settings);

	/**
	Amplifies frames in buffer and enqueues resulting frames in resultingBuffer
	*/
	void colorMagnify();



	void motionMagnify();
	/**
	Returns number of frames the downSampledMat should have to contain 2s of footage.
	@param fps Current framerate
	*/

	void waveletMagnify();

	int getOptimalBufferSize(int fps);

	int currentFrame;
	EvmSettings &evmSettings;
private:
	//Holds settings
	//EvmSettings &evmSettings;

	//original frames with no magnification
	Buffer<cv::Mat> &processingBuffer;

	//Data points buffer
	Buffer<cv::Rect> &faceArea;

	//Holds magnified images
	Buffer<cv::Mat> &magnifiedBuffer;

	//Buffer<cv::Mat> &magnifiedROIBuffer;
	//Buffer<cv::Rect> &capturePoints;

	//int currentFrame;

	std::vector<cv::Mat> motionPyramid;
	/*!
	* \brief lowpassHi (Motion magnification) Holds image pyramid of lowpassed current frame with
	*  high cutoff
	*/
	std::vector<cv::Mat> lowpassHi;
	/*!
	* \brief lowpassLo (Motion magnification) Holds image pyramid of lowpassed current frame with
	*  low cutoff
	*/
	std::vector<cv::Mat> lowpassLo;

	cv::Mat downSampledMat;

	void attenuate(const cv::Mat &src, cv::Mat &dst, double chromattenuation);

	/**
	Multiplies each channel by alpha
	@param src Mat to be amplified
	@param dst Mat to store the result
	@param alpha Amplification value
	*/
	void amplifyGaussian(const cv::Mat &src, cv::Mat &dst, int alpha);
	void amplifyLaplacian(const cv::Mat &src, cv::Mat &dst, int currlevel, float lambda, float delta, int exageration_factor, int levels, int alpha);

	vector< vector<cv::Mat> > wlMotionPyramid;
	/*!
	 * \brief lowpassHi (Motion magnification) Holds image pyramid of lowpassed current frame with
	 *  high cutoff
	 */
	vector< vector<cv::Mat> > wlLowpassHi;
	/*!
	 * \brief lowpassLo (Motion magnification) Holds image pyramid of lowpassed current frame with
	 *  low cutoff
	 */
	vector< vector<cv::Mat> > wlLowpassLo;

	void denoiseWavelet(vector< vector<cv::Mat> > &src, vector< vector<cv::Mat> > &dst, float threshold);
	/*!
	* \brief amplifyWavelet Adapted from amplify Laplacian for wavelet vector structure.
	* \param src Source vector.
	* \param dst Destination vector.
	* \param currentLevel Level of image pyramid that is amplified.
	*/
	void amplifyWavelet(const vector<cv::Mat> &src, vector<cv::Mat> &dst, int currentLevel, float lambda, float delta, int exxageration_factor);

	//brief amplifyGaussian (Color magnification) Amplifies a Gaussian image pyramid.



};
#endif