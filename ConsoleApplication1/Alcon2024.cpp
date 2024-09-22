// ConsoleApplication1.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\types_c.h>

// 魚の名前と番号
enum FISHNAMES
{
	KAWAMEDAKA, // 魚番号1: カワメダカ
	HIMEDAKA,	// 魚番号2: ヒメダカ
	KOGANE,		// 魚番号3: 小金
	FISHNUM

};

// csvファイルから読み取った1行を区切り文字で更に区切る関数
// 引数
//	input		csvファイルから読み取った1行
//	delimiter	区切り文字
std::vector<std::string> split(std::string& input, char delimiter) {
	std::istringstream stream(input);
	std::string field;
	std::vector<std::string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

// csv読み取り関数
//引数 
//	target_csv_path 読み取り先のファイル
//	delimiter		csvの区切り文字
//	csv_contents	読み取り結果を格納する変数
int CsvReader(const std::string& target_csv_path, const char delimiter, std::vector<std::vector<std::string>>& csv_contents) {

	// 中身が入っているなら終了
	if (!csv_contents.empty())
		return -1;

	std::ifstream ifs(target_csv_path);
	std::string line;

	while (std::getline(ifs, line)) {

		std::vector<std::string> strvec = split(line, ',');

		// 結果に格納
		csv_contents.emplace_back(strvec);
	}
}


// csv書き込み処理
//引数：
//	target_csv_path 書き込み先
//	wtite_contents	書き込み内容
int CsvWriter(const std::string& target_csv_path, std::vector<std::string>& wtite_contents) {

	std::ofstream ofs(target_csv_path);

	for (auto& each : wtite_contents) {
		ofs << each + "\n";
	}

	return 0;
}


/*--  メイン関数  --*/
int main()
{
	/*--- 入力に関する処理（ここから） ---*/
	// 問題番号
	const std::string problem_name = "Problem_01";

	// 動画ファイルのパスの文字列を格納するオブジェクトを宣言
	const std::string filepath_problem_dir = "./data/" + problem_name;
	const std::string filepath_input_csv = filepath_problem_dir + "/input-" + problem_name + ".csv";
	const std::string out_filepath = "./data/" + problem_name + "/result.mp4";

	// csvファイルの読み取り
	std::vector<std::vector<std::string>> read_csv_contents(0);
	CsvReader(filepath_input_csv, ',', read_csv_contents);

	// 動画ファイル名の読み取り
	const std::string filepath_input_movie = filepath_problem_dir + "/" + read_csv_contents.back()[0];

	// 動画ファイルを取り込むためのオブジェクトを宣言
	cv::VideoCapture video;
	// 動画ファイルを書き出すためのオブジェクトを宣言
	cv::VideoWriter writer;

	// 動画ファイルを開く
	video.open(filepath_input_movie);
	if (video.isOpened() == false) {
		// 動画ファイルが開けなかったときは終了する
		return -1;
	}

	// 作成する動画ファイルの設定
	int width, height, fourcc;
	fourcc = cv::VideoWriter::fourcc('H', '2', '6', '4'); // ビデオフォーマットの指定( ISO MPEG-4 / .mp4)
	double fps;
	
	width = std::atoi(read_csv_contents[1][0].c_str());	// フレーム横幅を取得
	height = std::atoi(read_csv_contents[2][0].c_str());	// フレーム縦幅を取得
	fps = std::atoi(read_csv_contents[5][0].c_str());		// フレームレートを取得

	writer.open(out_filepath, fourcc, fps, cv::Size(width, height));
	/*--- 入力に関する処理（ここまで） ---*/



	cv::Mat image;// 画像を格納するオブジェクトを宣言
	cv::Mat first_frame_image;		// 第1フレームの画像を格納するオブジェクトを宣言
	cv::Mat hsv;
	cv::Mat labeled;
	cv::Mat result_image; //結果画像を格納するオブジェクトを宣言

	int each_fish_num[FISHNUM] = {}; // 各魚のカウント結果を保存する変数

	// 各魚の検出範囲を保存する変数
	std::vector<cv::Rect> kawamedaka_areas(0);
	std::vector<cv::Rect> himedaka_areas(0);
	std::vector<cv::Rect> kogane_areas(0);

	bool is_first_frame = true;

	// 各魚の計測数を保存
	int each_fishs_num[FISHNUM] = {};

	while (1) {
		video >> image; // videoからimageへ1フレームを取り込む

		if (image.empty() == true) break; // 画像が読み込めなかったとき、無限ループを抜ける

		result_image = image.clone(); // 複製

		/*--- 前処理（ここから） ---*/
		cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV); // hsv変換画像の作成
		/*--- 前処理（ここまで） ---*/


		/*--- 小金候補を抽出（ここから） ---*/
		// hsv画像内の値が一定範囲の画素について2値化
		// H: 0から30, S: 150から255, V: 0から255　の値の画素が対象
		cv::Scalar hsv_min(10, 160, 170);
		cv::Scalar hsv_max(15, 255, 255);
		cv::Mat hsv_mask;	// 2値化結果を格納
		cv::inRange(hsv, hsv_min, hsv_max, hsv_mask);	// 2値化

		// 収縮と膨張処理でhsv後に2値化した画像からノイズを除去
		cv::Mat hsv_mask_corrected = hsv_mask.clone();
		cv::erode(hsv_mask_corrected, hsv_mask_corrected, cv::Mat(), cv::Point(-1, -1), 1); // 収縮
		cv::dilate(hsv_mask_corrected, hsv_mask_corrected, cv::Mat(), cv::Point(-1, -1), 1); // 膨張
		cv::dilate(hsv_mask_corrected, hsv_mask_corrected, cv::Mat(), cv::Point(-1, -1), 1); // 膨張
		cv::erode(hsv_mask_corrected, hsv_mask_corrected, cv::Mat(), cv::Point(-1, -1), 1); // 収縮

		// 金魚候補領域をラベリング
		cv::Mat stats;
		cv::Mat centroids;
		int nlabel = cv::connectedComponentsWithStats(hsv_mask_corrected, labeled, stats, centroids);

		// 600より面積が大きい領域のラベルを取得
		const int area_th = 600;
		
		for (int i = 1; i < nlabel; ++i) {
			int* param = stats.ptr<int>(i);

			int area = param[cv::ConnectedComponentsTypes::CC_STAT_AREA];

			if (area > area_th) {
				// 条件を満たせば小金とする
				int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
				int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
				int height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
				int width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];

				kogane_areas.emplace_back(x, y, width, height);
			}
		}
		/*--- 小金候補を抽出（ここまで） ---*/



		/*--- 検出結果の表示（ここから）---*/
		//カワメダカのバウンディングボックス作成
		for (const auto& each_area : kawamedaka_areas) {
			cv::rectangle(result_image, each_area, cv::Scalar(0, 100, 0), 2);
		}
		
		//ヒメダカのバウンディングボックス作成
		for (const auto& each_area : himedaka_areas) {
			cv::rectangle(result_image, each_area, cv::Scalar(100, 0, 0), 2);
		}

		//小金（黒）のバウンディングボックス作成
		for (const auto& each_area : kogane_areas) {
			cv::rectangle(result_image, each_area, cv::Scalar(0, 0, 100), 2);
		}

		/*--- 検出結果の表示（ここまで）---*/


		cv::imshow("showing", result_image); // ウィンドウに動画を表示する

		writer << result_image;  // 画像 image を動画ファイルへ書き出す

		// 計測した魚の数を保存
		int tmp_fish_nums[FISHNUM] = { kawamedaka_areas.size(), 
			himedaka_areas.size(), kogane_areas.size() };

		for (int i = 0; i < FISHNUM; i++) {
			if (each_fishs_num[i] < tmp_fish_nums[i])
				each_fishs_num[i] = tmp_fish_nums[i];
		}
			

		// 初期化
		kawamedaka_areas.clear();
		himedaka_areas.clear();
		kogane_areas.clear();

		if (cv::waitKey(1) == 'q') break; //qを押すと終了
	}

	// csvファイル書き込み用に配列を作成
	std::vector<std::string> wtite_contents(7);
	wtite_contents[0] = problem_name;
	wtite_contents[1] = "3";

	for (int i = 0; i < FISHNUM; i++) {
		wtite_contents[i + 2] = std::to_string(i + 1) +"," + std::to_string(each_fishs_num[i]);
	}

	CsvWriter("./" + problem_name + ".csv", wtite_contents);

	return 0;
}