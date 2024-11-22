#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

void track();

int main()
{
    cv::VideoCapture cap;
    cap.open(0);
    int width = 640,height = 480;
    cout << cap.get(cv::CAP_PROP_FRAME_WIDTH) << "\n";
    cout << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << "\n";
    cout << cap.get(cv::CAP_PROP_FPS) << "\n";

    cap.set(cv::CAP_PROP_FRAME_WIDTH,640);
    cap.set(cv::CAP_PROP_FPS,30);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,480);

    cout << cap.get(cv::CAP_PROP_FPS) << "\n";


    if (!cap.isOpened())
    {
        cout << "Fail to open!" << "\n";
        return 0;
    }

    cv::Mat img;
    string save_path = "/home/cjj/Desktop/RM_work/objective_track/saves/save.png";
    while (true){
        cap >> img;
        cv::imshow("Test",img);
        int key = cv::waitKey(1);
        if (key == 27) break;
        if (key == 'q'){
            cv::imwrite(save_path,img);
            break;
        }
    }
    cap.release();
    return 0;
}