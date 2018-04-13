#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <stdlib.h>     /* srand, rand */
#include <sensor_msgs/LaserScan.h>

float range_min = 1;

void scanMessageReceived(const sensor_msgs::LaserScan&msg) {
	range_min = msg.range_min;
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "saferandomwalk");
	ros::NodeHandle n;

	double iSecret = 0;
	double jSecret = 0;

	ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("husky_velocity_controller/cmd_vel", 1000);
	ros::Subscriber scanSub = n.subscribe("/scan",1000,&scanMessageReceived);
	ros::Rate loop_rate(10);

	geometry_msgs::Twist msg;

	srand( time( NULL ) );

	int counter = 0;

	ros::Time startTime = ros::Time::now();
	ros::Duration duration = ros::Duration( rand() % 2 + 1 );
  	
	int toggle = 1;

  	while (ros::ok()) {
		if ( ( ros::Time::now().sec - startTime.sec ) >= duration.sec ) {
	  		startTime = ros::Time::now();

      			if (toggle) {
        			duration = ros::Duration( rand() % 2 + 1 );
        			toggle = 0;
        			iSecret = 0;
        			jSecret = ((double)(rand() % 5 - 2))/5;
      			} else {
        			duration = ros::Duration( rand() % 2 + 1 );
        			toggle = 1;
        			iSecret = ((double)(rand() % 5 - 2))/5;
        			jSecret = 0;
      			}
    		}
		
	  	msg.linear.x = iSecret;
	  	msg.angular.z = jSecret;
		if(range_min <= 0.2 && msg.linear.x > 0) {
			ROS_INFO_STREAM("Obstacle detected.");			
			msg.linear.x = 0;
		}
	  	chatter_pub.publish(msg);
		ROS_INFO_STREAM("Publishing " << msg);
	  	counter++;
		ros::spinOnce();
  	}

  	return 0;
}
