#include "objective_track.hpp"
#include "Pose_calculator.hpp"
#include "my_interfaces/msg/position.hpp"
#include <rclcpp/rclcpp.hpp>

class objective_track : public rclcpp::Node
{
	public:
		objective_track (std::string name) : Node(name){
			RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());

			points_detector = this->create_publisher<my_interfaces::msg::Position>(
				"points_detection",10
			);

			Track();
		}

	private:
		cv::Mat img;
    	Eigen::Vector3d pts[5];
		rclcpp::Publisher<my_interfaces::msg::Position>::SharedPtr points_detector;

		void Track(){
			 // 打开摄像头
    		cv::VideoCapture cap;
    		cap.open(0);
    		int width = 640;
    		int height = 480;
    		cap.set(cv::CAP_PROP_FRAME_WIDTH,width);
    		cap.set(cv::CAP_PROP_FPS,30);
    		cap.set(cv::CAP_PROP_FRAME_HEIGHT,height);

    		cv::Mat img;
    		while (true){
    		    cap >> img;
    		    my_Pnp(pts,img);

				// 构造发布数据
				my_interfaces::msg::Position detection_msg;
				detection_msg.x = pts[0][0];
				detection_msg.y = pts[0][2];
				detection_msg.z = pts[0][1];
				points_detector->publish(detection_msg);

    		    cv::imshow("Track",img);
    		    if (cv::waitKey(1) == 27) break;

    		    /*pts[0]为center
    		      pts[0][0],pts[0][1],pts[0][2] 为相机坐标系下的x,y,z
    		      注意相机坐标系下的x,y,z与常见的正交坐标系不同
    		      x正方向朝视野的右端，y正方向朝视野的下端，z正方向朝视野的前面
    		    */

    		    // std::cout << pts[0][0] << " " << pts[0][2] << " " << -pts[0][1] << "\n"; // 目标物体的正交中心位置
    		}
			cap.release();
		}	
};

int main(int argc,char** argv)
{
	rclcpp::init(argc,argv);
	auto node = std::make_shared<objective_track>("cjj_detector");
	rclcpp::spin(node);
	rclcpp::shutdown();
    return 0;
}