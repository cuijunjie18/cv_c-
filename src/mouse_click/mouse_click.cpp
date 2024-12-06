#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

using namespace std;

vector<cv::Point> point_list;

void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {  // 左键点击事件
        std::cout << "Mouse Clicked at (" << x << ", " << y << ")" << std::endl;
        point_list.emplace_back(cv::Point(x,y));
    }
}

int main()
{
    cv::Mat img = cv::imread("images/yellow.png");
    cv::namedWindow("Point Coordinates");
    cv::setMouseCallback("Point Coordinates",mouseCallback);
    while (true){
        cv::imshow("Point Coordinates",img);
        if (cv::waitKey(1) == 27) break;
    }
    for (auto p : point_list){
        int x = p.x;
        int y = p.y;
        cout << img.at<cv::Vec3b>(y,x) << "\n";
    }
    return 0;
}