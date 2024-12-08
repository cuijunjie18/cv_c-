#ifndef POSE_CALCULATOR_INCLUDE_HPP
#define POSE_CALCULATOR_INCLUDE_HPP

#include "my_common.hpp"

cv::Point2d pixel2cam ( const cv::Point2d &p, const cv::Mat &K );

void my_Pnp(Eigen::Vector3d pts_c_solve[5],cv::Mat img);

#endif