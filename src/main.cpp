#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

// 展示函数
void show(cv::Mat img,string name = "Test"){
    cv::imshow(name,img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

// 返回待识别物体颜色的HSV上下界
void cvt_hsv(cv::Mat hsv,cv::Scalar &l_hsv,cv::Scalar &h_hsv){
    //cout << hsv << "\n";
    int b = hsv.at<cv::Vec3b>(0,0)[0];
    l_hsv = cv::Scalar(max(0,b - 10),100,100);
    h_hsv = cv::Scalar(min(b + 10,255),255,255);
}

// 对mask二级图找轮廓
void track(cv::Mat &mask,cv::Mat &img){
    cv::GaussianBlur(mask,mask,cv::Size(3,3),0); // 滤波平滑处理，便于轮廓的平滑连接
    vector<vector<cv::Point>> contours;
    cv::findContours(mask,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    int object_pos;
    double maxn = 0;
    for (int i = 0; i < contours.size(); i++){
        double temp = cv::contourArea(contours[i]);
        if (temp > maxn){
            maxn = temp;
            object_pos = i;
        }
    }

    if (contours.size() == 0) return;
    
    // 计算目标轮廓的外接矩形，以矩形中心为轮廓中心
    cv::Rect rect_object = cv::boundingRect(contours[object_pos]);
    cv::rectangle(img,rect_object,cv::Scalar(0,0,255));
    int cx = (rect_object.x + rect_object.width / 2);
    int cy = (rect_object.y + rect_object.height / 2);
    cv::circle(img,cv::Point(cx,cy),10,cv::Scalar(0,0,255),-1);
}

// debug用
void check(cv::Scalar l_hsv,cv::Scalar h_hsv,cv::Scalar track_color){
    cout << l_hsv << "\n";
    cout << h_hsv << "\n";
    cv::Mat test(500,500,CV_8UC3,track_color);
    cv::Mat hsv,mask;
    cv::cvtColor(test,hsv,cv::COLOR_BGR2HSV);
    cv::inRange(hsv,l_hsv,h_hsv,mask); // 根据阈值生成对应掩膜
    cv::imshow("Debug",test);
    cv::imshow("Mask",mask);
    cv::waitKey(0);
    cv::destroyAllWindows();
    exit(0);
}

int main()
{
    freopen("track_color.txt","r",stdin);
    cv::Mat img;
    cv::VideoCapture cap;
    cap.open(0);
    int width = 640;
    int height = 480;
    cap.set(cv::CAP_PROP_FRAME_WIDTH,width);
    cap.set(cv::CAP_PROP_FPS,30);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,height);

    if (!cap.isOpened()){
        cout << "Fail to open!" << "\n";
    }

    // 创建待识别颜色的BGR单像素值
    cv::Scalar track_color;
    cv::Mat hsv,mask,res;
    cv::Scalar l_hsv,h_hsv;
    for (int i = 0; i <= 2; i++) cin >> track_color[i];
    cv::Mat track_color_single(1,1,CV_8UC3,track_color); // 注意宏定义的常量不需要命名空间
    cv::cvtColor(cv::Mat(1,1,CV_8UC3,track_color),hsv,cv::COLOR_BGR2HSV);
    cvt_hsv(hsv,l_hsv,h_hsv);

    // check(l_hsv,h_hsv,track_color);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5),cv::Point(-1,-1));

    while (true){
        cap >> img;
        cv::cvtColor(img,hsv,cv::COLOR_BGR2HSV); // 原图转换颜色空间到HSV
        cv::inRange(hsv,l_hsv,h_hsv,mask); // 根据阈值生成对应掩膜
        // cv::bitwise_and(img,img,res,mask); // 用掩膜对原图进行处理(展示色域，若不需要，不用)
        
        cv::morphologyEx(mask,mask,cv::MORPH_DILATE,kernel,cv::Point(-1,-1),4); // 膨胀操作
        track(mask,img);
        cv::imshow("origin",img);
        // cv::imshow("hsv",hsv);
        // cv::imshow("mask",mask);
        // cv::imshow("res",res);
        res = cv::Mat(res.rows,res.cols,res.type(),cv::Scalar(0,0,0)); // 清空res图像，否则bitwis_and运算会累加
        if (cv::waitKey(1) == 27) break;
    }
    cv::destroyAllWindows();
    cap.release();
    return 0;
}

/*绿飞镖
[109, 164, 123]
[118, 166, 138]
[61, 162, 77]
[106, 186, 144]
[95, 175, 126]
[90, 216, 148]
[155, 211, 193]
[154, 213, 194]
[87, 217, 144]
100,187,144
*/

/*红盖
[73, 29, 174]
[67, 31, 165]
[61, 24, 156]
[63, 27, 155]
[62, 24, 164]
[75, 34, 183]
*/

/*黄盖
[75, 155, 186]
[90, 176, 211]
[70, 156, 191]
[68, 151, 181]
[64, 150, 185]
*/


