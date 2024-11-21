#include <iostream>
#include <opencv2/opencv.hpp>
#define INF 100000000
using namespace std;

// ROI数字区域矩形排序
bool cmp_rect(cv::Rect x,cv::Rect y){
    return x.x < y.x;
}

// 展示
void show(string title,cv::Mat img)
{
    cv::imshow(title,img);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return;
}

// 轮廓排序
void contours_sort(vector<vector<cv::Point>> & contours,bool reverse = false)
{
    int n = contours.size(); // 计算轮廓数量

    // 基于选择排序将轮廓排序
    for (int i = 0; i < n - 1; i++){
        int p = i;
        int cmp = reverse?-1:INF;
        for (int j = i; j < n; j++){
            cv::Rect temp = cv::boundingRect(contours[j]);
            if ((temp.x < cmp) ^ (reverse)){
                cmp = temp.x;
                p = j;
            }
        }
        if (p == i) continue;
        swap(contours[i],contours[p]);
    }
    return;
}