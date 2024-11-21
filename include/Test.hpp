#include <iostream>
#include <opencv2/opencv.hpp>

void hello()
{
    std::cout << "Hello,world!" << "\n";
    return;
}

void Test_video1()
{
    int width = 640;
    int heigth = 480;
    cv::VideoCapture cap;
	cap.open(0);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, width);    //设置宽度
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, heigth);  //设置长度
	cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));//视频流格式
	cap.set(cv::CAP_PROP_FPS, 30);//帧率 帧/秒

	if (!cap.isOpened())
	{
		std::cout << "打开失败" << "\n";
		return;
	}
	cv::Mat frame;
	cv::namedWindow("frame");//创建显示窗口


	while (1)
	{
		cap >> frame;//bool judge=cap.read(frame);
		if (frame.empty()) break;
		cv::imshow("frame", frame);
		if (27 == cv::waitKey(33))//"ESC"
			break;
	}
	cv::destroyWindow("frame");
	cap.release();
	return;
}

void Test_Camerapreview()
{
    cv::VideoCapture cap;
    cap.open(0); // 打开默认摄像头
    if (!cap.isOpened()){
        std::cout << "Fail to open Camera!" << "\n";
        return;
    }

    // 设置采集图片大小; 具体大小根据相机设置;
	// cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1024);
	// cap.set(cv::CAP_PROP_FRAME_WIDTH, 1024);

    // 生成新窗口
    cv::namedWindow("Video", 1);
    while(true)
    {
        cv::Mat img;
        cap >> img;
        cv::imshow("Video",img);
        if (cv::waitKey(1) != -1) break;
        
    }
    cap.release(); // 释放捕获器
    return; 
}

