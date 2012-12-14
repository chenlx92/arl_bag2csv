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

int main(int argc, char *argv[])
{

	if ((argc = !2))
	{
		
		std::cout << "Too many (or no) arguments........arg= filename.bag \n";
		std::cout << "number of args:" << argc << "\n";
		std::cout << argv[1] << "\n";
		return -1;

	}

	std::cout << "Extracting data from: " << argv[1] <<std::endl;

	std::stringstream ss;
	std::string delim = ",";
	std::string argv_name;
	std::string argv_fullname;
	std::ofstream myfile;
	time_t rawtime;
	struct tm * timeinfo;
	char str_now [80];

	ss << argv[1];
	ss >> argv_name;
	argv_fullname=argv_name;
	argv_name.erase(argv_name.end()-4,argv_name.end());

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

  strftime (str_now,80,"%c",timeinfo);

	myfile.open ((argv_name+ ".csv").c_str(), std::ios::out | std::ios::app);
	myfile  << str_now << "," << "Rosbag decomposition of " << argv_name << "\n";

	rosbag::Bag bag;
	bag.open(argv_fullname, rosbag::bagmode::Read);
	rosbag::View view(bag);

myfile << "Sequence, Time, Position in x [m], Position in y [m], Position in z [m],";
myfile<< "Rotation in x [quaternions], Rotation in y [quaternions], Rotation in z [quaternions], Rotation in w [quaternions],";
myfile << "Command in Linear x (Pitch) [-1 to 1], Command in Linear y (Roll) [-1 to 1], Command in Linear z (Thrust) [-1 to 1], Command in Angular z (Yaw) [-1 to 1],";
myfile << "IMU Gyro x [rads/sec],IMU Gyro y [rads/sec],IMU Gyro y [rads/sec],";
myfile << "IMU Accel x [m/sec^2],IMU Accel x [m/sec^2],IMU Accel x [m/sec^2]";
myfile <<std::endl;
	BOOST_FOREACH(rosbag::MessageInstance const m, view)
	{
		if (m.getTopic() == "/pose")
		{
		geometry_msgs::TransformStamped::ConstPtr value = m.instantiate<geometry_msgs::TransformStamped>();
			
			if (value != NULL)
			{	
				myfile << value->header.seq<< delim << value->header.stamp<< delim;
				myfile << value->transform.translation.x<< delim << value->transform.translation.y<< delim << value->transform.translation.z<< delim;
				myfile << value->transform.rotation.x<< delim << value->transform.rotation.y<< delim << value->transform.rotation.z<< delim;
				myfile << value->transform.rotation.w<< delim;
			}
		}//quad pose
		
		else if (m.getTopic() == "/commands")
		{
			geometry_msgs::Twist::ConstPtr value = m.instantiate<geometry_msgs::Twist>();
			if (value != NULL)
			{	
				myfile << value->linear.x << delim << value->linear.y << delim;
				myfile << value->linear.z << delim << value->angular.z << delim;
			}
		}//else cmd_vel
		
		else if (m.getTopic() == "/imu")
		{
			sensor_msgs::Imu::ConstPtr value = m.instantiate<sensor_msgs::Imu>();
			if (value != NULL)
			{	
				myfile << value->angular_velocity.x << delim << value->angular_velocity.y << delim << value->angular_velocity.z << delim;
				myfile << value->linear_acceleration.x << delim << value->linear_acceleration.y << delim << value->linear_acceleration.z << delim;
				myfile << std::endl; //end line
			}
		}//else imu
		
	}//boost
	bag.close();
 	myfile.close();
	return 0;
}//main
