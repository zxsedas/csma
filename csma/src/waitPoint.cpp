#include "ros/ros.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Float64MultiArray.h"
#include "sensor_msgs/LaserScan.h"
#include <cmath>
#include <vector>

#include "laser_geometry/laser_geometry.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"
#include "tf/transform_listener.h"
#include <boost/shared_ptr.hpp>

#define PI 3.14159265
#define laserStartAngle 0
#define laserEndAngle 180
sensor_msgs::LaserScan laser_msg;


float tb3_0_currentPositionX = 0, tb3_0_currentPositionY = 0, tb3_0_currentOrientationW = 0, tb3_0_currentOrientationZ = 0;
float tb3_1_currentPositionX = 0, tb3_1_currentPositionY = 0, tb3_1_currentOrientationW = 0, tb3_1_currentOrientationZ = 0;

float tb3_0_waitPointX = 0, tb3_0_waitPointY = 0;
float tb3_1_waitPointX = 0, tb3_1_waitPointY = 0;

void tb3_0_currentPosition(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msgAMCL)
{
	tb3_0_currentPositionX = msgAMCL->pose.pose.position.x;
	tb3_0_currentPositionY = msgAMCL->pose.pose.position.y;
	tb3_0_currentOrientationW = msgAMCL->pose.pose.orientation.w;
	tb3_0_currentOrientationZ = msgAMCL->pose.pose.orientation.z;
}
void tb3_1_currentPosition(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msgAMCL)
{
	tb3_1_currentPositionX = msgAMCL->pose.pose.position.x;
	tb3_1_currentPositionY = msgAMCL->pose.pose.position.y;
	tb3_1_currentOrientationW = msgAMCL->pose.pose.orientation.w;
	tb3_1_currentOrientationZ = msgAMCL->pose.pose.orientation.z;
}


void tb3_0_waitPoint(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    laser_msg = *msg;
    std::vector<float> laser_ranges;
    laser_ranges = laser_msg.ranges;
    float max = 0, min = 0;
    short maxIndex = 0, minIndex = 0;
    for(short index=laserStartAngle; index<=laserEndAngle;index++)
    {
	if(!isfinite(laser_ranges[index]))continue; //if value == inf  do continue
	//ROS_INFO("index: %d, value: %lf", index, laser_ranges[index]);
	if(laser_ranges[index] > max)              //laser value max
	{
		maxIndex = index;
		max = laser_ranges[index];
	}	
	// if(laser_ranges[index] < min)              //laser value min
	// {
	// 	minIndex = index;
	// 	min = laser_ranges[index];
	// }
	
    }
    tb3_0_waitPointX = -(abs((cos(maxIndex*PI/180) * max))/8); //cos*r
    tb3_0_waitPointY = (sin(maxIndex*PI/180) * max)/8;
	
    //ROS_INFO("maxIndex: %d, max: %lf, x: %lf, y: %lf", maxIndex, max, x, y);
  
  
}


void tb3_1_waitPoint(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    laser_msg = *msg;
    std::vector<float> laser_ranges;
    laser_ranges = laser_msg.ranges;
    float max = 0, min = 0;
    short maxIndex = 0, minIndex = 0;
    for(short index=laserStartAngle; index<=laserEndAngle;index++)
    {
	if(!isfinite(laser_ranges[index]))continue; //if value == inf  do continue
	//ROS_INFO("index: %d, value: %lf", index, laser_ranges[index]);
	if(laser_ranges[index] > max)              //laser value max
	{
		maxIndex = index;
		max = laser_ranges[index];
	}	
	// if(laser_ranges[index] < min)              //laser value min
	// {
	// 	minIndex = index;
	// 	min = laser_ranges[index];
	// }
	
    }
    
    tb3_1_waitPointX = abs((cos(maxIndex*PI/180) * max))/8; //cos*r
    tb3_1_waitPointY = (sin(maxIndex*PI/180) * max)/8;
    //ROS_INFO("TB3_1_X:%lf, TB3_1_Y:%lf ", tb3_1_waitPointX, tb3_1_waitPointY)
    //ROS_INFO("maxIndex: %d, max: %lf, x: %lf, y: %lf", maxIndex, max, tb3_1_waitPointX, tb3_1_waitPointY);

}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "waitPoint");
    ros::NodeHandle n;  
   


    ros::Subscriber sub_waitPoint_tb3_0 = n.subscribe<sensor_msgs::LaserScan>("tb3_0/scan", 100,tb3_0_waitPoint);
    ros::Subscriber sub_waitPoint_tb3_1 = n.subscribe<sensor_msgs::LaserScan>("tb3_1/scan", 100,tb3_1_waitPoint);

    ros::Subscriber sub_currentPosition_tb3_0 = n.subscribe<geometry_msgs::PoseWithCovarianceStamped>("tb3_0/amcl_pose", 100, tb3_0_currentPosition);
    ros::Subscriber sub_currentPosition_tb3_1 = n.subscribe<geometry_msgs::PoseWithCovarianceStamped>("tb3_1/amcl_pose", 100, tb3_1_currentPosition);
    ros::Publisher waitPoint = n.advertise<std_msgs::Float64MultiArray>("waitPoint", 100); 


    ros::Rate loop_rate(10);
    while(ros::ok())
    {
	ros::spinOnce();
	std_msgs::Float64MultiArray msg;
	msg.data.push_back(tb3_0_currentPositionX);    //msg[0]
	msg.data.push_back(tb3_0_currentPositionY);    //msg[1]
        msg.data.push_back(tb3_0_currentOrientationW); //msg[2]
	msg.data.push_back(tb3_0_currentOrientationZ); //msg[3]
	msg.data.push_back(tb3_0_waitPointX);          //msg[4]
	msg.data.push_back(tb3_0_waitPointY);          //msg[5]

	msg.data.push_back(tb3_1_currentPositionX);    //msg[6]
	msg.data.push_back(tb3_1_currentPositionY);    //msg[7]
	msg.data.push_back(tb3_1_currentOrientationW); //msg[8]
	msg.data.push_back(tb3_1_currentOrientationZ); //msg[9]
	msg.data.push_back(tb3_1_waitPointX);          //msg[10]
	msg.data.push_back(tb3_1_waitPointY);          //msg[11]

        //[tb3_0_positionX, tb3_0_positionY, tb3_0_orientation, tb3_0_waitPointX, tb3_0_waitPointY, 						 tb3_1_positionX, tb3_1_positionY, tb3_1_orientation,tb3_1_waitPointX, tb3_1_waitPointY]
	waitPoint.publish(msg);
                       
	loop_rate.sleep();
    }
    

    return 0;
}
