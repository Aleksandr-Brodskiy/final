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
double x_dist = 1;
double y_dist = 2;
double obs_dist = 1;
bool first = true;
bool map = false;
bool odd = true;


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
	map = true;
	//ROS_INFO_STREAM(msg);
}

bool clearPoint(int i, int j) {
	if(i - obs_dist/resolution < 0 || j - obs_dist/resolution < 0 || i + obs_dist/resolution > height || j + obs_dist/resolution > width)
		return false;
	for(int row = i - obs_dist/resolution; row < i + obs_dist/resolution; row++)
		if(data[row][j] != 0)
			return false;
	for(int col = j - obs_dist/resolution; col < j + obs_dist/resolution; col++)
		if(data[i][col] != 0)
			return false;
	return true;
	
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
	int last_row = 0;

	while (ros::ok && !map) { //wait for map
		ros::spinOnce();
	}
	for(int i = 0; i < height; i++) {
		if(odd) {
			for(int j = 0; j < width; j++) {
				if(clearPoint(i,j)) {
					pose.x = j*resolution + origin.position.x;
					pose.y = i*resolution + origin.position.y;
					pose.theta = 0;
					send.publish(pose);
					//ros::Rate(1).sleep();
					first = false;
					ROS_INFO_STREAM("Publishing pose: " << pose);
					j += x_dist/resolution;

				}
			}
			i += y_dist/resolution;
			odd = false;
		} else {
			for(int j = width-1; j >= 0; j--) {
				if(clearPoint(i,j)) {
					pose.x = j*resolution + origin.position.x;
					pose.y = i*resolution + origin.position.y;
					pose.theta = M_PI;
					send.publish(pose);
					//ros::Rate(1).sleep();
					first = false;
					ROS_INFO_STREAM("Publishing pose: " << pose);
					j -= x_dist/resolution;

				}
			}
			i += y_dist/resolution;
		}

	}
		
}
