#include "ros/ros.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "std_msgs/Float64.h"
#include <cmath>

double tb3_0_poseAMCLx, tb3_0_poseAMCLy, tb3_0_poseAMCLa;
double tb3_1_poseAMCLx, tb3_1_poseAMCLy, tb3_1_poseAMCLa;
double distanceTwoPoint = 0;

void tb3_0_poseAMCLCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msgAMCL)
{
    tb3_0_poseAMCLx = msgAMCL->pose.pose.position.x;
    tb3_0_poseAMCLy = msgAMCL->pose.pose.position.y;
    tb3_0_poseAMCLa = msgAMCL->pose.pose.orientation.w;   
    //ROS_INFO("tb3_0 x:%lf, y:%lf, z:%lf", tb3_0_poseAMCLx, tb3_0_poseAMCLy, tb3_0_poseAMCLa);
}
void tb3_1_poseAMCLCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msgAMCL)
{
    tb3_1_poseAMCLx = msgAMCL->pose.pose.position.x;
    tb3_1_poseAMCLy = msgAMCL->pose.pose.position.y;
    tb3_1_poseAMCLa = msgAMCL->pose.pose.orientation.w;   
    //ROS_INFO("tb3_1 x:%lf, y:%lf, z:%lf", tb3_1_poseAMCLx, tb3_1_poseAMCLy, tb3_1_poseAMCLa);
}
void distanceTwoPointCal()
{
    double distX = pow( (tb3_0_poseAMCLx-tb3_1_poseAMCLx), 2.0);
    double distY = pow( (tb3_0_poseAMCLy-tb3_1_poseAMCLy), 2.0);
    distanceTwoPoint = pow( (distX + distY), 0.5);
    //ROS_INFO("%lf   %lf", distX, distY);
    //ROS_INFO("dist between two point: %lf", distanceTwoPoint);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
    ros::Publisher dist_pub = n.advertise<std_msgs::Float64>("distTwoPoint", 100);   
    ros::Subscriber sub_amcl_tb3_0 = n.subscribe<geometry_msgs::PoseWithCovarianceStamped>("tb3_0/amcl_pose", 100,tb3_0_poseAMCLCallback);
    ros::Subscriber sub_amcl_tb3_1 = n.subscribe<geometry_msgs::PoseWithCovarianceStamped>("tb3_1/amcl_pose", 100,tb3_1_poseAMCLCallback);
     


    ros::Rate loop_rate(10);
    while(ros::ok())
    {
	ros::spinOnce();
	std_msgs::Float64 msg;
	distanceTwoPointCal();
	msg.data = distanceTwoPoint;
	if(msg.data > 0)
	{
	    dist_pub.publish(msg);	
	    ROS_INFO("publish: %lf", msg.data);
	}	
	loop_rate.sleep();
    }
    

    //ros::spin();
    return 0;
}
