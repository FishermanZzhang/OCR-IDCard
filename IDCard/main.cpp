#include <iostream>
#include <vector>
#include <opencv2\opencv.hpp>
#include "DetectEdge.h"
#include "HoughDetectEdge.h"
#include "LSDDetectEdge.h"
#include "Tools.h"

using namespace std;

int main(int argc, char * argv[])
{
	cv::Mat color = cv::imread("../Input/13.jpg");
	//HoughDetectEdge de;
	LSDDetectEdge de;

	vector<cv::Vec4i> edges;
	cv::Mat dst;
	de.detect(color, dst);
	cv::imshow("img", dst);

	cv::waitKey();




}