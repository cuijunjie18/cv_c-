#include "objective_track.hpp"

// 展示函数
/*注意：头文件中声明的是有默认参数的，在实际写的时候不能加*/
void show(cv::Mat img,std::string name){
    cv::imshow(name,img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

// 返回待识别物体颜色的HSV上下界
void cvt_hsv(cv::Mat hsv,cv::Scalar &l_hsv,cv::Scalar &h_hsv){
    //cout << hsv << "\n";
    int b = hsv.at<cv::Vec3b>(0,0)[0];
    l_hsv = cv::Scalar(std::max(0,b - 10),100,100);
    h_hsv = cv::Scalar(std::min(b + 10,255),255,255);
}

// 对mask二级图找轮廓
void track(cv::Scalar l_hsv,cv::Scalar h_hsv,cv::Mat &img,std::vector<cv::Point2d> &pts){
    /*step1 : 生成mask*/
    cv::Mat mask,hsv;
    cv::cvtColor(img,hsv,cv::COLOR_BGR2HSV);
    cv::inRange(hsv,l_hsv,h_hsv,mask); // 生成对应色域的掩膜mask(二极图)
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5),cv::Point(-1,-1)); // 建立形态学kernel
    cv::morphologyEx(mask,mask,cv::MORPH_DILATE,kernel,cv::Point(-1,-1),4); // 对mask迭代膨胀，增大色域面积

    /*step2 : 查找二极图轮廓contours*/
    cv::GaussianBlur(mask,mask,cv::Size(3,3),0); // 滤波平滑处理，便于轮廓的平滑连接
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    int object_pos; // 最大面积轮廓下标
    double maxn = 0;
    for (int i = 0; i < contours.size(); i++){
        double temp = cv::contourArea(contours[i]);
        if (temp > maxn){
            maxn = temp;
            object_pos = i;
        }
    }

    if (contours.size() == 0) return; // 没有轮廓，认为img中无目标物体，直接返回
    
    /*step3 : 建立目标检测在img上的结果*/

    // 计算目标轮廓的外接矩形，以矩形中心为轮廓中心
    cv::Rect rect_object = cv::boundingRect(contours[object_pos]);
    cv::rectangle(img,rect_object,cv::Scalar(0,0,255));
    int cx = (rect_object.x + rect_object.width / 2);
    int cy = (rect_object.y + rect_object.height / 2);
    cv::circle(img,cv::Point(cx,cy),10,cv::Scalar(0,0,255),-1);

    /*step4 : 建立目标检测结果在pts上的结果*/
    pts[0].x = cx,pts[0].y = cy; // 中心点
    pts[1].x = rect_object.x,pts[1].y = rect_object.y;
    pts[2].x = rect_object.x + rect_object.width,pts[2].y = rect_object.y;
    pts[3].x = rect_object.x + rect_object.width,pts[3].y = rect_object.y + rect_object.height;
    pts[4].x = rect_object.x,pts[4].y = rect_object.y + rect_object.height;

    // std::cout << "Center Point:" << pts[0].x << " " << pts[0].y << "\n";
}

// debug用
void check(cv::Scalar l_hsv,cv::Scalar h_hsv,cv::Scalar track_color){
    std::cout << l_hsv << "\n";
    std::cout << h_hsv << "\n";
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