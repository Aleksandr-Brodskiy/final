#include <ros/ros.h>
#include "std_msgs/String.h"
#include "../../../devel/include/logical_camera_plugin/logicalImage.h"
void cameraMessageReceived(const logical_camera_plugin::logicalImage&msg) {
	ROS_INFO_STREAM(msg);
}
int main(int argc,char **argv) {
        ros::init(argc,argv,"treasures");
        ros::NodeHandle nh;
        ros::Subscriber scanSub = nh.subscribe("/objectsDetected",1000,&cameraMessageReceived);
	while(ros::ok())
        	ros::spin();
}
