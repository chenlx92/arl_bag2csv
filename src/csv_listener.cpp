#include <fstream>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <boost/foreach.hpp>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TransformStamped.h> 
#include <geometry_msgs/Vector3.h> 
#include <rosgraph_msgs/Log.h>
#include <tf/tf.h>
#include <sensor_msgs/Imu.h>

//makes csv from streaming data, use this program simultaniously with experiment or bag replaying

geometry_msgs::TransformStamped quad1_msg_keep;
geometry_msgs::TransformStamped quad2_msg_keep;
std_msgs::Float32MultiArray state_keep;
/*
state_keep.data[0]=0;
state_keep.data[1]=0;
state_keep.data[2]=0;
state_keep.data[3]=0;
state_keep.data[4]=0;
state_keep.data[5]=0;
state_keep.data[6]=0;
state_keep.data[7]=0;
state_keep.data[8]=0;
*/
geometry_msgs::Vector3 joy_keep;

geometry_msgs::Vector3 cmd_keep;

int had_message =0;


void quad1_cb(const geometry_msgs::TransformStamped& pose_msg)
{
had_message =1;
  quad1_msg_keep=pose_msg;
}

void quad2_cb(const geometry_msgs::TransformStamped& pose_msg)
{
had_message =1;
  quad2_msg_keep=pose_msg;
}

void state_cb(const std_msgs::Float32MultiArray& msg)
{
had_message =1;
  state_keep=msg;
}

void joy_cb(const geometry_msgs::Vector3& msg)
{
had_message =1;
  joy_keep=msg;
}

void cmd_cb(const geometry_msgs::Vector3& msg)
{
had_message =1;
  cmd_keep=msg;
}

int main(int argc, char *argv[])
{
  ros::init (argc, argv, "csv_listener");
  ros::NodeHandle node;
  ros::Subscriber state_sub = node.subscribe("/state_post_KF", 1, state_cb);
  ros::Subscriber quad1_sub = node.subscribe("/quad1/pose", 1,  quad1_cb);
  ros::Subscriber quad2_sub = node.subscribe("/quad2/pose", 1, quad2_cb);
  ros::Subscriber joy_sub = node.subscribe("/joy_vel", 1, joy_cb);
  ros::Subscriber cmd_sub = node.subscribe("/cmd_vel_u", 1, cmd_cb);
  ros::Rate loop_rate(30);
  
joy_keep.x =0;
joy_keep.y =0;
joy_keep.z =0;
cmd_keep.x=0;
cmd_keep.y=0;
cmd_keep.z=0;

state_keep.data[8];
/*
state_keep.data[0]=0;
state_keep.data[1]=0;
state_keep.data[2]=0;
state_keep.data[3]=0;
state_keep.data[4]=0;
state_keep.data[5]=0;
state_keep.data[6]=0;
state_keep.data[7]=0;
state_keep.data[8]=0;
*/

if ((argc = !2))
{
	
	std::cout << "Too many (or no) arguments........arg= filename_to_create \n";
	std::cout << "number of args:" << argc << "\n";
	std::cout << argv[1] << "\n";
	return -1;

}
	std::stringstream ss;
	std::string delim = ",";
	std::string argv_name;
	std::string argv_fullname;
	std::ofstream myfile;
	char str_now [80];

	ss << argv[1];
	ss >> argv_name;
	argv_fullname=argv_name;
	argv_name.erase(argv_name.end(),argv_name.end());
	
  
	ROS_INFO("Creating csv: %s.csv",argv[1]);
	
	

	myfile.open ((argv_name+ ".csv").c_str(), std::ios::out | std::ios::app);
	myfile  << str_now << "," << "Decomposition of " << argv_name << "\n";

myfile << "Sequence, Time,";

myfile<<  "Quad1 Position in x [m], Position in y [m], Position in z [m],";

myfile<<  "Quad2 Position in x [m], Position in y [m], Position in z [m],";

myfile << "State Tag Position x [m],Position y [m],Position z [m],";
myfile << "Quad1 Vel x [m/s],Quad1 Vel y [m/s],Quad1 Vel z [m/s],";
myfile << "Quad2 Vel x [m/s],Quad2 Vel y [m/s],Quad2 Vel z [m/s],";

myfile << "Joy Input Vel x [m/s],Quad2 Vel y [m/s],Quad2 Vel z [m/s],";

myfile << "Cmd Vel Output Vel x [m/s],Quad2 Vel y [m/s],Quad2 Vel z [m/s],";

std::cout << "Creating csv from bag." <<std::endl;


	if (!had_message){
	ROS_INFO("Waiting for message.");
	}
	while (!had_message){
	ros::spinOnce ();
	loop_rate.sleep ();
	}
	
	ROS_INFO("Creating csv");
	while (ros::ok() && had_message)
		{
	//print quad1
		myfile << quad1_msg_keep.header.seq<< delim << quad1_msg_keep.header.stamp<< delim;
		myfile << quad1_msg_keep.transform.translation.x<< delim << quad1_msg_keep.transform.translation.y<< delim << quad1_msg_keep.transform.translation.z<< delim;
	//quad2
		myfile << quad2_msg_keep.transform.translation.x<< delim << quad2_msg_keep.transform.translation.y<< delim << quad2_msg_keep.transform.translation.z<< delim;
	//state
		myfile << state_keep.data[0] << delim << state_keep.data[1] << delim << state_keep.data[2] << delim;
		myfile << state_keep.data[3] << delim << state_keep.data[4] << delim << state_keep.data[5] << delim;
		myfile << state_keep.data[6] << delim << state_keep.data[7] << delim << state_keep.data[8] << delim;
	//joy
		myfile << joy_keep.x << delim << joy_keep.y << delim << joy_keep.z << delim;
	//cmd
		myfile << cmd_keep.x << delim << cmd_keep.y << delim << cmd_keep.z << delim;
	//end the line
		myfile << std::endl; //end line
		
		ros::spinOnce ();
		loop_rate.sleep ();
		}///while
	return 0;
	
}//main
