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

#ifndef SLIDINGGRAPH_HPP
#define SLIDINGGRAPH_HPP

#include "opencv2\opencv.hpp"
#include <vector>

class SlidingGraph {
public:
	/**
	Constructor
	@param width of the resulting Mat
	@param height of the resulting Mat
	@param lowest value in the range of values to show
	@param highest value in the range of values to show
	@param maximum number of points to plot

	*/
	SlidingGraph(int w, int h, int yl, int yh, int maxPoints);

	/**
	Adds new point to plot
	@param value of the point to plot.
	*/
	void addPoint(int p);

	/**
	Draws graph and stores it into the matrix passed as parameter
	@param cv::Mat in which the graph will be stored
	*/
	void getGraph(cv::Mat &dst);
private:
	//Maximum number of points to plot
	int maxNumOfPoints;

	//Points to plot
	std::vector<int> points;

	//Mat dimensions
	int width, height;

	//Range of values
	int y_low, y_high;

	//Resulting graph
	cv::Mat graph;
};

#endif