#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>
#include <iostream>
#include <nav_msgs/OccupancyGrid.h>
#include <geometry_msgs/Pose.h>
#include <iterator>

//std::vector<int8_t> data (1,0);
int8_t data[800][800] = {0};
unsigned int height = 0;
unsigned int width = 0;
float resolution = 0;
geometry_msgs::Pose origin;

int path_count = 0;
int dist = 1;
bool first = true;



void mapReceived(const nav_msgs::OccupancyGrid&msg) {

	       
	height = msg.info.height;
	width = msg.info.width;
	resolution = msg.info.resolution;
	origin = msg.info.origin;
	int length = height*width;

	for (int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			data[i][j] = msg.data[i*width+j];
			//ROS_INFO_STREAM("data[" << i*width+j << "]=" << (int)msg.data[i*width+j]);	
		}
	}
	ROS_INFO_STREAM(origin);
	ROS_INFO_STREAM("received map of height " << height << " width " << width << " length " << length);
	//ROS_INFO_STREAM(msg);
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "planner");
	ros::NodeHandle nh;
	ros::Publisher send = nh.advertise<geometry_msgs::Pose2D>("planner", 1000);
	ros::Subscriber subPose = nh.subscribe("map",1000,mapReceived);

	geometry_msgs::Pose2D pose;
	int row_count = 0;
	int col_count = 0;

	while (ros::ok) {
		for(int i = 0; i < height; i++) {
			if(i%2 == 0) {
				for(int j = 0; j < width; j++) {
					int row = i;
					while(row >= 0 && data[row][j] == 0) {
						row--;
						row_count++;
					}
					if(row_count*resolution >= dist && col_count*resolution >= dist) {
						//send point
						pose.x = j*resolution + origin.position.x;
						pose.y = i*resolution + origin.position.y;
						pose.theta = 0;
						if(first) {
						send.publish(pose);
						first = false;
						}
						ROS_INFO_STREAM("Publishing pose: " << pose);
						col_count = 0;
					} else if(data[i][j] == 0)
						col_count++;
					else
						col_count = 0;
				}
			} else {
				for(int j = width-1; j >= 0; j--) {
					int row = i;
					while(row >= 0 && data[row][j] == 0) {
						row--;
						row_count++;
					}
					if(row_count*resolution >= dist && col_count*resolution >= dist) {
						//send point
						pose.x = (i%width)*resolution + origin.position.x;
						pose.y = (ceil(i/width))*resolution + origin.position.y;
						pose.theta = 0;
						send.publish(pose);
						col_count = 0;
					} else if(data[i][j] == 0)
						col_count++;
					else
						col_count = 0;
				}
			}
	
		}
		
		ros::spinOnce();

	}
}