#include <ros/ros.h>
#include "logical_camera_plugin/logical_camera_plugin.hh"

#include <gazebo/physics/Link.hh>
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/World.hh>
#include <gazebo/sensors/Sensor.hh>
#include <gazebo/sensors/SensorManager.hh>
#include "std_msgs/String.h"
#include "logical_camera_plugin/logicalImage.h"
void cameraMessageReceived(const logical_camera_plugin::logicalImage&msg) {
	ROS_INFO_STREAM(msg);
}
int main(int argc,char **argv) {
        ros::init(argc,argv,"treasures");
        ros::NodeHandle nh;
        ros::Subscriber scanSub = nh.subscribe("/objectDetected",1000,&cameraMessageReceived);
        ros::spin();
}
