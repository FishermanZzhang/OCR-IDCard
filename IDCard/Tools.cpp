#include "Tools.h"


Tools::Tools()
{
}


Tools::~Tools()
{
}


void Tools::MergeLines(vector<cv::Vec4i>& lines,cv::Vec4i& edge, int type)
{
	if (type == 1)
	{
		MergeVerticalLines(lines, edge); // ��ֱ��
	}
	else{
		MergeHorizontalLines(lines, edge); //ˮƽ��
	}
}
void Tools::MergeHorizontalLines(vector<cv::Vec4i>& lines, cv::Vec4i& edge)
{
	//ˮƽֱ�ߺϲ�����

	//�����ϲ����������ֱ��
	auto distancepowerfromVec4i = [](cv::Vec4i& line1){
		return (pow(abs(line1[0] - line1[2]), 2) + pow(abs(line1[1] - line1[3]), 2));
	};
	double dis = 0.0;
	for (size_t i = 0; i < lines.size(); ++i)
	{
		double tdis = 0.0;
		if ((tdis = distancepowerfromVec4i(lines[i])) > dis)
		{
			dis = tdis;
			edge = lines[i];
		}
	}
}
void Tools::MergeVerticalLines(vector<cv::Vec4i>& lines, cv::Vec4i& edge)
{
	//��ֱֱ��ת��ˮƽֱ�ߣ�����ˮƽֱ�ߺϲ�����
	vector<cv::Vec4i> tmplines(lines.size());
	for (size_t i = 0; i < lines.size(); ++i)
	{
		cv::Vec4i line = lines[i];
		std::swap(line[0], line[1]);
		std::swap(line[2], line[3]);
		tmplines.emplace_back(line);
	}
	MergeHorizontalLines(tmplines, edge);
	std::swap(edge[0], edge[1]);
	std::swap(edge[2], edge[3]);

}

cv::Point Tools::CrossPoint(const cv::Vec4i& line1, const cv::Vec4i& line2)
{
	cv::Point p;
	float denominator = (line1[1] - line1[3])*(line2[2] - line2[0]) - (line1[2] - line1[0])*(line2[1] - line2[3]);
	p.x = (line1[1] * line1[2] - line1[0] * line1[3])*(line2[2] - line2[0]) - (line1[2] - line1[0])*(line2[1] * line2[2] - line2[0] * line2[3]);
	p.x = p.x / denominator;
	p.y = (line1[1] - line1[3])*(line2[1] * line2[2] - line2[0] * line2[3]) - (line2[1] - line2[3])*(line1[1] * line1[2] - line1[0] * line1[3]);
	p.y = p.y / denominator;
	return p;
}