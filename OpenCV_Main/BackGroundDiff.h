#pragma once
#include"BoundingBox.h"
#include"Binarization.h"

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\types_c.h>

using namespace Drawable;

namespace DetectionMethod
{
	class BackGroundDiff
	{
	private:
		Binarization* binary;
		BoundingBox* bounding;
		cv::Mat backGround;	//元となる画像(1フレーム目)
		cv::Mat frame1_img;	//2フレーム目の画像
		cv::Mat frame2_img;	//3フレーム目の画像
		cv::Mat diff,diff1, diff2;	//各フレームの差分を保管する用の変数
		int count = 0;
		~BackGroundDiff();
	public:
		BackGroundDiff(const cv::Mat base);
		cv::Mat CalcDiffBackGround(const cv::Mat image);
	};
}


