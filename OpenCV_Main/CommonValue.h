#pragma once

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\types_c.h>
namespace Common
{
	// 魚の名前と番号
	enum FISHNAMES
	{
		KAWAMEDAKA, // 魚番号1: カワメダカ
		HIMEDAKA,	// 魚番号2: ヒメダカ
		KOGANE,		// 魚番号3: 小金
		FISHNUM
	};

	//Scalarをペアで管理するためのデータ構造
	class PairScalar
	{
	public:
		const cv::Scalar min;
		const cv::Scalar max;

		PairScalar(cv::Scalar min_Value, cv::Scalar max_Value)
		: min(min_Value), max(max_Value)
		{}
	};
}