#include "LSDDetectEdge.h"
#include "Tools.h"


LSDDetectEdge::LSDDetectEdge()
{
	detector_ = cv::createLineSegmentDetector(cv::LSD_REFINE_NONE, 0.8, 0.6, 4.0, 20);
}


LSDDetectEdge::~LSDDetectEdge()
{
}

void LSDDetectEdge::detect(cv::Mat& image, vector<cv::Vec4i>& edges)
{
	edges.clear();
	// edges[0] ��
	// edges[1] ��
	// edges[2] ��
	// edges[3] ��
	cv::Mat gray;
	if (image.channels() == 3){
		cv::cvtColor(image, gray, CV_BGR2GRAY);
	}
	else{
		gray = image.clone();
	}
	vector<vector<cv::Vec4i>> lines;
	LSDLines(gray, lines);
	if (lines.size() != 4) return;
	//merge lines
	mergeLines(lines, edges);

}

bool LSDDetectEdge::detect(cv::Mat& image, cv::Mat& dst)
{
	vector<cv::Vec4i> edges;
	detect(image, edges);

	if (edges.size() != 4)
	{
		cerr << "detection error " << endl;
		return false;
	}
	vector<cv::Point2f> PTransfrom(4);
	vector<cv::Point2f> PTransto(4);
	PTransfrom[0] = Tools::CrossPoint(edges[0], edges[2]); //upleft
	PTransfrom[1] = Tools::CrossPoint(edges[0], edges[3]); //upright
	PTransfrom[2] = Tools::CrossPoint(edges[1], edges[2]); //downleft
	PTransfrom[3] = Tools::CrossPoint(edges[1], edges[3]); //downright
#ifdef _DEBUG
	for (size_t i = 0; i < PTransfrom.size(); ++i)
	{
		cv::circle(image, PTransfrom[i], 1, cv::Scalar(0, 0, 255));
	}
	cv::imshow("point", image);
	cv::waitKey();
#endif //end _DEBUG
	PTransto[0].x = 0;
	PTransto[0].y = 0;
	PTransto[1].x = 760;
	PTransto[1].y = 0;
	PTransto[2].x = 0;
	PTransto[2].y = 480;
	PTransto[3].x = 760;
	PTransto[3].y = 480;
	cv::Mat tf = cv::getPerspectiveTransform(PTransfrom, PTransto);
	dst = cv::Mat::zeros(cv::Size(760, 480), image.type());
	cv::warpPerspective(image, dst, tf, dst.size(), cv::INTER_CUBIC);
#ifdef _DEBUG
	cv::imshow("result", dst);
	cv::waitKey();
#endif //end _DEBUG

	return true;

}

void LSDDetectEdge::LSDLines(cv::Mat& gray, vector<vector<cv::Vec4i>>& lines)
{
	const double THETA = 30.0 / 180;
	const int LINENUM = 8;
	// lines[0] ��
	// lines[1] ��
	// lines[2] ��
	// lines[3] ��
	lines.clear();
	vector<cv::Vec4i> tmplines;
	detector_->detect(gray, tmplines);


	//����ֱ��λ�÷ֳ��ĸ������ֱ��
	vector<cv::Vec4i> ups;
	vector<cv::Vec4i> downs;
	vector<cv::Vec4i> lefts;
	vector<cv::Vec4i> rights;
	for (size_t i = 0; i < tmplines.size(); ++i)
	{
		cv::Vec4i& line = tmplines[i];
		//cv::line(gray, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(255 * (i <= 1), 0, 255 * (i>1)), 1, CV_AA);
		int detaX = abs(line[0] - line[2]);
		int detaY = abs(line[1] - line[3]);
		
		if (detaX > detaY && atan(1.0*detaY / detaX) < THETA)// "ˮƽ����"
		{
			if (std::max(line[1], line[3]) < gray.rows / 3)
			{
				ups.emplace_back(line);
				continue;
			}
			if (std::max(line[1], line[3]) > gray.rows * 2 / 3)
			{
				downs.emplace_back(line);
				continue;
			}
		}
		if (detaX < detaY && atan(1.0*detaX / detaY) < THETA)
		{
			if (std::max(line[0], line[2]) < gray.cols / 4)
			{
				lefts.emplace_back(line);
				continue;
			}
			if (std::max(line[0], line[2]) > gray.cols * 3 / 4)
			{
				rights.emplace_back(line);
				continue;
			}
		}
	}
	////���˶�ֱ��
	//if (tmplines.size() > LINENUM){
	//	sort(tmplines.begin(), tmplines.end(), [](cv::Vec4i&line1, cv::Vec4i& line2){ \
		//		return (pow(abs(line1[0] - line1[2]), 2) + pow(abs(line1[1] - line1[3]), 2)) > \
		//			(pow(abs(line2[0] - line2[2]), 2) + pow(abs(line2[1] - line2[3]), 2));});
	//	vector<cv::Vec4i> ttmplines;
	//	//ttmplines.swap(tmplines);
	//	int linenum = std::min(LINENUM, static_cast<int>(ttmplines.size() * 0.2));
	//	tmplines.clear();
	//	tmplines.insert(tmplines.begin(),ttmplines.begin(), ttmplines.begin() + linenum);
	//	//tmplines.insert(ttmplines.begin(), ttmplines.begin() + linenum);
	//}
	auto removeshortline = [&LINENUM](vector<cv::Vec4i>& lines){
		if (lines.size() < LINENUM) return;
		vector<cv::Vec4i> tmplines;
		tmplines.swap(lines);
		sort(tmplines.begin(), tmplines.end(), [](cv::Vec4i&line1, cv::Vec4i& line2){ \
			return (pow(abs(line1[0] - line1[2]), 2) + pow(abs(line1[1] - line1[3]), 2)) > \
			(pow(abs(line2[0] - line2[2]), 2) + pow(abs(line2[1] - line2[3]), 2)); });
		lines.insert(lines.begin(), tmplines.begin(), tmplines.begin() + LINENUM);

	};
	removeshortline(ups);
	lines.emplace_back(ups);
	removeshortline(downs);
	lines.emplace_back(downs);
	removeshortline(lefts);
	lines.emplace_back(lefts);
	removeshortline(rights);
	lines.emplace_back(rights);
	//return  lines.size() == 4;
#ifdef _DEBUG
	cv::Mat cdst;
	cv::cvtColor(gray, cdst, CV_GRAY2BGR);
	for (size_t i = 0; i < lines.size(); ++i)
	{
		for (size_t j = 0; j < lines[i].size(); ++j)
		{
			cv::Vec4i& l = lines[i][j];
			cv::line(cdst, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255 * (i<=1), 0, 255*(i>1)), 1, CV_AA);
		}
	}
	cv::imshow("lines", cdst);
	cv::waitKey();
#endif //end _DEBUG
}
void LSDDetectEdge::mergeLines(vector<vector<cv::Vec4i>>& lines, vector<cv::Vec4i>& edges)
{
	for (size_t i = 0; i < lines.size(); ++i)
	{
		cv::Vec4i edge;
		Tools::MergeLines(lines[i], edge, static_cast<int>(i >= 2));
		edges.emplace_back(edge);
	}
}
