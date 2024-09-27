#include <iostream>
#include"Repository.h"
#include"TemplateMatching.h"
#include"Binarization.h"
#include"BoundingBox.h"
#include"CommonValue.h"
#include"BackGroundDiff.h"

using namespace Common;
using namespace Repository;
using namespace DetectionMethod;
using namespace Drawable;

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

int main()
{
    const std::string data_roomPath = "./data/";
    const std::string problem_name = "Problem_01";

    IRepository* DataReader = new InputFileReader(data_roomPath, problem_name);

    int width, height, fourcc;
    fourcc = cv::VideoWriter::fourcc('H', '2', '6', '4');    //ビデオフォーマットの指定
    double fps;
    cv::VideoCapture video;	//動画を読み込む用のクラス

    DataReader->CreateVideo(width, height, fps, video);  //MP4動画出力

    cv::Mat image; //画像格納用オブジェクト
    cv::Mat result_image; //結果画像を格納するオブジェクト
    cv::Mat hsv;
    cv::Mat labeled;

    // 各魚の検出範囲を保存する変数

    bool is_first_frame = true;


    video >> image;

    //TemplateMatching* method = new TemplateMatching();
    Binarization* binaryMethod = new Binarization();
    BoundingBox* boundingBox = new BoundingBox();
    BackGroundDiff* backGroundDiff = new BackGroundDiff(image);


    while (1)
    {
        video >> image; //videoからimageへ1フレーム取り込む

        if (image.empty() == true)  //画像が読み込めなかった時ループ終了
            break;
        

        result_image = image.clone();   //コピー
        cv::Mat binary_Result_KOGANE;  //2値化画像保存用変数
        cv::Mat binary_Result_KAWAMEDAKA;  //2値化画像保存用変数
        cv::Mat binary_Result_HIMEDAKA;  //2値化画像保存用変数
        cv::Mat diff_Result; //背景差分法の結果画像
        cv::Mat And_Result;

        //動体検知
        diff_Result = backGroundDiff->CalcDiffBackGround(image);    //1フレーム目との差分を取って動体を検知
        diff_Result = binaryMethod->MorphologicalProcessing(diff_Result);   //膨張・収縮処理でノイズ除去



        //小金の色検知
        binary_Result_KOGANE = binaryMethod->ToBinary(image, KOGANE);    //imageをhsvに変換して2値化
        binary_Result_KOGANE = binaryMethod->MorphologicalProcessing(binary_Result_KOGANE);   //膨張・収縮処理でノイズ除去
        
        //動いていてかつ小金の色に一致する物体
        cv::bitwise_and(diff_Result, binary_Result_KOGANE, And_Result);  //色判別と動体検知の結果を論理積

        //バウンディングボックス生成
        boundingBox->DetectionFish(And_Result, KOGANE);
        boundingBox->DrawRectangle(result_image, KOGANE);


        //カワメダカの色検知
        binary_Result_KAWAMEDAKA = binaryMethod->ToBinary(image, KAWAMEDAKA);    //imageをhsvに変換して2値化
        binary_Result_KAWAMEDAKA = binaryMethod->MorphologicalProcessing(binary_Result_KAWAMEDAKA);   //膨張・収縮処理でノイズ除去
        
        //動いていてかつカワメダカの色に一致する物体
        cv::bitwise_and(diff_Result, binary_Result_KAWAMEDAKA, And_Result);  //色判別と動体検知の結果を論理積

        //バウンディングボックス生成
        boundingBox->DetectionFish(And_Result, KAWAMEDAKA);
        boundingBox->DrawRectangle(result_image, KAWAMEDAKA);

        //ヒメダカの色検知
        binary_Result_HIMEDAKA = binaryMethod->ToBinary(image, HIMEDAKA);    //imageをhsvに変換して2値化
        binary_Result_HIMEDAKA = binaryMethod->MorphologicalProcessing(binary_Result_HIMEDAKA);   //膨張・収縮処理でノイズ除去

        //動いていてかつヒメダカの色に一致する物体
        cv::bitwise_and(diff_Result, binary_Result_HIMEDAKA, And_Result);  //色判別と動体検知の結果を論理積

        //バウンディングボックス生成
        boundingBox->DetectionFish(And_Result, HIMEDAKA);
        boundingBox->DrawRectangle(result_image, HIMEDAKA);


        //cv::imshow("Debug", And_Result);    //ウィンドウに動画を表示する
        //DataReader->DrawToVideo(And_Result);
        
        

        //cv::imshow("Debug", diff_Result);    //ウィンドウに動画を表示する
        //DataReader->DrawToVideo(diff_Result);


        cv::imshow("showing", result_image);    //ウィンドウに動画を表示する
        DataReader->DrawToVideo(result_image);

        //このフレームで検知した魚の数を計算し、これまでで一番多ければ更新する
        boundingBox->CalcFishNum();


        // 初期化
        boundingBox->ClearRectangle();

        if (cv::waitKey(1) == 'q') break; //qを押すと終了
    }

    // csvファイル書き込み用に配列を作成
    std::vector<std::string> wtite_contents(7);
    wtite_contents[0] = problem_name;
    wtite_contents[1] = "3";

    for (int i = 0; i < FISHNUM; i++) {
        wtite_contents[i + 2] = std::to_string(i + 1) + "," + std::to_string(boundingBox->fishNum[i]);
    }

    CsvWriter("./" + problem_name + ".csv", wtite_contents);

    return 0;
}


