#include "BackGroundDiff.h"
///<summary>
///背景差分法をするクラス
///

namespace DetectionMethod
{
	BackGroundDiff::BackGroundDiff(const cv::Mat base) : backGround(base)
	{
		binary = new Binarization();
		/*video >> frame1_img;
		video >> frame2_img;
		*/
		backGround = binary->ToGray(backGround);	//各フレームをグレースケール化
		/*frame1_img = binary->ToGray(frame1_img);
		frame2_img = binary->ToGray(frame2_img);
		*/
	}

	/// <summary>
	/// 背景差分法の実装関数
	/// 動体を二値化して返す
	/// </summary>
	/// <param name="video">
	/// 対象の動画
	/// </param>
	/// <returns>
	/// 動いている物体を白色に変換した二値化画像を返す
	/// </returns>
	cv::Mat BackGroundDiff::CalcDiffBackGround(const cv::Mat image)
	{
		cv::Mat Gray;
		Gray = binary->ToGray(image);
		cv::absdiff(backGround, Gray, diff);
		cv::threshold(diff, diff, 30, 255, cv::THRESH_BINARY);
		//1フレーム目と2フレーム目の差分
		/*cv::absdiff(backGround, frame1_img, diff1);
		//2フレーム目と3フレーム目の差分
		cv::absdiff(frame1_img, frame2_img, diff2);
		//差分1と差分2の結果を論理積で比較
		cv::bitwise_and(diff1, diff2, diff);

		cv::threshold(diff, diff, 30, 255, cv::THRESH_BINARY);

		//フレームをずらす
		frame1_img.copyTo(backGround, frame1_img);
		frame2_img.copyTo(frame1_img, frame2_img);

		video >> frame2_img;
		frame2_img = binary->ToGray(frame2_img);
		
		*/
		return diff;
	}

	BackGroundDiff::~BackGroundDiff()
	{
		delete binary;
		binary = nullptr;
	}
}