#pragma once
#include "DetectEdge.h"
#include <opencv2\opencv.hpp>
class LSDDetectEdge :
	public DetectEdge
{
public:
	LSDDetectEdge();
	virtual ~LSDDetectEdge();
	virtual void detect(cv::Mat& image, vector<cv::Vec4i>& edges);
	virtual bool detect(cv::Mat& image, cv::Mat& dst);
	void LSDLines(cv::Mat& gary, vector<vector<cv::Vec4i>>& lines);
	void mergeLines(vector<vector<cv::Vec4i>>& lines, vector<cv::Vec4i>& edges);
private:
	cv::Ptr<cv::LineSegmentDetector> detector_;
};

