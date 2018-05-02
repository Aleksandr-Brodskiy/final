#include <ros/ros.h>
#include "std_msgs/String.h"
#include "../../../devel/include/logical_camera_plugin/logicalImage.h"
#include <string>
#include<vector>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>

std::map<std::string, logical_camera_plugin::logicalImage> treasures;
int treasure_count = 0;
std::vector<logical_camera_plugin::logicalImage> treasureArray;
bool foundTreasure = false;
logical_camera_plugin::logicalImage treasure;
geometry_msgs::TransformStamped newLocation;
geometry_msgs::PoseWithCovarianceStamped amclLoc;

void amcl(const geometry_msgs::PoseWithCovarianceStamped &msg){
	amclLoc = msg;
	//if(foundTreasure){
	        //ROS_INFO_STREAM(msg);
	//	foundTreasure = false;
	//}
}

void cameraMessageReceived(const logical_camera_plugin::logicalImage&msg) {
        //treasures[msg.modelName] = msg;
		bool found = false;
		for(int i = 0; i < treasureArray.size(); i++){
			if(treasureArray[i].modelName == msg.modelName){
				//already exists ignore treasure
				found = true;
				ROS_INFO_STREAM("Treasure already found...Ignoring");
			}
		}
		if(!found){
			//foundTreasure = true;
			treasure = msg;
			//IF USING TRANSFORMATIONS
			//treasure.pose_pos_x = msg.pose_pos_x; //+ newLocation.transform.translation.x;
			//treasure.pose_pos_y = msg.pose_pos_y; //+ newLocation.transform.translation.y;

			//IF USING AMCL
			treasure.pose_pos_x = msg.pose_pos_x + amclLoc.pose.pose.position.x;
                        treasure.pose_pos_y = msg.pose_pos_y + amclLoc.pose.pose.position.y;
			ROS_WARN("FOUND A TREASURE");
			ROS_INFO_STREAM(treasure);
			treasureArray.push_back(treasure);
		}
	/*
        if(treasure_count != treasures.size()) {//found a new treasure
                treasure_count = treasures.size();
                ROS_INFO_STREAM("Found " << treasure_count << " treasure(s)");
                ROS_INFO_STREAM(msg);
	}
	*/
}

int main(int argc,char **argv) {
        ros::init(argc,argv,"treasures");
        ros::NodeHandle nh;


        tf2_ros::Buffer buffer;
        tf2_ros::TransformListener listener(buffer);
        geometry_msgs::TransformStamped transformStamped;

        ros::Subscriber amclSub = nh.subscribe("amcl_pose",1000,&amcl);
        ros::Subscriber scanSub = nh.subscribe("/objectsDetected",1000,&cameraMessageReceived);
        ros::Time time = ros::Time(0);
        while(ros::ok()){
                try{
                        transformStamped = buffer.lookupTransform("map", "logical_camera_link",time, ros::Duration(1));
			//In case we wish to use transformations instead of AMCL
			//newLocation = transformStamped;
                }catch (tf2::TransformException &ex) {
                        //ROS_WARN("%s",ex.what());
			ROS_WARN("LETS HOPE WE CAN FIND SOME TREASURES :)");
                }
        //ROS_INFO_STREAM("Obtained transformation " << transformStamped);
	ros::spinOnce();
	}
}

