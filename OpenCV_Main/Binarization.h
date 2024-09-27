#pragma once
#include "TemplateMatching.h"
#include "CommonValue.h"

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\types_c.h>

using namespace Common;

namespace DetectionMethod
{
	class Binarization
	{
	private:
		const std::map<const int, PairScalar> FishThreshold	//ËáílÇFISHNAMESÇ≈ä«óù
		{
			{KOGANE, PairScalar(cv::Scalar(0,100,170), cv::Scalar(15,255,255))},
			{KAWAMEDAKA, PairScalar(cv::Scalar(15, 150, 120), cv::Scalar(30, 220, 200))},
			{HIMEDAKA, PairScalar(cv::Scalar(15, 200, 150), cv::Scalar(30, 255, 200))}
		};
	public:
		//Binarization();
		cv::Mat ToGray(const cv::Mat src);
		cv::Mat ToBinary(const cv::Mat src, const int FishType);
		cv::Mat MorphologicalProcessing(const cv::Mat src);
	};
}

