#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2\opencv.hpp>
#include "DetectEdge.h"
#include "HoughDetectEdge.h"
#include "LSDDetectEdge.h"
#include "Tools.h"

using namespace std;

int main(int argc, char * argv[])
{
	//cv::Mat color = cv::imread("../template/15.jpg");
	//HoughDetectEdge de;
	LSDDetectEdge de;

	
	vector<cv::Vec4i> edges;
	ifstream input("./train.txt");
	string imagename;
	while (input >> imagename)
	{
		cv::Mat dst;
		cv::Mat color = cv::imread("../Train/"+imagename);
		//cv::imshow("color", color);
		de.detect(color, dst);
		
		cv::Rect name(105, 45, 150, 45);
		cv::Rect nums(200, 320, 400, 56);
		cv::rectangle(dst, name, cv::Scalar(0, 0, 255));
		cv::rectangle(dst, nums, cv::Scalar(255, 0, 0));
		cv::imshow("img", dst);
		cv::waitKey();
		cv::destroyAllWindows();
	}


	cv::waitKey();




}