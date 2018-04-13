# include <ros/ros.h>
# include <nav_msgs/GetPlan.h>
# include <geometry_msgs/Pose2D.h>
# include <sensor_msgs/LaserScan.h>
# include <tf2/LinearMath/Quaternion.h>
# include <move_base_msgs/MoveBaseAction.h>
# include <tf2_geometry_msgs/tf2_geometry_msgs.h>
# include <actionlib/client/simple_action_client.h>

bool shouldMove = true;
int counter = 0;
void readSensor(const sensor_msgs::LaserScan&msg){
	int num = 0;
    for (int i = 0; i < (msg.ranges.size() - 1); i++){
        if (msg.ranges[i] < 0.25){
		num++;
		if(num > 3){
			//more than one range is close so it shouldnt be random data
            		shouldMove = false;
            		break;
		}
        }
    }
}



void goalLive(const move_base_msgs::MoveBaseFeedbackConstPtr& fb){
	if (counter == 10){
		ROS_INFO_STREAM("Current position: (x: " << fb->base_position.pose.position.x << ", y: " << fb->base_position.pose.position.y << ")");
        	counter = -1;
    	}
    	counter++;
}

void goalAssigned(){
	ROS_INFO_STREAM("Random goal assigned");
}

void goalReached(const actionlib::SimpleClientGoalState& state, const move_base_msgs::MoveBaseResultConstPtr& result){
	ROS_INFO_STREAM("Random goal reached");
}

int main (int argc, char **argv) {
	ros::init(argc, argv, "saferandomwalk");
	ros::NodeHandle nh;
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> goToGoal("move_base", true);
	ros::Subscriber sub = nh.subscribe("/scan",1000,&readSensor);
	tf2::Quaternion quaternion;
    	ros::Rate rate(1);

  	while (ros::ok()){

	//Make sure that the robot should still move
	if(shouldMove){
		move_base_msgs::MoveBaseGoal goal;
		goal.target_pose.header.frame_id = "map";
		goal.target_pose.header.stamp = ros::Time::now();
		quaternion.setRPY ( 0 , 0 , ((double)(rand() % 5))/3);
		goal.target_pose.pose.position.x = ((double)(rand() % 5));
		goal.target_pose.pose.orientation = tf2::toMsg(quaternion);
		goToGoal.sendGoal(goal, &goalReached, &goalAssigned, &goalLive);
        	ros::spinOnce();

        }else{
	   	//ROBOT HAS TO STOP I.E close by to obstacle
		ROS_WARN("HAS STOPPED DUE TO AN OBSTACLE!");
            	goToGoal.cancelAllGoals();
            	rate.sleep();
        }
        rate.sleep();
	}//ends while

 	return 0;

}
