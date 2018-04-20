#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Quaternion.h>
#include <tf/transform_datatypes.h>

float x = 0;
float y = 0;
float theta = 0;
bool newGoal = false;

void planReceived(const geometry_msgs::Pose2D&msg) {
        ROS_INFO_STREAM("x: " << msg.x << " y: " << msg.y << " theta: " << msg.theta);
        x = msg.x;
        y = msg.y;
        theta = msg.theta;
	newGoal = true;
}

void serviceActivated() {
    	ROS_INFO_STREAM("Service received goal");
}

void serviceDone(const actionlib::SimpleClientGoalState& state,
		 const move_base_msgs::MoveBaseResultConstPtr& result) {
    	ROS_INFO_STREAM("Service completed");
    	ROS_INFO_STREAM("Final state " << state.toString().c_str());
	newGoal = false;
}

void serviceFeedback(const move_base_msgs::MoveBaseFeedbackConstPtr& fb) {
   	ROS_INFO_STREAM("Service still running");
	ROS_INFO_STREAM("Current pose (x,y) " <<
		    fb->base_position.pose.position.x << "," <<
		    fb->base_position.pose.position.y);
}

int main(int argc,char ** argv) {
	ros::init(argc,argv,"nav");
        ros::NodeHandle nh;
        ros::Subscriber subPose = nh.subscribe("planner",1000,planReceived);

        actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>
	ac("move_base",true);
	ROS_INFO_STREAM("Waiting for server to be available...");
    	while (!ac.waitForServer()) {
    	}
    	ROS_INFO_STREAM("done!");
	while(!newGoal)
		ros::spinOnce();
    	move_base_msgs::MoveBaseGoal goal;

	goal.target_pose.header.frame_id = "map";
    	goal.target_pose.header.stamp = ros::Time::now();
    	goal.target_pose.pose.position.x = x;
    	goal.target_pose.pose.position.y = y;
	geometry_msgs::Quaternion q;
    	tf::quaternionTFToMsg(tf::createQuaternionFromRPY(0,0,theta), q);
	goal.target_pose.pose.orientation = q;
    	ac.sendGoal(goal,&serviceDone,&serviceActivated,&serviceFeedback);
	while(ros::ok()) {
		if(ac.waitForResult()){
			while(!newGoal)
				ros::spinOnce();
		    	move_base_msgs::MoveBaseGoal goal;

			goal.target_pose.header.frame_id = "map";
		    	goal.target_pose.header.stamp = ros::Time::now();
		    	goal.target_pose.pose.position.x = x;
		    	goal.target_pose.pose.position.y = y;
			geometry_msgs::Quaternion q;
		    	tf::quaternionTFToMsg(tf::createQuaternionFromRPY(0,0,theta), q);
			goal.target_pose.pose.orientation = q;
		    	ac.sendGoal(goal,&serviceDone,&serviceActivated,&serviceFeedback);
	    	} 
	}
    	
    	return 0;

}
