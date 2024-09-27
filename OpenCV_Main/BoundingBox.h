#pragma once
#include"CommonValue.h"

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\types_c.h>

using namespace Common;
namespace Drawable
{
	class Rectangles
	{
	private:
		std::vector<cv::Rect> FishRectangle;
	public:
		void AddRect(cv::Rect);
		std::vector<cv::Rect>& GetRectVec();
		int GetVecLength();
		void ClearVec();
	};

	class BoundingBox
	{
	private:
		std::map<int, Rectangles> RectanglesOfFish;	//FISHNAME‚ğKey‚Æ‚µ‚Ä‘Î‰‚·‚é‹›‚ÌRect‚ğŠÇ—
		const std::map<int, cv::Scalar> RectangleColor
		{
			{KAWAMEDAKA, cv::Scalar(0, 100, 0)},
			{HIMEDAKA, cv::Scalar(100, 0, 0)},
			{KOGANE, cv::Scalar(0, 0, 100)}
		};

		const int area_size = 600;	//—Ìˆæ‚ÌÅ¬’l
		const std::map<int, int> Area_Size
		{
			{KOGANE,700},
			{KAWAMEDAKA, 500},
			{HIMEDAKA,300}
		};

		cv::Mat stats;
		cv::Mat centroids;
		cv::Mat labeled_image;
		
		int labelCount = 0;
		void SetLabel(const cv::Mat src);
		void CalcRect(const int FishType);
	public:
		int fishNum[FISHNUM] = { 0,0,0 };
		void DetectionFish(const cv::Mat src, const int FishType);
		void DrawRectangle(cv::Mat& src, const int FishType);
		void ClearRectangle();
		void CalcFishNum();
	};
}

