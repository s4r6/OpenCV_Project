#pragma once
#include<map>
#include"CommonValue.h"

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\types_c.h>

using namespace Common;

namespace DetectionMethod
{
	

	class TemplateMatching
	{
	private:
		std::map<int, std::string> HashName = {
			{KAWAMEDAKA, "Kawamedaka"},
			{HIMEDAKA, "Himedaka"},
			{KOGANE, "Syoukin"}
		};

		std::map<int, cv::Mat> HashImage;

		const std::string tmp_imgPath = "./Template/";
		//const cv::Mat tmp_img;
		//const cv::Mat search_img;
		cv::Mat result_img;
	public:
		TemplateMatching();
		std::vector<cv::Rect> Do(cv::Mat& search_img, int fishType);
	};
}


