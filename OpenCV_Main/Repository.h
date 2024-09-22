#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\types_c.h>

namespace Repository
{
	class IRepository
	{
	protected:
		std::string _fileRootPath = "";	//入力ファイルの元のパス
		std::string _fileDataPath = "";	//データファイルのパス
		std::string _fileMoviePath = "";//動画ファイルのパス
		std::string _outFilePath = "";	//結果の出力先パス
	
	public:
		virtual int ReadMovie(int& width, int& height, double& fps) = 0;	//動画よみこみ
		virtual int ReadData() = 0;	//データ読み込み
	};


	/// <summary>
	/// CSVファイルからファイルパス,データ情報を取得
	/// </summary>
	class InputFileReader : public IRepository
	{
	private:
		std::vector<std::vector<std::string>> read_csv_contents = std::vector<std::vector<std::string>>(0);
		cv::VideoCapture video;	
		cv::VideoWriter writer;

		int ReadData() override;
		
		std::vector<std::string> Split(const std::string& input, const char delimiter);

	public:
		InputFileReader(std::string data_rootPath, std::string problem_name);
		int ReadMovie(int& width, int& height, double& fps) override;
		
		 
	};
}