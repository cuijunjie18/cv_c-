#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// 展示图像的函数
void show(Mat img, string title = "Test") {
    imshow(title, img);
    waitKey(0);
    destroyAllWindows();
}

// 根据给定的HSV图像确定追踪的HSV上下界
void cvt_hsv(Mat hsv, Scalar &l_hsv, Scalar &h_hsv) {
    int b = hsv.at<Vec3b>(0, 0)[0];  // 获取HSV图像中的Hue值
    l_hsv = Scalar(b - 30, 100, 100);  // HSV范围的下界
    h_hsv = Scalar(b + 30, 255, 255);  // HSV范围的上界
}

// 基于轮廓查找的图像追踪
void Track(Mat origin, Mat mask) {
    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);  // 查找轮廓
    drawContours(origin, contours, -1, Scalar(0, 255, 0), 5);  // 在原图上绘制轮廓
    show(origin);
}

int main() {
    // 定义要追踪的颜色（BGR格式）
    Scalar track_color(71, 31, 200);  // BGR格式的颜色
    Mat hsv;
    cvtColor(Mat(1, 1, CV_8UC3, track_color), hsv, COLOR_BGR2HSV);  // 转换为HSV

    Scalar l_hsv, h_hsv;
    cvt_hsv(hsv, l_hsv, h_hsv);  // 计算HSV范围
    cout << "HSV: " << hsv << endl;
    cout << "Lower HSV bound: " << l_hsv << endl;
    cout << "Upper HSV bound: " << h_hsv << endl;

    // 假设你有一个原始图像和一个已经创建的mask，调用Track函数
    // Mat image = imread("your_image.jpg");  // 加载图像
    // Mat mask = inRange(image, l_hsv, h_hsv);  // 创建mask
    
    // 这里可以用图像和mask进行追踪，以下是一个简单的示例
    // Track(image, mask);

    return 0;
}
