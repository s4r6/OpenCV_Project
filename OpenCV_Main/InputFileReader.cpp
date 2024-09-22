#include"Repository.h"

namespace Repository 
{
	//指定されたファイルを読み込む
	InputFileReader::InputFileReader(std::string data_rootPath, std::string problem_name)
	{
		//PATHを初期化
		_outFilePath = data_rootPath + problem_name + "/result.mp4";
		_fileRootPath = data_rootPath;
		_fileDataPath = data_rootPath + "input-" + problem_name;	
		
		ReadData();	//CSVファイル読み込み

		_fileMoviePath = _fileRootPath + read_csv_contents.back()[0];	//後ろから0番目の要素(動画名)を取得

		ReadMovie();	//動画(mp4)ファイル読み込み
	}

	//CSVファイルの文字列を","で分割して格納
    int InputFileReader::ReadData(){
		//中身が入っているなら終了
		if (!read_csv_contents.empty())
			return -1;

		std::ifstream ifs(_fileDataPath);
		std::string line;

		while (std::getline(ifs, line))	//1行ずつ読み込み
		{
			std::vector<std::string> strvec = Split(line, ',');	//読み取った文字列を分割して取得

			read_csv_contents.emplace_back(strvec);
		}
	}

	int InputFileReader::ReadMovie(int* width, int* height, double* fps)
	{
		video.open(_fileMoviePath);
		if (video.isOpened() == false)
		{
			return -1;
		}

		int width, height, fourcc;
		fourcc = cv::VideoWriter::fourcc('H', '2', '6', '4');	//ビデオフォーマットの指定
		double fps;

		width = std::atoi(read_csv_contents[1][0].c_str());	//横幅取得
		height = std::atoi(read_csv_contents[2][0].c_str());	//縦幅取得
		fps = std::atoi(read_csv_contents[5][0].c_str());	//フレームレートを取得

		writer.open(_outFilePath, fourcc, fps, cv::Size(width, height));
	}

	std::vector<std::string> InputFileReader::Split(const std::string& input, const char delimiter)
	{
		std::istringstream stream(input);	
		std::string field;	
		std::vector<std::string> result;	//区切った文字列を格納

		//入力された文字列を","で区切ってfieldに格納
		while (getline(stream, field, delimiter))	
		{
			result.push_back(field);
		}
		return result;
	}
}