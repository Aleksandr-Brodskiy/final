#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>
#include <iostream>
#include <nav_msgs/OccupancyGrid.h>
#include <geometry_msgs/Pose.h>

std::vector<int8_t> data (1,0);
unsigned int height = 0;
unsigned int width = 0;
float resolution = 0;
geometry_msgs::Pose origin;

int path_count = 0;
int dist = 1;



void mapReceived(const nav_msgs::OccupancyGrid&msg) {
	ROS_INFO_STREAM("received map");
	int length = ((sizeof(msg.data))/(sizeof(msg.data[0])));
	data.resize(length);

	for (int c = 0; c < length; c++) 
	{
		data.push_back(msg.data[c]);	
	}       
	height = msg.info.height;
	width = msg.info.width;
	resolution = msg.info.resolution;
	origin = msg.info.origin;
	ROS_INFO_STREAM(origin);
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "planner");
	ros::NodeHandle nh;
	ros::Publisher send = nh.advertise<geometry_msgs::Pose2D>("planner", 1000);
	ros::Subscriber subPose = nh.subscribe("map",1000,mapReceived);

	geometry_msgs::Pose2D pose;
	int i = 0;
	int count = 0;

	while (ros::ok)
	{
		while(data[i] != 0) //find first clear pixel
			i++;
		while(i < height*width) {
			if(data[i] == 0) //find a number of clear pixels in a row
				count++;	
			else
				count = 0;
			
			if(resolution*count >= dist) {
				pose.x = i%width;
				pose.y = ceil(i/width);
				//send.publish(pose);
			}
			
			
		}
		ros::spinOnce();

	}
}
