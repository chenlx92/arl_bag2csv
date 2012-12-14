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
#include <std_msgs/Int32.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TransformStamped.h> 
#include <rosgraph_msgs/Log.h>
#include <tf/tf.h>
#include <sensor_msgs/Imu.h>

//or just use $ rostopic echo -b file.bag -p /topic


geometry_msgs::Twist twist_msg_keep;
geometry_msgs::TransformStamped pose_msg_keep;
sensor_msgs::Imu imu_msg_keep;

void twist_cb(const geometry_msgs::Twist& twist_msg)
{
  twist_msg_keep=twist_msg;
}

void pose_cb(const geometry_msgs::TransformStamped& pose_msg)
{
  pose_msg_keep=pose_msg;
}

void imu_cb(const sensor_msgs::Imu& imu_msg)
{
  imu_msg_keep=imu_msg;
}

int main(int argc, char *argv[])
{
  ros::init (argc, argv, "Mouse_Control");
  ros::NodeHandle node;
  ros::Subscriber twist_sub = node.subscribe("/cmd_vel", 1, twist_cb);
   ros::Subscriber pose_sub = node.subscribe("/quad/pose", 1, pose_cb);
     ros::Subscriber imu_sub = node.subscribe("/ardrone/imu", 1, imu_cb);

  ros::Rate loop_rate(20);
  

	if ((argc = !2))
	{
		
		std::cout << "Too many (or no) arguments........arg= filename.bag \n";
		std::cout << "number of args:" << argc << "\n";
		std::cout << argv[1] << "\n";
		return -1;

	}

	ROS_INFO("Writing data to : %s.bag",argv[1]);
	
	std::stringstream ss;
	std::string delim = ",";
	std::string argv_name;

	ss << argv[1];
	ss >> argv_name;

	rosbag::Bag bag;
	bag.open((argv_name+ ".bag").c_str(), rosbag::bagmode::Write);

	while (ros::ok())
		{
		bag.write("/pose", ros::Time::now(), pose_msg_keep);
		bag.write("/commands", ros::Time::now(), twist_msg_keep);
		bag.write("/imu", ros::Time::now(), imu_msg_keep);

		ros::spinOnce ();
		loop_rate.sleep ();
		}///while
	bag.close();
	return 0;
	
}//main
