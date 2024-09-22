// ConsoleApplication.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\types_c.h>


/*  OpenCVの名前空間を使用する  */
using namespace cv;

/*--  列挙型  --*/
// 画像の種類( ImageType )
enum {
	IT_Source,		// 元画像
	IT_Gray,		// 輝度化画像
	IT_Binary,		// 二値化画像
	IT_Smooth,		// 平滑化画像
	IT_Laplacian,	// ラプラシアンエッジ画像
	IT_Affine,		// アフィン変換画像
	IT_NUM			// 画像の種類の数
};


/*--  グローバル変数  --*/
// ウィンドウの名前
const char g_WindowName[IT_NUM][20] = { "Source Image", "Gray Image", "Binary Image", "Smooth Image", "Laplacian Image", "Affine Image" };

// 画像の名前
const char g_ImageName[20] = { "./lena.jpg" };


/*--  メイン関数  --*/
int main()
{
	/*  変数宣言  */
	// 画像を読み込む変数
	Mat image[IT_NUM];


	/*  元画像読み込み  */
	image[IT_Source] = imread(g_ImageName, IMREAD_COLOR);


	/*  輝度化画像作成  */
	// 輝度化
	cvtColor(image[IT_Source], image[IT_Gray], CV_BGR2GRAY);


	/*  二値化画像作成  */
	// 二値化
	threshold(image[IT_Gray], image[IT_Binary], 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	
	/*  平滑化画像作成  */
	// 平滑化
	GaussianBlur(image[IT_Source], image[IT_Smooth], Size(25, 25), 10, 10);


	/*  ラプラシアン画像作成  */
	// ブロックで隔離
	{
		// ラプラシアンのエッジ及び合計のエッジを保持する変数
		Mat laplacian;

		// ラプラシアンのエッジを求める
		Laplacian(image[IT_Gray], laplacian, CV_32F, 3);

		// 符号付き32bitのデータを符号なし8bitのデータに変換する
		convertScaleAbs(laplacian, image[IT_Laplacian], 1, 0);
	}


	/*  アフィン変換画像作成  */
	// ブロックで隔離
	{
		// アフィン変換行列を初期化
		Mat affineMatrix;
		// 回転の中心
		CvPoint2D32f center = { image[IT_Source].cols * 0.5f, image[IT_Source].rows * 0.5f };
		// その他パラメータ
		double angle = 30.0, scale = 0.8;

		// 画像を回転させるアフィン行列を算出
		affineMatrix = getRotationMatrix2D(center, angle, scale);

		// アフィン変換
		warpAffine(image[IT_Source], image[IT_Affine], affineMatrix, image[IT_Source].size());
	}


	/*  ウィンドウ関係の処理  */
	for (int i = 0; i < IT_NUM; ++i) {
		// ウィンドウ作成
		namedWindow(g_WindowName[i], WINDOW_AUTOSIZE);

		// ウィンドウに画像を表示
		imshow(g_WindowName[i], image[i]);
	}

	/*  キー入力があるまで待つ  */
	waitKey(0);

	/*  全てのウィンドウを破棄  */
	// ( destoroyWindow( "ウィンドウ名" )でも可 )
	destroyAllWindows();

	return 0;
}