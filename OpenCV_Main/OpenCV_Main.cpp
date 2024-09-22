#include <iostream>
#include"Repository.h"

using namespace Repository;

int main()
{
    const std::string data_roomPath = "./data/";
    const std::string problem_name = "Problem_01";

    IRepository* DataReader = new InputFileReader(data_roomPath, problem_name);

    int width, height, fourcc;
    fourcc = cv::VideoWriter::fourcc('H', '2', '6', '4');    //ビデオフォーマットの指定
    double fps;

    DataReader->ReadMovie(width, height, fps);
}


