#include "ros/ros.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "std_msgs/Float64.h"
#include "sensor_msgs/LaserScan.h"
#include <cmath>
#include <vector>
#define PI 3.14159265
sensor_msgs::LaserScan laser_msg;

void tb3_0_waitPoint(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    laser_msg = *msg;
    std::vector<float> laser_ranges;
    laser_ranges = laser_msg.ranges;
    float max = 0, min = 0;
    short maxIndex = 0, minIndex = 0;
    for(short index=90; index<=270;index++)
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
    float x = (cos(maxIndex*PI/180) * max)/4; //cos*r
    float y = (sin(maxIndex*PI/180) * max)/4;
    ROS_INFO("maxIndex: %d, max: %lf, x: %lf, y: %lf", maxIndex, max, x, y);
    //ROS_INFO("maxIndex: %d  maxValue: %lf ,  minIndex: %d  minValue: %lf", maxIndex, max, minIndex, min);
    //ros::Duration(3).sleep();
}
void tb3_1_waitPoint(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    laser_msg = *msg;
    std::vector<float> laser_ranges;
    laser_ranges = laser_msg.ranges;
    float max = 0, min = 0;
    short maxIndex = 0, minIndex = 0;
    for(short index=90; index<=270;index++)
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
    float x = (cos(maxIndex*PI/180) * max)/4; //cos*r
    float y = (sin(maxIndex*PI/180) * max)/4;
    ROS_INFO("maxIndex: %d, max: %lf, x: %lf, y: %lf", maxIndex, max, x, y);
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
    //ros::Publisher dist_pub = n.advertise<std_msgs::Float64>("distTwoPoint", 100);   
    ros::Subscriber sub_waitPoint_tb3_0 = n.subscribe<sensor_msgs::LaserScan>("tb3_0/scan", 100,tb3_0_waitPoint);
    ros::Subscriber sub_waitPoint_tb3_1 = n.subscribe<sensor_msgs::LaserScan>("tb3_1/scan", 100,tb3_1_waitPoint);
     


    ros::Rate loop_rate(10);
    while(ros::ok())
    {
	ros::spinOnce();
	//std_msgs::Float64 msg;
	//distanceTwoPointCal();
	//msg.data = distanceTwoPoint;
	// if(msg.data > 0)
	// {
	//     dist_pub.publish(msg);	
	//     ROS_INFO("publish: %lf", msg.data);
	// }	
	loop_rate.sleep();
    }
    

    //ros::spin();
    return 0;
}
