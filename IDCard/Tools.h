#pragma once
#include <opencv2\opencv.hpp>
#include <vector>
using namespace std;
class Tools
{
public:
	Tools();
	~Tools();
	static void MergeLines(vector<cv::Vec4i>& lines, cv::Vec4i& edge,int type);
	static cv::Point CrossPoint(const cv::Vec4i& line1, const cv::Vec4i& line2);
private:
	static void MergeHorizontalLines(vector<cv::Vec4i>& lines, cv::Vec4i& edge);
	static void MergeVerticalLines(vector<cv::Vec4i>& lines, cv::Vec4i& edge);
};

