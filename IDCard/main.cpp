#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2\opencv.hpp>
#include "DetectEdge.h"
#include "HoughDetectEdge.h"
#include "LSDDetectEdge.h"

using namespace std;

int main(int argc, char * argv[])
{
	//cv::Mat color = cv::imread("../template/15.jpg");
	//HoughDetectEdge de;
	LSDDetectEdge de;

	
	vector<cv::Vec4i> edges;
	ifstream input("./idimages.txt");
	string imagename;
	while (input >> imagename)
	{
		cv::Mat color = cv::imread("../Train/"+imagename);
		//cv::imshow("color", color);
		vector<cv::Mat> items;
		de.detect(color, items);
		for (auto & it : items)
		{
			cv::Mat st;
			cv::resize(it, st, cv::Size(1.0 * 32 * it.cols / it.rows, 32), 0, 0, cv::INTER_CUBIC);
			cv::cvtColor(st, st, CV_BGR2GRAY);
			cv::threshold(st, st, 155, 255, CV_THRESH_OTSU | CV_THRESH_BINARY);

			cv::imshow("it", st);
			cv::waitKey();
		}
		
		//cv::Rect nameRect(105, 42, 150, 55);
		//cv::Rect numsRect(200, 320, 410, 56);
		////cv::rectangle(dst, nameRect, cv::Scalar(0, 0, 255));
		////cv::rectangle(dst, numsRect, cv::Scalar(255, 0, 0));
		////cv::Mat nameMat = dst(nameRect).clone();
		////cv::Mat numsMat = dst(numsRect).clone();
		////cv::Mat namegray,namebin;
		////cv::Mat numsgray,numsbin;
		////cv::cvtColor(nameMat, namegray, CV_BGR2GRAY);
		////cv::cvtColor(numsMat, numsgray, CV_BGR2GRAY);
		////cv::threshold(namegray, namebin, 150, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
		////cv::threshold(numsgray, numsbin, 150, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

		//auto trim = [](const cv::Mat& block){
		//	cv::Mat gray;
		//	if (block.channels() == 3){
		//		cv::cvtColor(block, gray, CV_BGR2GRAY);
		//	}
		//	else{
		//		gray = block;
		//	}
		//	pair<int, int> height;
		//	cv::Mat bin;
		//	cv::threshold(gray, bin, 150, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

		//	vector<int> feature(gray.rows);
		//	for (int i = 0; i < gray.rows; ++i){
		//		cv::Mat& tmpmat = bin.row(i);
		//		cv::Scalar s = cv::sum(tmpmat);
		//		feature[i] = s[0];
		//	}
		//	int half = gray.rows / 2;
		//	if (feature[half] == 0){
		//		half = gray.rows / 3;
		//	}
		//	if (feature[half] == 0){
		//		half = gray.rows * 2 / 3;
		//	}
		//	if (feature[half] == 0){
		//		half = gray.rows / 4;
		//	}
		//	if (feature[half] == 0){
		//		half = gray.rows * 3 / 4;
		//	}
		//	int y1 = half;
		//	int y2 = half;
		//	while (y1 >= 0 && feature[y1] != 0){
		//		--y1;
		//	}
		//	while (y2 < gray.rows && feature[y2] != 0){
		//		++y2;
		//	}
		//	height.first = std::max(0, y1 - 2);
		//	height.second = std::min(gray.rows, y2 + 2);
		//	
		//	return height;
		//};
		//pair<int, int> heightname;
		//pair<int, int> heightnums;
		//cv::Mat& nametmp = dst(nameRect);
		//cv::Mat& numstmp = dst(numsRect);

		//heightname = trim(nametmp);
		//heightnums = trim(numstmp);
		//cv::Mat nameMat = nametmp.rowRange(heightname.first, heightname.second).clone();
		//cv::Mat numsMat = numstmp.rowRange(heightnums.first, heightnums.second).clone();


		//cv::Rect rnameRect(105 + heightname.first, 42, 150, heightname.second - heightname.first);
		//cv::Rect rnumsRect(200 + heightnums.first, 320, 410,heightnums.second - heightnums.first);

		//cv::Mat nameMat = color(name_nums.first).clone();
		//cv::Mat numsMat = color(name_nums.second).clone();
		////cv::imshow("img", dst);
		//cv::imshow("name", nameMat);
		//cv::imshow("nums", numsMat);
		//cv::waitKey();
		cv::destroyAllWindows();
	}


	cv::waitKey();




}