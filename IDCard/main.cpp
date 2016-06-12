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
	//to_string(10);
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
			//cv::resize(it, st, cv::Size(1.0 * 32 * it.cols / it.rows, 32), 0, 0, cv::INTER_CUBIC);
			cv::cvtColor(it, st, CV_BGR2GRAY);
			cv::threshold(st, st, 155, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
			cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 7));
			//cv::dilate(st, st, element);
			cv::morphologyEx(st, st, cv::MORPH_DILATE, element);

			vector<vector<cv::Point>> points;
			cv::findContours(st, points, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
			vector<cv::Rect> regions;
			int blockheight = 0;
			for (auto ps : points){
				cv::Rect region;
				int minx = INT_MAX;
				int maxx = 0;
				int miny = INT_MAX;
				int maxy = 0;
				for (auto p : ps){
					minx = std::min(minx, p.x);
					maxx = std::max(maxx, p.x);
					miny = std::min(miny, p.y);
					maxy = std::max(maxy, p.y);
				}
				region.x = minx;
				region.y = miny;
				region.width = maxx - minx;
				region.height = maxy - miny;
				blockheight = std::max(blockheight, maxy - miny);
				if (region.size().area() <= 40 || region.width < 5){
					continue;
				}
				regions.emplace_back(region);
				//cv::imshow("block", it(region));
				//cv::waitKey();
			}
			regions.clear();
			sort(regions.begin(), regions.end(), [](cv::Rect r1, cv::Rect r2){
				return r1.x < r2.x;
			});
			vector<cv::Rect> tmpregions;
			tmpregions.swap(regions);
			for (int i = 0; i < tmpregions.size(); ++i){
				if (tmpregions[i].width > 1.7 * blockheight){
					//regions.emplace_back(tmpregions[i]);
					regions.emplace_back(cv::Rect(tmpregions[i].x, tmpregions[i].y, tmpregions[i].width / 2, tmpregions[i].height));
					regions.emplace_back(cv::Rect(tmpregions[i].x + tmpregions[i].width / 2, tmpregions[i].y, tmpregions[i].width - tmpregions[i].width / 2, tmpregions[i].height));
				}
				else if (tmpregions[i].width > 0.75* blockheight){
					regions.emplace_back(tmpregions[i]);
				}
				else{
					int j = i + 1;
					cv::Rect mergeRect = tmpregions[i];
					int width = tmpregions[i].width;
					for (; j < tmpregions.size(); ++j){
						width = tmpregions[j].x + tmpregions[j].width - tmpregions[i].x;
						if (width > 1.1 * blockheight) break;
						mergeRect |= tmpregions[j];

					}
					regions.emplace_back(mergeRect);
					i = j - 1;
				}
			}
			for (auto r : regions){
				cout << r.width << " " << r.height;
				cv::imshow("block", it(r));
				cv::waitKey();
			}
			cv::imshow("it", st);
			cv::waitKey();
			break;
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