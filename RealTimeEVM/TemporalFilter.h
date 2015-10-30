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

#ifndef TEMPORALFILTER_H
#define TEMPORALFILTER_H
// OpenCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>


using namespace std;

////////////////////////
///Helper //////////////
////////////////////////
/*!
 * \brief img2tempMat (Color Magnification) Takes a Mat, reshapes it in 1 column with width*height rows & concatenates it
 *  on the right side of dst.
 * \param frame Input frame.
 * \param dst Matrice of concatenated reshaped frames, 1 column = 1 frame. Most right frame is most actual frame.
 * \param maxImages Maximum number of images hold in dst. If columns of dst > maxImages, the most
 *  left column is deleted. Value should be a power of 2 for fast DFT.
 */
void img2tempMat(const cv::Mat &frame, cv::Mat &dst, int maxImages);
/*!
 * \brief tempMat2img (Color Magnification) Takes a Mat of line-concatenated frames and reshapes 1 column back into a frame.
 * \param src Mat of concatenated frames.
 * \param position The column in src that shall get reshaped.
 * \param frameSize The destination size the reshaped frame shall have.
 * \param frame Output frame with size frameSize.
 */
void tempMat2img(const cv::Mat &src, int position, const cv::Size &frameSize, cv::Mat &frame);
/*!
 * \brief createIdealBandpassFilter (Color Magnification) Creates a filter mask for an ideal filter.
 * \param filter Filter mask.
 * \param cutoffLo Lower cutoff frequency.
 * \param cutoffHi Upper cutoff frequency.
 * \param framerate Framerate of processed video.
 */
void createIdealBandpassFilter(cv::Mat &filter, double cutoffLo, double cutoffHi, double framerate);

////////////////////////
///Filter //////////////
////////////////////////
/*!
 * \brief iirFilter (Euler Magnification) Applies an iirFilter (in space domain) on 1 level of a Laplace Pyramid.
 * \param src Newest input image of a level of a Laplace Pyramid.
 * \param dst Iir filtered level of a Laplace Pyramid.
 * \param lowpassHi Holding the informations about the previous (high) lowpass filtered images of a level.
 * \param lowpassLo Holding the informations about the previous (low) lowpass filtered images of a level.
 * \param cutoffLo Lower cutoff frequency.
 * \param cutoffHi Higher cutoff frequency.
 */
void iirFilter(const cv::Mat &src, cv::Mat &dst, cv::Mat &lowpassHi, cv::Mat &lowpassLo, float cutoffLo, float cutoffHi);
/*!
 * \brief iirWaveletFilter (Wavelet Magnification) Applies an iirFilter (in space domain) on 1 level of a DWT.
 * \param src
 * \param dst
 * \param lowpassHi
 * \param lowpassLo
 * \param cutoffLo
 * \param cutoffHi
 */
void iirWaveletFilter(const vector<cv::Mat> &src, vector<cv::Mat> &dst, vector<cv::Mat> &lowpassHi, vector<cv::Mat> &lowpassLo, double cutoffLo, double cutoffHi);
/*!
 * \brief idealFilter (Color Magnification)
 * \param src
 * \param dst
 * \param cutoffLo
 * \param cutoffHi
 * \param framerate
 */
void idealFilter(const cv::Mat &src,cv::Mat &dst, double cutoffLo, double cutoffHi, double framerate);

#endif // TEMPORALFILTER_H
