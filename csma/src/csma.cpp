#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "actionlib_msgs/GoalID.h"

double distTwoPoint = 1;

void distCallback(const std_msgs::Float64::ConstPtr& msg)
{
    //ROS_INFO("I heard: %lf", msg->data);
    distTwoPoint = msg->data;
    //msg->data;
}


class Csma
{
    private:
        
        ros::Subscriber sub;
        ros::NodeHandle n;
        ros::Publisher tb3_0_terminate_pub;
        ros::Publisher tb3_1_terminate_pub;
        actionlib_msgs::GoalID empty_goal;
        int count=0;
    public:
        Csma()
        {
            sub = n.subscribe("distTwoPoint", 1000, distCallback);    //capture distTwoPoint
            this->tb3_0_terminate_pub = n.advertise<actionlib_msgs::GoalID>("tb3_0/move_base/cancel", 1);
            this->tb3_1_terminate_pub = n.advertise<actionlib_msgs::GoalID>("tb3_1/move_base/cancel", 1);
	         
	}
        void run();
        bool deadlock();
	
};
void Csma::run()
{
    ros::Rate loop_rate(10);
    while(ros::ok())
    {
	ros::spinOnce();
        if(deadlock())
	{
            tb3_0_terminate_pub.publish(empty_goal);
            tb3_1_terminate_pub.publish(empty_goal);
            ROS_INFO("deadlock occur!!");
	}
        loop_rate.sleep();
    }
}
bool Csma::deadlock()
{
	//ROS_INFO("dist: %lf", distTwoPoint);
	return distTwoPoint <= 0.28;
}





int main(int argc, char **argv)
{
    ros::init(argc, argv, "csma");
    //ros::NodeHandle n;
    //ros::Subscriber sub = n.subscribe("distTwoPoint", 1000, distCallback);    //capture distTwoPoint
    //ros::Publisher pub0 = n.advertise<actionlib_msgs::GoalID>("tb3_0/move_base/cancel", 1000);
    //ros::Publisher pub1 = n.advertise<actionlib_msgs::GoalID>("tb3_1/move_base/cancel", 1000);
    Csma csmaNode;
    csmaNode.run();
    return 0;
}
