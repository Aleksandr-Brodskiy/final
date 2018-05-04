#include <ros/ros.h>
#include "std_msgs/String.h"
#include "../../../devel/include/logical_camera_plugin/logicalImage.h"
#include <string>
#include<vector>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>

std::map<std::string, geometry_msgs::Pose> treasures;
int treasure_count = 0;
geometry_msgs::TransformStamped transformStamped;

void cameraMessageReceived(const logical_camera_plugin::logicalImage&msg) {
	
        if(treasures.count(msg.modelName) == 0) {//found a new treasure

		geometry_msgs::Pose treasurePose;

		treasurePose.position.x = msg.pose_pos_x + transformStamped.transform.translation.x;
		treasurePose.position.y = msg.pose_pos_y + transformStamped.transform.translation.y;
		treasurePose.position.z = msg.pose_pos_z + transformStamped.transform.translation.z;
		treasurePose.orientation.x = transformStamped.transform.rotation.x;
		treasurePose.orientation.y = transformStamped.transform.rotation.y;
		treasurePose.orientation.z = transformStamped.transform.rotation.z;
		treasurePose.orientation.w = transformStamped.transform.rotation.w;

		treasures[msg.modelName] = treasurePose;
                ROS_INFO_STREAM("Found " << treasures.size() << " treasure(s)");
		ROS_INFO_STREAM("modelName: " << msg.modelName);
                ROS_INFO_STREAM(treasurePose);
	}
	
}

int main(int argc,char **argv) {
        ros::init(argc,argv,"treasures");
        ros::NodeHandle nh;

        tf2_ros::Buffer buffer;
        tf2_ros::TransformListener listener(buffer);

        ros::Subscriber scanSub = nh.subscribe("/objectsDetected",1000,&cameraMessageReceived);

        while(ros::ok()){
                try{
                        transformStamped = buffer.lookupTransform("map", "logical_camera_link",ros::Time(0), ros::Duration(1));
                }catch (tf2::TransformException &ex) {
                        //ROS_WARN("%s",ex.what());
			ROS_WARN("LETS HOPE WE CAN FIND SOME TREASURES :)");
                }

		ros::spinOnce();
	}
}

