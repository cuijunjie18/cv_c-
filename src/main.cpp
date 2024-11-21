#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Test.hpp"
#include "img_operator.hpp"

using namespace std;

/*收获：模版匹配一定要同种图片类型，如二级，BGR，gray*/

int main()
{
    // 读入要处理图片的路径
    string num_template_path = "images/ocr_a_reference.png";
    string card_path = "images/credit_card_02.png";

    // 读入模版并进行预处理
    cv::Mat num_template = cv::imread(num_template_path);
    cv::Mat num_template_gray; // 灰度化
    cv::cvtColor(num_template,num_template_gray,cv::COLOR_BGR2GRAY);
    cv::Mat num_template_thresh; // 二极化
    cv::threshold(num_template_gray,num_template_thresh,127,255,cv::THRESH_BINARY_INV);

    vector<vector<cv::Point>> contours; // 定义轮廓数组

    // 获取模版轮廓并排序
    cv::findContours(num_template_thresh,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    contours_sort(contours,false);

    // 测试轮廓
    //cv::Mat temp = num_template.clone();
    //for (int i = 0; i < contours.size(); i++){
    //    cv::drawContours(temp,contours,i,cv::Scalar(0,0,255),3);
    //    show("Contours_test",temp);
    //}

    // 提取每个数字模版
    vector<cv::Mat> nums;
    for (int i = 0; i < contours.size(); i++){
        cv::Rect rect_temp = cv::boundingRect(contours[i]);
        int x = rect_temp.x;
        int y = rect_temp.y;
        int w = rect_temp.width;
        int h = rect_temp.height;
        nums.emplace_back(num_template_thresh(cv::Range(y,y+h),cv::Range(x,x+w)));
    }

    //cout << nums[0].size() << " " << nums[0].rows << " " << nums[0].cols << "\n";

    // 模版图片统一大小
    for (int i = 0; i < contours.size(); i++){
        cv::resize(nums[i],nums[i],cv::Size(nums[0].cols,nums[0].rows));
    }

    // 读入待识别的证件，并进行预处理
    cv::Mat card,card_resize,card_gray,card_top,card_close,card_thresh;
    cv::Mat card_grad_x,card_grad_y,card_grad_xy,card_sobel;
    card = cv::imread(card_path);

    // 定义的形态学操作核的大小
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(4,4),cv::Point(-1,-1));
    cv::resize(card,card_resize,cv::Size(0,0),0.5,0.5);
    cv::cvtColor(card_resize,card_gray,cv::COLOR_BGR2GRAY);
    cv::morphologyEx(card_gray,card_top,cv::MORPH_TOPHAT,kernel);
    cv::Sobel(card_top,card_sobel,CV_16S,1,0);
    cv::convertScaleAbs(card_sobel,card_grad_x);
    cv::Sobel(card_top,card_sobel,CV_16S,0,1);
    cv::convertScaleAbs(card_sobel,card_grad_y);
    cv::addWeighted(card_grad_x,0.5,card_grad_y,0.5,0,card_grad_xy);
    //show("x",card_grad_x);
    //show("y",card_grad_y);
    //show("xy",card_grad_xy);
    cv::morphologyEx(card_grad_xy,card_close,cv::MORPH_CLOSE,kernel,cv::Point(-1,-1),2);
    cv::threshold(card_close,card_thresh,127,255,cv::THRESH_OTSU|cv::THRESH_BINARY);
    //show("close",card_close);
    //show("thresh",card_thresh);

    // 预处理完后寻找轮廓
    contours.clear();
    cv::findContours(card_thresh,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    vector<cv::Rect> ROI_rect;
    for (int i = 0; i < contours.size(); i++){
        cv::Rect rect_temp = cv::boundingRect(contours[i]);
        int x = rect_temp.x;
        int y = rect_temp.y;
        int w = rect_temp.width;
        int h = rect_temp.height;
        double check = (double)(w)/h;
        if (check >= 3.05 && check <= 3.20) ROI_rect.emplace_back(rect_temp);
    }
    sort(ROI_rect.begin(),ROI_rect.end(),cmp_rect); // 排序

    // 按顺序提取每个数字
    vector<cv::Mat> cut;
    for (int i = 0; i < ROI_rect.size(); i++){
        int x = ROI_rect[i].x;
        int y = ROI_rect[i].y;
        int w = ROI_rect[i].width;
        int h = ROI_rect[i].height;
        cv::Mat temp = card_resize(cv::Range(y,y+h),cv::Range(x,x+w));
        cv::resize(temp,temp,cv::Size(0,0),3,3);
        cv::cvtColor(temp,card_gray,cv::COLOR_BGR2GRAY);
        cv::threshold(card_gray,card_thresh,127,255,cv::THRESH_OTSU|cv::THRESH_BINARY);
        contours.clear();
        cv::findContours(card_thresh,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
        contours_sort(contours);
        // for (int i = 0; i < contours.size(); i++){
        //     cv::drawContours(temp,contours,i,cv::Scalar(0,0,255),3);
        //     show("Contours_test",temp);
        // }
        for (int j = 0; j < contours.size(); j++){
            cv::Rect rect_temp = cv::boundingRect(contours[j]);
            x = rect_temp.x;
            y = rect_temp.y;
            w = rect_temp.width;
            h = rect_temp.height;
            cut.emplace_back(temp(cv::Range(y,y+h),cv::Range(x,x+w)));
        }
    }

    // 对提取的单个数字进行处理
    for (int i = 0; i < cut.size(); i++){
        cv::cvtColor(cut[i],cut[i],cv::COLOR_BGR2GRAY);
        cv::threshold(cut[i],cut[i],127,255,cv::THRESH_OTSU|cv::THRESH_BINARY);
    }

    // 进行模版匹配
    vector<int> ans;
    int h = nums[0].rows; // 统一图像大小的模版
    int w = nums[0].cols;
    for (int i = 0; i < cut.size(); i++){
        double maxn_result = -99999;
        int match = -1;
        cv::resize(cut[i],cut[i],cv::Size(w,h));
        cv::Mat result;
        cv::Point minn_p,maxn_p;
        double minn,maxn;
        for (int j = 0; j <= 9; j++){
            cv::matchTemplate(cut[i],nums[j],result,cv::TM_CCOEFF_NORMED);
            cv::minMaxLoc(result,&minn,&maxn,&minn_p,&maxn_p);
            if (i == 1) cout << j << " " << maxn << "\n";
            if (maxn > maxn_result){
                maxn_result = maxn;
                match = j;
            }
        }
        ans.emplace_back(match);
    }
    for (auto to : ans) cout << to;
    cout << "\n";
    return 0;
}

// 3.05 ~ 3.20