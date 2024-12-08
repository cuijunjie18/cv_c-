#include "Pose_calculator.hpp"
#include "objective_track.hpp"

cv::Point2d pixel2cam ( const cv::Point2d &p, const cv::Mat &K ){
    return cv::Point2d
           (
               ( p.x - K.at<double> ( 0,2 ) ) / K.at<double> ( 0,0 ),
               ( p.y - K.at<double> ( 1,2 ) ) / K.at<double> ( 1,1 )
           );
}

void my_Pnp(Eigen::Vector3d pts_c_solve[5],cv::Mat img){
    /*step1 : 接收图片*/
    // string img_path = ""; // 图片路径
    std::ifstream f; // 文件读取流
    // f.open("image_path.txt");
    // f >> img_path;
    // f.close();
    // cv::Mat img = cv::imread(img_path);

    /*step2 : 设置目标物体的颜色阈值*/
    cv::Scalar l_hsv,h_hsv,track_color;
    f.open("/home/cjj/Desktop/RM_work/opencv_project/track_and_pnp/src/objective_track/config/yellow.txt");
    for (int i = 0; i < 3; i++) f >> track_color[i];
    f.close();
    cv::Mat hsv(1,1,CV_8UC3,track_color);
    cv::cvtColor(hsv,hsv,cv::COLOR_BGR2HSV);
    cvt_hsv(hsv,l_hsv,h_hsv);

    /*step3 : 获取目标检测结果*/
    std::vector<cv::Point2d> pts(5); // 五个相机坐标下的二维像素特征点
    track(l_hsv,h_hsv,img,pts);
    // std::string debug_check;
    // if (argc != 1) debug_check = argv[1];
    // if (debug_check == "debug"){
    //     cout << "Center Point:" << pts[0].x << " " << pts[0].y << "\n";
    //     for (int i = 1; i <= 4; i++) cout << pts[i].x << " " << pts[i].y << "\n";
    //     show(img);
    // }

    /*step4 : 设置Pnp解算的前置参数*/

    // 内参矩阵
    cv::Mat K = cv::Mat_<double>(3,3);
    f.open("/home/cjj/Desktop/RM_work/opencv_project/track_and_pnp/src/objective_track/config/K_Matrix.txt");
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++) f >> K.at<double>(i,j);
    }
    f.close();

    std::vector<cv::Point3d> pts_w(5); // 目标物体的世界坐标
    std::vector<cv::Point3d> pts_c(5); // 目标物体的相机坐标(cv版)
    std::vector<Eigen::Vector3d> pts_ce(5); // 目标物体的相机坐标(Eigen版)
    std::vector<cv::Point2d> pts_2d_normal(5); // Pnp-3d-2d解算需要的二维像素坐标归一化坐标

    f.open("/home/cjj/Desktop/RM_work/opencv_project/track_and_pnp/src/objective_track/config/objective_world_pose.txt");
    // 读取世界坐标
    for (int i = 0; i < 5; i++){
        f >> pts_w[i].x >> pts_w[i].y >> pts_w[i].z;
    }
    f.close();
    // 计算归一化坐标
    for (int i = 0; i < 5; i++){
        // pts_2d_normal.emplace_back(pixel2cam(pts[i],K)); // 错误用法，定义了(5)，再emplack_back()就是6开始了
        pts_2d_normal[i] = pixel2cam(pts[i],K);
        // cout << pts_2d_normal[i] << "\n";
    }

    /*step5 : 利用3d-2d匹配对进行Pnp解算相机外参R,t*/
    cv::Mat R,t;
    cv::solvePnP(pts_w,pts_2d_normal,K,cv::Mat(),R,t,false); // Pnp解算
    cv::Rodrigues(R,R); // R为旋转向量的形式，转化为旋转矩阵
    // cout << R << "\n";

    /*step6 : 利用R,t对目标物体的相机坐标进行求解*/

    /*方法一：cv的坐标变化函数*/
    cv::transform(pts_w,pts_c,R); // 旋转变化
    for (int i = 0; i < 5; i++){
        pts_c[i].x += t.at<double>(0);
        pts_c[i].y += t.at<double>(1);
        pts_c[i].z += t.at<double>(2);
    }// 平移变化
    // cout << pts_c << "\n\n";

    /*方法二：利用Eigen(具有矩阵计算的优良性能)*/
    Eigen::Matrix3d R_m;
    Eigen::Vector3d t_m;
    std::vector<Eigen::Vector3d> pts_we(5);
    cv::cv2eigen(R,R_m);
    cv::cv2eigen(t,t_m);
    for (int i = 0; i < 5; i++){
        cv::cv2eigen(cv::Mat(pts_w[i]),pts_we[i]);
        pts_c_solve[i] = R_m * pts_we[i] + t_m;
    }

    // !!!!经验算，两种方法的解算结果一样，但是我们偏向于用Eigen,因为其矩阵库更齐全
    
    /*step7 : 将pts_we坐标发送出去！*/
}