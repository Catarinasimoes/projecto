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

#include <Kinect.h>
#include <sys\timeb.h>

#include "CaptureThreadIR.h"
#include "Buffer.hpp"
#include "Sampling.hpp"



CaptureThreadIR::CaptureThreadIR(Buffer<cv::Mat> &buffer, Buffer<cv::Rect> &face_area, Buffer<vector<cv::Point>> &sampling_points, EvmSettings &settings) : framesBuffer(buffer), faceArea(face_area), samplingPoints(sampling_points), evmSettings(settings)  {
}


bool CaptureThreadIR::connectToKinect() {
	// Sensor
	HRESULT hResult = S_OK;
	hResult = GetDefaultKinectSensor(&pSensor);
	if (FAILED(hResult)){
		std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
		return false;
	}

	hResult = pSensor->Open();
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::Open()" << std::endl;
		return false;
	}

	// Source
	hResult = pSensor->get_InfraredFrameSource(&pInfraredSource);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::get_InfraredFrameSource()" << std::endl;
		return false;
	}
	
	hResult = pSensor->get_BodyFrameSource(&pBodySource);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinect Sensor::get_BodyFrameSource()" << std::endl;
		return false;
	}

	// Reader
	hResult = pInfraredSource->OpenReader(&pInfraredReader);
	if (FAILED(hResult)){
		std::cerr << "Error : IInfraredFrameSource::OpenReader()" << std::endl;
		return false;
	}

	hResult = pBodySource->OpenReader(&pBodyReader);
	if (FAILED(hResult)){
		std::cerr << "Error : IBodyFrameSource::OpenReader()" << std::endl;
		return false;
	}
	
	// Description
	hResult = pInfraredSource->get_FrameDescription(&pDescription);
	if (FAILED(hResult)){
		std::cerr << "Error : IInfraredFrameSource::get_FrameDescription()" << std::endl;
		return false;
	}

	pDescription->get_Width(&kinect_infrared_width);
	pDescription->get_Height(&kinect_infrared_height);

	bufferSize = kinect_infrared_height * kinect_infrared_width * 4 * sizeof(unsigned char);

	// Coordinate Mapper
	hResult = pSensor->get_CoordinateMapper(&pCoordinateMapper);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::get_CoordinateMapper()" << std::endl;
		return false;
	}

	deformations = vector<vector<float>>(BODY_COUNT, vector<float>(FaceShapeDeformations::FaceShapeDeformations_Count));

	for (int count = 0; count < BODY_COUNT; count++){
		// Source
		hResult = CreateHighDefinitionFaceFrameSource(pSensor, &pHDFaceSource[count]);
		if (FAILED(hResult)){
			std::cerr << "Error : CreateHighDefinitionFaceFrameSource()" << std::endl;
			return false;
		}

		// Reader
		hResult = pHDFaceSource[count]->OpenReader(&pHDFaceReader[count]);
		if (FAILED(hResult)){
			std::cerr << "Error : IHighDefinitionFaceFrameSource::OpenReader()" << std::endl;
			return false;
		}

		// Open Face Model Builder
		hResult = pHDFaceSource[count]->OpenModelBuilder(FaceModelBuilderAttributes::FaceModelBuilderAttributes_None, &pFaceModelBuilder[count]);
		if (FAILED(hResult)){
			std::cerr << "Error : IHighDefinitionFaceFrameSource::OpenModelBuilder()" << std::endl;
			return false;
		}

		// Start Collection Face Data
		hResult = pFaceModelBuilder[count]->BeginFaceDataCollection();
		if (FAILED(hResult)){
			std::cerr << "Error : IFaceModelBuilder::BeginFaceDataCollection()" << std::endl;
			return false;
		}

		// Create Face Alignment
		hResult = CreateFaceAlignment(&pFaceAlignment[count]);
		if (FAILED(hResult)){
			std::cerr << "Error : CreateFaceAlignment()" << std::endl;
			return false;
		}

		// Create Face Model
		hResult = CreateFaceModel(1.0f, FaceShapeDeformations::FaceShapeDeformations_Count, &deformations[count][0], &pFaceModel[count]);
		if (FAILED(hResult)){
			std::cerr << "Error : CreateFaceModel()" << std::endl;
			return false;
		}
	}

	//Get number of vertexes in face model
	hResult = GetFaceModelVertexCount(&vertex); // 1347
	if (FAILED(hResult)){
		std::cerr << "Error : GetFaceModelVertexCount()" << std::endl;
		return false;
	}


	return true;
}


void CaptureThreadIR::captureCamera() {
	IInfraredFrame* pInfraredFrame;
	HRESULT hResult;
	bool infraredFrameAdded = false;
	//keep getting last frame until told to stop
	while (isRunning) {
		//time stuff
		struct timeb start;
		ftime(&start);

		//Color Frame
		pInfraredFrame = nullptr;
		hResult = pInfraredReader->AcquireLatestFrame(&pInfraredFrame);
		if (SUCCEEDED(hResult)){
			cv::Mat bufferMat(kinect_infrared_height, kinect_infrared_width, CV_8UC1);

			//hResult = pColorFrame->CopyConvertedFrameDataToArray(bufferSize, reinterpret_cast<BYTE*>(bufferMat.data), ColorImageFormat::ColorImageFormat_Bgra);
			hResult = pInfraredFrame->CopyFrameDataToArray(bufferSize, reinterpret_cast<UINT16*>(bufferMat.data));
			if (SUCCEEDED(hResult)){
				//Remove transparency channel
				//cv::cvtColor(bufferMat, bufferMat, CV_BGRA2BGR);

				//Add to magnifying queue
				framesBuffer.enqueue(bufferMat);
				infraredFrameAdded = true;

			}
			else {
				cerr << "Failed to convert IR frame to Mat!" << endl;
			}
		}
		else {
			cerr << "Failed to aquire IR frame!" << endl;
		}

		SafeRelease(pInfraredFrame);


		//Body stuff for hd face
		IBodyFrame* pBodyFrame = nullptr;
		hResult = pBodyReader->AcquireLatestFrame(&pBodyFrame);
		if (SUCCEEDED(hResult)){
			//inicializar body count a zero
			IBody* pBody[BODY_COUNT] = { 0 };
			hResult = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, pBody);
			if (SUCCEEDED(hResult)){
				for (int count = 0; count < BODY_COUNT; count++){ //percorrer todos os bodys (??)
					BOOLEAN bTrackingIdValid = false;
					hResult = pHDFaceSource[count]->get_IsTrackingIdValid(&bTrackingIdValid);
					if (!bTrackingIdValid){
						BOOLEAN bTracked = false;
						hResult = pBody[count]->get_IsTracked(&bTracked);
						if (SUCCEEDED(hResult) && bTracked){
							// Set TrackingID to Detect Face
							UINT64 trackingId = _UI64_MAX;
							hResult = pBody[count]->get_TrackingId(&trackingId);
							if (SUCCEEDED(hResult)){
								pHDFaceSource[count]->put_TrackingId(trackingId);
							}
						}
					}
				}
			}
			for (int count = 0; count < BODY_COUNT; count++){
				SafeRelease(pBody[count]);
			}
		}
		SafeRelease(pBodyFrame);


		//HD Face Frame
		vector<cv::Point> points;	//to hold this frame face points

		int topLeftx = (kinect_infrared_width * 2) / 5;
		int topLefty = (kinect_infrared_height * 1) / 4;
		int bottomRightx = (kinect_infrared_width * 3) / 5;
		int bottomRighty = (kinect_infrared_height * 3) / 4;



		for (int count = 0; count < 1; count++){	//Change 1 to BODY_COUNT to handle more than one set of points (up to 6 people)
			IHighDefinitionFaceFrame* pHDFaceFrame = nullptr;
			hResult = pHDFaceReader[count]->AcquireLatestFrame(&pHDFaceFrame);
			if (SUCCEEDED(hResult) && pHDFaceFrame != nullptr){
				BOOLEAN bFaceTracked = false;
				hResult = pHDFaceFrame->get_IsFaceTracked(&bFaceTracked);
				if (SUCCEEDED(hResult) && bFaceTracked){
					hResult = pHDFaceFrame->GetAndRefreshFaceAlignmentResult(pFaceAlignment[count]);
					if (SUCCEEDED(hResult) && pFaceAlignment[count] != nullptr){
						// HD Face Points
						std::vector<CameraSpacePoint> facePoints(vertex);
						hResult = pFaceModel[count]->CalculateVerticesForAlignment(pFaceAlignment[count], vertex, &facePoints[0]);
						if (SUCCEEDED(hResult)){

							//Convert each sampling point to opencv coordinates and pass it to the preview thread ALTEREI
							for (int p = 0; p < (sizeof(sampling::samplingPoints) / sizeof(int)); p++){
								ColorSpacePoint colorSpacePoint;
								hResult = pCoordinateMapper->MapCameraPointToColorSpace(facePoints[sampling::samplingPoints[p]], &colorSpacePoint);
								if (SUCCEEDED(hResult)){
									int x = static_cast<int>(colorSpacePoint.X);
									int y = static_cast<int>(colorSpacePoint.Y);
									if ((x >= 0) && (x < kinect_infrared_width) && (y >= 0) && (y < kinect_infrared_height)){//Será preciso mudar?
										//Add this point's coordinates to array
										points.push_back(cv::Point(x, y));

									}
								}
							}

							//Use this to get a moving ROI around the face, ROI bounds need to be calculated
							ColorSpacePoint colorSpacePoint;
							hResult = pCoordinateMapper->MapCameraPointToColorSpace(facePoints[36], &colorSpacePoint);
							if (SUCCEEDED(hResult)) {
							topLeftx = static_cast<int>(colorSpacePoint.X);
							}

							hResult = pCoordinateMapper->MapCameraPointToColorSpace(facePoints[29], &colorSpacePoint);
							if (SUCCEEDED(hResult)) {
							topLefty = static_cast<int>(colorSpacePoint.Y);
							}

							hResult = pCoordinateMapper->MapCameraPointToColorSpace(facePoints[0], &colorSpacePoint);
							if (SUCCEEDED(hResult)) {
							bottomRighty = static_cast<int>(colorSpacePoint.Y);
							}

							hResult = pCoordinateMapper->MapCameraPointToColorSpace(facePoints[614], &colorSpacePoint);
							if (SUCCEEDED(hResult)) {
							bottomRightx = static_cast<int>(colorSpacePoint.X);
							}
						}
					}
				}
			}
			SafeRelease(pHDFaceFrame);
		}

		if (infraredFrameAdded) {
			samplingPoints.enqueue(points);

			cv::Rect ROI(cv::Point(topLeftx, topLefty), cv::Point(bottomRightx, bottomRighty));
			faceArea.enqueue(ROI);
		}


		infraredFrameAdded = false;

		//regulate fps
		struct timeb end;
		ftime(&end);
		int elapsed_time = (int)(1000.0 * (end.time - start.time)
			+ (end.millitm - start.millitm));
		//cout+ << "Elapsed time: " << elapsed_time << endl;
		int delta_time = 1000.0 / evmSettings.getFramerate() - elapsed_time;
		//cout << "Delta time: " << delta_time << endl;


		if (delta_time > 0) {
			cv::waitKey(delta_time);
		}
		else {
			cv::waitKey(1);
			cout << "Having trouble keeping up with video source at " << evmSettings.getFramerate() << " frames per second!" << endl;
		}
	}
}


bool CaptureThreadIR::startCapturing() {
	isRunning = true;
	//start worker thread
	workerThread = std::thread(&CaptureThreadIR::captureCamera, this);

	return true;
}


bool CaptureThreadIR::stopCapturing(){
	isRunning = false;

	//Wait for thread to finish
	workerThread.join();

	return true;
}



//Free allocated resources
bool CaptureThreadIR::disconnectKinect() {
	SafeRelease(pInfraredSource);
	SafeRelease(pBodySource);
	SafeRelease(pInfraredReader);
	SafeRelease(pBodyReader);
	SafeRelease(pDescription);
	SafeRelease(pCoordinateMapper);

	for (int count = 0; count < BODY_COUNT; count++){
		SafeRelease(pHDFaceSource[count]);
		SafeRelease(pHDFaceReader[count]);
		SafeRelease(pFaceModelBuilder[count]);
		SafeRelease(pFaceAlignment[count]);
		SafeRelease(pFaceModel[count]);
	}

	if (pSensor){
		pSensor->Close();
	}
	SafeRelease(pSensor);

	return true;
}