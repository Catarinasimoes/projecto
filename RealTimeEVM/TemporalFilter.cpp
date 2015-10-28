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

#include "TemporalFilter.h"

////////////////////////
///Filter //////////////
////////////////////////
void iirFilter(const cv::Mat &src, cv::Mat &dst, cv::Mat &lowpassHi, cv::Mat &lowpassLo,
               double cutoffLo, double cutoffHi)
{
    // Set minimum for cutoff, so low cutoff gets faded out
    if(cutoffLo == 0)
        cutoffLo = 0.01;

    /* The higher cutoff*, the faster the lowpass* image of the lowpass* pyramid gets faded out.
     * That means, a high cutoff weights new images (= \param src)
     * more than the old ones (= \param lowpass*), so long lasting movements are faded out fast.
     * The other way, a low cutoff evens out fast movements ocurring only in a few number of src images. */

	cv::Mat tmp1 = (1 - cutoffHi)*lowpassHi + cutoffHi*src;
	cv::Mat tmp2 = (1 - cutoffLo)*lowpassLo + cutoffLo*src;
    lowpassHi = tmp1;
    lowpassLo = tmp2;

    dst = lowpassHi - lowpassLo;
}

void iirWaveletFilter(const vector<cv::Mat> &src, vector<cv::Mat> &dst, vector<cv::Mat> &lowpassHi, vector<cv::Mat> &lowpassLo,
                      double cutoffLo, double cutoffHi)
{
    // Set minimum for cutoff, so low cutoff gets faded out
    if(cutoffLo == 0)
        cutoffLo = 0.01;

    /* The higher cutoff*, the faster the lowpass* image of the lowpass* pyramid gets faded out.
     * That means, a high cutoff weights new images (= \param src)
     * more than the old ones (= \param lowpass*), so long lasting movements are faded out fast.
     * The other way, a low cutoff evens out fast movements ocurring only in a few number of src images. */

    // Do this for every detail/coefficient image
    for(int dims = 0; dims < 3; dims++) {
		cv::Mat tmp1 = (1 - cutoffHi)*lowpassHi[dims] + cutoffHi*src[dims];
		cv::Mat tmp2 = (1 - cutoffLo)*lowpassLo[dims] + cutoffLo*src[dims];
        lowpassHi[dims] = tmp1;
        lowpassLo[dims] = tmp2;

        dst[dims] = lowpassHi[dims] - lowpassLo[dims];
    }
}

void idealFilter(const cv::Mat &src, cv::Mat &dst, double cutoffLo, double cutoffHi, double framerate)
{
    if(cutoffLo == 0.00)
        cutoffLo += 0.01;


	cv::Mat channels[3];

	cv::split(src, channels);

    // Apply filter on each channel individually
    for (int curChannel = 0; curChannel < 3; curChannel++) {
		cv::Mat current = channels[curChannel];
		cv::Mat tempImg;

        int width = current.cols;
		int height = cv::getOptimalDFTSize(current.rows);

        copyMakeBorder(current, tempImg,
                       0, height - current.rows,
                       0, width - current.cols,
					   cv::BORDER_CONSTANT, cv::Scalar::all(0));

        // DFT
		dft(tempImg, tempImg, cv::DFT_ROWS | cv::DFT_SCALE);

        // construct Filter
		cv::Mat filter = tempImg.clone();
		createIdealBandpassFilter(filter, cutoffLo, cutoffHi, framerate);

        // apply
		mulSpectrums(tempImg, filter, tempImg, cv::DFT_ROWS);

        // inverse
		idft(tempImg, tempImg, cv::DFT_ROWS | cv::DFT_SCALE);

		tempImg(cv::Rect(0, 0, current.cols, current.rows)).copyTo(channels[curChannel]);
    }
    merge(channels, 3, dst);

    normalize(dst, dst, 0, 1, CV_MINMAX);
}

void createIdealBandpassFilter(cv::Mat &filter, double cutoffLo, double cutoffHi, double framerate)
{
    float width = filter.cols;
    float height = filter.rows;

    // Calculate frequencies according to framerate and size
    double fl = 2 * cutoffLo * width / framerate;
    double fh = 2 * cutoffHi * width / framerate;

    double response;

    // Create the filtermask, looks like the quarter of a circle
    for(int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if(x >= fl && x <= fh)
                response = 1.0f;
            else
                response = 0.0f;

            filter.at<float>(y,x) = response;
        }
    }
}

////////////////////////
///Helper //////////////
////////////////////////
void img2tempMat(const cv::Mat &frame, cv::Mat &dst, int maxImages)
{
    // Reshape in 1 column
	cv::Mat reshaped = frame.reshape(frame.channels(), frame.cols*frame.rows).clone();

   
    reshaped.convertTo(reshaped, CV_32FC3);

    // First frame
    if(dst.cols == 0) {
        reshaped.copyTo(dst);
    }
    // Later frames
    else {
        hconcat(dst,reshaped,dst);
    }

    // If dst reaches maximum, delete the first column (eg the oldest image)
    if(dst.cols > maxImages && maxImages != 0) {
        dst.colRange(1,dst.cols).copyTo(dst);
    }
}

void tempMat2img(const cv::Mat &src, int position, const cv::Size &frameSize, cv::Mat &frame)
{
	cv::Mat line = src.col(position).clone();
    frame = line.reshape(line.channels(), frameSize.height).clone();
}