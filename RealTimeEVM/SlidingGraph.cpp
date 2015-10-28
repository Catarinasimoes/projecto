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

#include "SlidingGraph.hpp"

SlidingGraph::SlidingGraph(int w, int h, int yl, int yh, int maxPoints) : width(w), height(h), y_low(yl), y_high(yh), maxNumOfPoints(maxPoints) {
	graph = cv::Mat(height, width, CV_8UC3);
}


void SlidingGraph::addPoint(int p){
	//shift points in graph
	points.push_back(p);
	if (points.size() > maxNumOfPoints) {
		points.erase(points.begin());
	}

	//plot graph in an opencv Mat
	graph = cv::Scalar(255, 255, 255);	//fill with white
	for (int i = 0; i < points.size(); i++) {
		int point = points[i];
		if (point > y_low && point < y_high) {
			int ypos = height - ((point * height ) / (y_high - y_low));
			int xpos = (i * width) / maxNumOfPoints;
			cv::circle(graph, cv::Point(xpos, ypos), 2, cv::Scalar(0, 0, 255));
		}
	}
}


void SlidingGraph::getGraph(cv::Mat &dst) {
	dst = graph.clone();
}