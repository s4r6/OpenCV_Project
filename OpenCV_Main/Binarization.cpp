#include "Binarization.h"
/// <summary>
/// 二値化するためのクラス
/// </summary>
namespace DetectionMethod
{
	cv::Mat Binarization::ToGray(const cv::Mat src)
	{
		cv::Mat gray;
		cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);	//グレースケール化
		return gray;
	}

	/// <summary>
	/// hsv形式に変換して2値化する
	/// </summary>
	/// <param name="src">
	/// 2値化する前の元画像
	/// </param>
	/// <param name="FishType">
	/// どの魚の閾値に対して2値化するかの識別子(FISHNAMES)で渡す
	/// </param>
	/// <returns>
	/// 2値化したcv::Mat形式の画像を返す
	/// </returns>
	cv::Mat Binarization::ToBinary(const cv::Mat src, const int FishType)
	{
		cv::Mat hsv_result;
		cv::cvtColor(src, hsv_result, cv::COLOR_BGR2HSV);	//srcをhsvに変換して保管
		
		cv::Mat binary_rusult;	//2値化の結果を格納する変数
		cv::Scalar max = FishThreshold.at(FishType).max;
		cv::Scalar min = FishThreshold.at(FishType).min;

		cv::inRange(hsv_result, min, max, binary_rusult);	//特定の閾値で2値化
		return binary_rusult;
	}

	/// <summary>
	/// 膨張・収縮によるノイズ除去
	/// </summary>
	/// <param name="src">
	/// ノイズ除去する前の元画像
	/// </param>
	/// <returns>
	/// cv:Mat形式でノイズ除去後の画像が返ってくる
	/// </returns>
	cv::Mat Binarization::MorphologicalProcessing(const cv::Mat src)
	{
		// 収縮と膨張処理でhsv後に2値化した画像からノイズを除去
		cv::Mat hsv_mask_corrected = src.clone();
		cv::erode(hsv_mask_corrected, hsv_mask_corrected, cv::Mat(), cv::Point(-1, -1), 1); // 収縮
		cv::dilate(hsv_mask_corrected, hsv_mask_corrected, cv::Mat(), cv::Point(-1, -1), 1); // 膨張
		cv::dilate(hsv_mask_corrected, hsv_mask_corrected, cv::Mat(), cv::Point(-1, -1), 1); // 膨張
		cv::erode(hsv_mask_corrected, hsv_mask_corrected, cv::Mat(), cv::Point(-1, -1), 1); // 収縮
	
		return hsv_mask_corrected;
	}
}