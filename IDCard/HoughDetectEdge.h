#pragma once
#include "DetectEdge.h"
class HoughDetectEdge :
	public DetectEdge
{
public:
	HoughDetectEdge();
	virtual ~HoughDetectEdge();
	virtual void detect(cv::Mat& image, vector<cv::Vec4i>& edges);
	virtual bool detect(cv::Mat& image, cv::Mat& dst) = 0;
private:
	void canndyImage(cv::Mat& gray);
	void houghLines(cv::Mat& gary, vector<vector<cv::Vec4i>>& lines);
	void mergeLines(vector<vector<cv::Vec4i>>& lines, vector<cv::Vec4i>& edges);
};

