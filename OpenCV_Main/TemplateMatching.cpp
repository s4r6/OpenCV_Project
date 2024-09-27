#include "TemplateMatching.h"
/// <summary>
/// テンプレートマッチングをするクラス(使ってない)
/// </summary>
namespace DetectionMethod
{
	TemplateMatching::TemplateMatching()
	{
		auto begin = HashName.begin(), end = HashName.end();	//テンプレート画像を読み込む
		for (auto iter = begin; iter != end; iter++)
		{
			HashImage[iter->first] = cv::imread(tmp_imgPath + HashName[iter->first] + ".jpg");
			if (!HashImage[iter->first].data)
			{
				std::cout << iter->first << "が読み込めませんでした" << std::endl;
			}
			return;
		}
	}

	std::vector<cv::Rect> TemplateMatching::Do(cv::Mat& search_img, int fishType)
	{
		std::vector<cv::Rect> returnVec;
		for (int i = 0; i < 50; i++)
		{
			
			cv::matchTemplate(search_img, HashImage[fishType], result_img, CV_TM_CCOEFF_NORMED);	//テンプレートマッチング
			//最大スコアの場所を探す
			cv::Rect roi_rect(0, 0, HashImage[fishType].cols, HashImage[fishType].rows);
			cv::Point max_pt;
			double maxVal;
			cv::minMaxLoc(result_img, NULL, &maxVal, NULL, &max_pt);

			if (maxVal < 0.5)	//スコア0.5以下なら終了
				break;

			roi_rect.x = max_pt.x;
			roi_rect.y = max_pt.y;

			returnVec.emplace_back(roi_rect);
		}
		
		return returnVec;
	}
}