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
#define NOMINMAX
#include "Magnificator.h"
#include <algorithm>
#include <Windows.h>

Magnificator::Magnificator(Buffer<cv::Mat> &buffer, Buffer<cv::Mat> &resultingBuffer, Buffer<cv::Rect> &face_area, EvmSettings &settings) : processingBuffer(buffer), magnifiedBuffer(resultingBuffer), faceArea(face_area), evmSettings(settings), currentFrame(0) {
}


void Magnificator::colorMagnify() {
	if (evmSettings.getmode() == 2)
		return;
	//check if settings have changed and if so clear buffers
	if (evmSettings.haveChanged()) {
		//clear buffers here

		downSampledMat = cv::Mat();
	}

	//Load settings here (this prevents setting from changing during the magnification):
	int alpha = evmSettings.getAlpha();
	unsigned int framerate = evmSettings.getFramerate();
	unsigned int levels = evmSettings.getLevels();
	double fl = evmSettings.getFl();
	double fh = evmSettings.getFh();


	// Magnify only when processing buffer holds new images
	int buffersize = processingBuffer.size();
	//cout << "frames to magnify" << buffersize << endl;
	if (buffersize == 0)
		return;


	cv::Mat filteredFrame, downSampledFrame, filteredMat;
	std::vector<cv::Mat> inputFrames, inputROIs, inputPyramid;
	std::vector<cv::Rect> inputFaceArea;

	cv::Rect ROIrect;
	// Process every frame in buffer that wasn't magnified yet
	while (processingBuffer.size() > 0 && inputFrames.size() < getOptimalBufferSize(framerate)) {
		// Grab oldest frame from processingBuffer and delete it to save memory
		cv::Mat input = processingBuffer.dequeue();

		//Grab corresponding set of points
		ROIrect = faceArea.dequeue();

		// Save input frame and points to add motion later
		inputFrames.push_back(input);
		inputFaceArea.push_back(ROIrect);

		cv::Mat ROI = input(ROIrect);
		ROI.convertTo(ROI, CV_32FC3);	// VERY SLOW
		inputROIs.push_back(ROI);

		/* 1. SPATIAL FILTER, BUILD GAUSS PYRAMID */
		buildGaussPyrFromImg(ROI, levels, inputPyramid);

		/* 2. CONCAT EVERY SMALLEST FRAME FROM PYRAMID IN ONE LARGE MAT, 1COL = 1FRAME */
		downSampledFrame = inputPyramid.at(levels - 1);
		img2tempMat(downSampledFrame, downSampledMat, getOptimalBufferSize(framerate));
	}


	/* 3. TEMPORAL FILTER */
	idealFilter(downSampledMat, filteredMat, fl, fh, framerate);

	/* 4. AMPLIFY */
	amplifyGaussian(filteredMat, filteredMat, alpha);

	// Add amplified image (color) to every frame
	int initialInputFramesSize = inputFrames.size();
	for (int i = 0; i < initialInputFramesSize; i++) {
		/* 5. DE-CONCAT 1COL TO DOWNSAMPLED COLOR IMAGE */
		tempMat2img(filteredMat, i, downSampledFrame.size(), filteredFrame);

		/* 6. RECONSTRUCT COLOR IMAGE FROM PYRAMID */
		cv::Mat color;
		buildImgFromGaussPyr(filteredFrame, levels, color, inputROIs.front().size());

		/* 7. ADD COLOR IMAGE TO ORIGINAL IMAGE */
		color = color + inputROIs.front();
		inputROIs.erase(inputROIs.begin());

		//Normalize color range
		double min, max;
		minMaxLoc(color, &min, &max);
		color.convertTo(color, CV_8UC3, 255.0 / (max - min), -min * 255.0 / (max - min));	//VERY SLOW

		//Stich ROI in original frame
		cv::Mat output = inputFrames.front();
		inputFrames.erase(inputFrames.begin());
		color.copyTo(output(inputFaceArea.front()));

		//Draw rectangle around face area
		cv::rectangle(output, inputFaceArea.front(), cv::Scalar(255, 0, 0));
		inputFaceArea.erase(inputFaceArea.begin());

		// Fill internal buffer with magnified imagefl
		magnifiedBuffer.enqueue(output);


	}
}


void Magnificator::motionMagnify(){
	if (evmSettings.getmode() == 1 || evmSettings.getmode() == 3)
		return;
	if (evmSettings.haveChanged()) {
		//clear buffers here
		cv::Mat inputframes;
		cv::Mat motion;
		inputframes.size == 0;

	}
	int alpha = evmSettings.getAlpha();
	unsigned int framerate = evmSettings.getFramerate();
	unsigned int levels = evmSettings.getLevels();
	double colow = evmSettings.getcolow();
	double cohigh = evmSettings.getcohigh();
	//cout << alpha << " " << " " << framerate << " " << levels << " " << colow << " " << cohigh << endl;
	int buffersize = processingBuffer.size();
	//cout << "frames to magnify" << buffersize << endl;
	if (buffersize == 0)
		return;

	cv::Mat input, motion;
	std::vector<cv::Mat> inputPyramid, inputFrames, inputROIs;
	std::vector<cv::Rect> inputFaceArea;
	cv::Rect ROIrect;

	//cout << "frames to magnify" << buffersize << endl;


	while (processingBuffer.size() > 0 && inputFrames.size() < getOptimalBufferSize(framerate)) {
		//cout << "optimal buffersize is " << getOptimalBufferSize(framerate) << endl;
		if (evmSettings.haveChanged()){
			int alpha = evmSettings.getAlpha();
			unsigned int framerate = evmSettings.getFramerate();
			unsigned int levels = evmSettings.getLevels();
			double colow = evmSettings.getcolow();
			double cohigh = evmSettings.getcohigh();
		}
		//cout << alpha << " " << " " << framerate << " " << levels << " " << colow << " " << cohigh << endl;
		// Grab oldest frame from processingBuffer and delete it to save memory
		cv::Mat input = processingBuffer.dequeue();

		ROIrect = faceArea.dequeue();
		cv::Mat ROI = input(ROIrect);
		// Save input frame and points to add motion later
		inputFrames.push_back(input);// adicionar nova matriz input a parte de tras
		inputFaceArea.push_back(ROIrect);

		//ROI.convertTo(input, CV_32FC3, 1.0 / 255.0f);
		ROI.convertTo(ROI, CV_32FC3, 1.0 / 255.0f);
		cvtColor(ROI, ROI, CV_BGR2YCrCb);
		//BGR2YCbCr(ROI, ROI);
		inputROIs.push_back(ROI);
		buildLaplacePyrFromImg(ROI, levels, inputPyramid);
		//processar movimento
		if (inputFrames.size() < getOptimalBufferSize(framerate)) {


			lowpassHi = inputPyramid;
			lowpassLo = inputPyramid;
			motionPyramid = inputPyramid;

		}
		else {

			/* 2. TEMPORAL FILTER EVERY LEVEL OF LAPLACE PYRAMID */
			for (int curLevel = 0; curLevel < levels; ++curLevel) {
				iirFilter(inputPyramid.at(curLevel), motionPyramid.at(curLevel), lowpassHi.at(curLevel), lowpassLo.at(curLevel),
					evmSettings.getcolow(), evmSettings.getcohigh());
			}
		}

		int w = ROI.size().width;
		int h = ROI.size().height;
		

		// Amplification variable//8.0
		float delta = evmSettings.getcowavelength() / (8.0 * (1.0 + alpha));
		//std::cout << "Delta is " << delta << endl;
		int exaggeration_factor = 10;

		float lambda = sqrt(w*w + h*h)/ 3.0;
		//std::cout << "Lambda is " << lambda << endl;
		//std::cout << delta;
		for (int curLevel = levels; curLevel >= 0; --curLevel) {
			amplifyLaplacian(motionPyramid.at(curLevel), motionPyramid.at(curLevel), curLevel, lambda, delta, 10, levels, alpha);
			//cout << "iam now amplifying level  " << curLevel << endl;
			lambda /= 2.0;
		}
	}


	buildImgFromLaplacePyr(motionPyramid, levels, motion);


	/* 5. ATTENUATE (if not grayscale) */
	attenuate(motion, motion, 1);

	if (buffersize > 0)
	{
		//cout << "buffersize is now " << buffersize << endl;
		motion = inputROIs.front() + motion;
	}

	else
	{
		//cout << "buffersize is now " << buffersize << endl;
	motion = inputROIs.front();
}
	cvtColor(motion, motion, CV_YCrCb2BGR);
	//YCbCr2BGR(motion, motion);
	double minimo, maximo;
	minMaxLoc(motion, &minimo, &maximo);
	motion.convertTo(motion, CV_8UC3, 255.0 / (maximo - minimo), -minimo * 255.0 / (maximo - minimo));	//VERY SLOW
	
	cv::Mat output = inputFrames.front();


	motion.copyTo(output(inputFaceArea.front()));

	cv::rectangle(output, inputFaceArea.front(), cv::Scalar(255, 0, 0));
	
	magnifiedBuffer.enqueue(output);
	
}

void Magnificator::waveletMagnify() {
	if (evmSettings.getmode() == 2 || evmSettings.getmode() == 1)
		return;
	int alpha = evmSettings.getAlpha();
	unsigned int framerate = evmSettings.getFramerate();
	unsigned int levels = evmSettings.getLevels();
	double fl = evmSettings.getFl();
	double fh = evmSettings.getFh();

	// Magnify only when processing buffer holds new images
	int buffersize = processingBuffer.size();
	//cout << "frames to magnify" << buffersize << endl;
	if (buffersize == 0)
		return;


	cv::Mat input, output, motion, ROI;
	vector< vector<cv::Mat> > inputPyramid;
	vector <cv::Mat> inputFrames, inputROIs;
	vector<cv::Rect> inputFaceArea;
	cv::Rect ROIrect;
	int pChannels;


	while (processingBuffer.size() > 0 && inputFrames.size() < getOptimalBufferSize(framerate)){

		//tirar uma frame do buffer
		input = processingBuffer.dequeue();
		if (processingBuffer.size() > 0)
			//Se o buffer não estiver vazio
		{
			input = processingBuffer.dequeue();
		}

		//tirar um rectangulo do buffer
		ROIrect = faceArea.dequeue();
		//Fazer a ROI
		cv::Mat ROI = input(ROIrect);
		//Guardar na fila para depois adicionar o movimento a frame e o rectangulo
		inputFrames.push_back(input);
		inputFaceArea.push_back(ROIrect);

		//Matriz de 3 de "profundidade"
		//cv::Mat inputChannels[1];
		//Converter a ROI para 32 bits
		//input.convertTo(input, CV_32FC3, 1.0 / 255.0f);
		cvtColor(ROI, ROI, CV_RGB2GRAY);
		ROI.convertTo(ROI, CV_32FC1, 1.0 / 255.0f);

		//Converter para YIC
		//cvtColor(ROI, ROI, CV_BGR2YCrCb);
		//Dividir os canais apenas na ROI
		//split(ROI, inputChannels);
		//Acumular a ROI na fila
		inputROIs.push_back(ROI);
		//Piramide e por no primeiro canal da ROI

		// buildWaveletPyrFromImg(inputChannels[0].clone(), levels, inputPyramid);
		buildWaveletPyrFromImg((inputROIs.front()).clone(), levels, inputPyramid);
		//Se depois do dequeue o buffer estiver vazio
		//if (processingBuffer.size()== 0)
		if (inputFrames.size() < getOptimalBufferSize(framerate)) {
			wlLowpassHi = inputPyramid;
			wlLowpassLo = inputPyramid;
			wlMotionPyramid = inputPyramid;
		}
		else {
			/* 2. TEMPORAL FILTER EVERY LEVEL OF LAPLACE PYRAMID */
			for (int curLevel = 0; curLevel < levels; ++curLevel) {
				iirWaveletFilter(inputPyramid.at(curLevel), wlMotionPyramid.at(curLevel), wlLowpassHi.at(curLevel), wlLowpassLo.at(curLevel),
					evmSettings.getcolow(), evmSettings.getcohigh());

			}

			//Constantes com a altura das ROIS
			int w = ROI.size().width;
			int h = ROI.size().height;
			//variavel de amplificaçao
			double delta = evmSettings.getcowavelength() / (8.0 * (1.0 + evmSettings.getAlpha()));
			// Amplification Booster for better visualization
			int exaggeration_factor = 2;
			double lambda = sqrt(w*w + h*h) / 3.0;
			// While reconstructing

			for (int curLevel = levels - 1; curLevel >= 0; --curLevel) {
				amplifyWavelet(wlMotionPyramid.at(curLevel), wlMotionPyramid.at(curLevel), curLevel, lambda, delta, exaggeration_factor);
				lambda /= 2.0;
			}
		}
		buildImgFromWaveletPyr(wlMotionPyramid, motion, ROI.size());
		//GaussianBlur(motion, motion, cv::Size(0, 0),1.25 );
		GaussianBlur(motion, motion, cv::Size(0, 0), 1.25);
		// Merge array of motions back into one image
		/*for (int chn = 0; chn < 1; ++chn) {
			inputChannels[chn] = motion;
			}
			merge(inputChannels, 1, motion);*/
		//attenuate(motion, motion, 0.1);
		if (processingBuffer.size() > 0)
			output = inputROIs.front() + motion;
		else
			output = inputROIs.front();

		//inputROIs.erase(inputROIs.begin());
		//cvtColor(output, output, CV_YCrCb2BGR);
		output.convertTo(output, CV_8UC1, 255.0, 1.0 / 255.0);

		motion.copyTo(output(inputFaceArea.front()));
		magnifiedBuffer.enqueue(output);
		//inputFaceArea.erase(inputFaceArea.begin());

	}

}









int Magnificator::getOptimalBufferSize(int fps)
{
	// Calculate number of images needed to represent 2 seconds of film material
	unsigned int round = (unsigned int)max(2 * fps, 16);
	// Round to nearest higher power of 2
	round--;
	round |= round >> 1;
	round |= round >> 2;
	round |= round >> 4;
	round |= round >> 8;
	round |= round >> 16;
	round++;

	return round;
}


void Magnificator::amplifyGaussian(const cv::Mat &src, cv::Mat &dst, int alpha)
{
	dst = src * alpha;
}


void Magnificator::amplifyLaplacian(const cv::Mat &src, cv::Mat &dst, int currlevel, float lambda, float delta, int exageration_factor, int levels, int alpha)
{

	float currAlpha = (lambda / (delta*8.0) - 1.0) * exageration_factor;
	//cout << currAlpha << endl;
	// Set lowpassed&downsampled image and difference image with highest resolution to 0,
	// amplify every other level
	dst = (currlevel == levels || currlevel == 0) ? src * 0
		: src * std::min((float)alpha, currAlpha);

}


void Magnificator::attenuate(const cv::Mat &src, cv::Mat &dst, double chromAttenuation){
	{
		// Attenuate only if image is not grayscale
		if (src.channels() > 2)
		{
			cv::Mat planes[3];
			split(src, planes);
			planes[1] = planes[1] * chromAttenuation;
			planes[2] = planes[2] * chromAttenuation;
			merge(planes, 3, dst);
		}
	}

}



void Magnificator::amplifyWavelet(const vector<cv::Mat> &src, vector<cv::Mat> &dst, int currentLevel, float lambda, float delta, int exxageration_factor)
{
	float currAlpha = (lambda / (delta*8.0) - 1.0) * exxageration_factor;
	// Set reference image (lowest resolution) to 0, amplify every other channel
	for (int dims = 0; dims < 3; ++dims) {
		dst.at(dims) = src.at(dims) * std::min((float)evmSettings.getAlpha(), currAlpha);
	}
	if (currentLevel == evmSettings.getLevels() - 1)
		dst.at(3) = src.at(3) * 0;
}

void Magnificator::denoiseWavelet(vector< vector<cv::Mat> > &src, vector< vector<cv::Mat> > &dst, float threshold)
{

}

void Magnificator::YCbCr2BGR(const cv::Mat &src, cv::Mat &dst)
{
	cv::Mat image = cv::Mat::zeros(src.size(), src.type());

	const uchar* uc_pixel = src.data;
	uchar* c_pixel = image.data;
	for (int row = 0; row < src.rows; ++row)
	{
		uc_pixel = src.data + row*src.step;
		c_pixel = image.data + row*src.step;
		for (int col = 0; col < src.cols; ++col)
		{
			int Y = uc_pixel[0];
			int Cb = uc_pixel[1];
			int Cr = uc_pixel[2];

			c_pixel[0] = Y + 1.4*(Cr - 128);
			c_pixel[1] = Y - 0.343*(Cb - 128) - 0.711*(Cr - 128);
			c_pixel[2] = Y + 1.765*(Cb - 128);

			uc_pixel += 3;
			c_pixel += 3;
		}
	}

	dst = image;
}

void Magnificator::BGR2YCbCr(const cv::Mat &src, cv::Mat &dst)
{
	cv::Mat image = cv::Mat::zeros(src.size(), src.type());

	const uchar* uc_pixel = src.data;
	uchar* c_pixel = image.data;
	for (int row = 0; row < src.rows; ++row)
	{
		uc_pixel = src.data + row*src.step;
		c_pixel = image.data + row*src.step;
		for (int col = 0; col < src.cols; ++col)
		{
			int R = uc_pixel[0];
			int G = uc_pixel[1];
			int B = uc_pixel[2];

			c_pixel[0] = 0 + 0.299*R + 0.587*G + 0.114*B;
			c_pixel[1] = 128 - 0.169*R - 0.331*G + 0.5*B;
			c_pixel[2] = 128 + 0.5*R - 0.419*G - 0.081*B;

			uc_pixel += 3;
			c_pixel += 3;
		}
	}

	dst = image;
}