#include "BoundingBox.h"

/// <summary>
/// 矩形の管理・描画するためのクラス
/// </summary>
namespace Drawable
{
	void Rectangles::AddRect(cv::Rect rect)
	{
		FishRectangle.emplace_back(rect);
	}

	std::vector<cv::Rect>& Rectangles::GetRectVec()
	{
		return FishRectangle;
	}

	int Rectangles::GetVecLength()
	{
		return FishRectangle.size();
	}

	void Rectangles::ClearVec()
	{
		FishRectangle.clear();
	}

	void BoundingBox::DetectionFish(const cv::Mat src, const int FishType)
	{
		SetLabel(src);
		CalcRect(FishType);
	}

	void BoundingBox::SetLabel(const cv::Mat src)
	{
		//ラベリング
		labelCount = cv::connectedComponentsWithStats(src, labeled_image, stats, centroids);
	}

	void BoundingBox::CalcRect(const int FishType)
	{
		for (int i = 0; i < labelCount; ++i)
		{
			int* param = stats.ptr<int>(i);

			int area = param[cv::ConnectedComponentsTypes::CC_STAT_AREA];

			if (area > Area_Size.at(FishType))
			{
				// 条件を満たせば矩形を計算
				int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
				int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
				int height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
				int width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
			
				RectanglesOfFish[FishType].AddRect(cv::Rect(x, y, width, height));
			}
		}
	}

	void BoundingBox::DrawRectangle(cv::Mat& src, int FishType)
	{
		for (const auto& each_area : RectanglesOfFish.at(FishType).GetRectVec())
		{
			cv::rectangle(src, each_area, RectangleColor.at(FishType), 2);
		}
	}

	void BoundingBox::ClearRectangle()	//全要素をクリア
	{
		auto begin = RectanglesOfFish.begin(), end = RectanglesOfFish.end();
		for (auto iter = begin; iter != end; iter++)
		{
			RectanglesOfFish[iter->first].ClearVec();
		}
	}

	void BoundingBox::CalcFishNum()
	{
		for (auto& pair : RectanglesOfFish)	
		{
			if(fishNum[pair.first] < pair.second.GetVecLength())
				fishNum[pair.first] = pair.second.GetVecLength();
		}
	}

}