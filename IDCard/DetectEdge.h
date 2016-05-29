#pragma once
#include<opencv2\opencv.hpp>
#include<vector>
using namespace std;
class DetectEdge
{
public:
	DetectEdge();
	virtual ~DetectEdge();
	virtual void detect(cv::Mat& image, vector<cv::Vec4i>& edges) = 0;
	virtual bool detect(cv::Mat& image, cv::Mat& dst) = 0;
};

