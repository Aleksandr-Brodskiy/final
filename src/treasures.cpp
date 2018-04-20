#include <ros/ros.h>
#include "std_msgs/String.h"
#include "../../../devel/include/logical_camera_plugin/logicalImage.h"
#include <string>

std::map<std::string, logical_camera_plugin::logicalImage> treasures;
int treasure_count = 0;

void cameraMessageReceived(const logical_camera_plugin::logicalImage&msg) {
	treasures[msg.modelName] = msg;
	if(treasure_count != treasures.size()) {//found a new treasure
		treasure_count = treasures.size();
		
		ROS_INFO_STREAM("Found " << treasure_count << " treasure(s)");
		ROS_INFO_STREAM(msg);	
	}
}

int main(int argc,char **argv) {
        ros::init(argc,argv,"treasures");
        ros::NodeHandle nh;
        ros::Subscriber scanSub = nh.subscribe("/objectsDetected",1000,&cameraMessageReceived);
	while(ros::ok())
        	ros::spin();
}
