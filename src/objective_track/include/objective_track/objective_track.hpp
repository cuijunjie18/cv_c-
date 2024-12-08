#ifndef OBJECTIVE_TRACK_INCLUDE_HPP
#define OBJECTIVE_TRACK_INCLUDE_HPP
#include "my_common.hpp"

// 展示函数
void show(cv::Mat img,std::string name = "debug");

// 返回待识别物体颜色的HSV上下界
void cvt_hsv(cv::Mat hsv,cv::Scalar &l_hsv,cv::Scalar &h_hsv);

// 对mask二级图找轮廓，并在原img用外接矩形标记特征物体
void track(cv::Scalar l_hsv,cv::Scalar h_hsv,cv::Mat &img,std::vector<cv::Point2d> &pts);

// debug用
void check(cv::Scalar l_hsv,cv::Scalar h_hsv,cv::Scalar track_color);

#endif