#include "ros/ros.h"

#include "std_msgs/Float64.h"
#include "std_msgs/Float64MultiArray.h"
#include "geometry_msgs/PoseStamped.h"
#include "nav_msgs/Path.h"

#include "csma/WaitRandomTime.h"
#include <thread>

#define tb3_between_distance 4.12
#define passage_start_x -0.42
#define passage_end_x 2.9
#define passage_start_y 0
#define passage_end_y 0.75
using namespace std;


int first_set_goal0 = 0, first_set_goal1 = 0;
double distTwoPoint = 10;
int tb0_length = 0, tb1_length = 0;
int *occupy0=new int(0);
int *occupy1=new int(0);
int* tb3_0_passage = new int(0);
int* tb3_1_passage = new int(0);

//tb3 current point
struct currentPoint{
    float tb3_x = 0;
    float tb3_y = 0;
    float tb3_w = 0;
    float tb3_z = 0; 
};
currentPoint tb3_0_currentPoint;   //tb3_0 current coordinate
currentPoint tb3_1_currentPoint;   //tb3_1 current coordinate

//tb3 goal point
struct goal{
	float tb3_x = 0;
	float tb3_y = 0;
	float tb3_w = 0;
	float tb3_z = 0;
};

goal tb3_0_goalPoint;   //tb3_0 goal coordinate
goal tb3_1_goalPoint;   //tb3_1 goal coordinate

void distCallback(const std_msgs::Float64::ConstPtr& msg)                     //echo distCallback 
{
    distTwoPoint = msg->data;
}
void determine(currentPoint tb, int* passage)
{
    if(tb.tb3_x >= passage_start_x && tb.tb3_x <= passage_end_x && tb.tb3_y >= passage_start_y && tb.tb3_y <= passage_end_y)
    {
        *passage=1;	
    }
    else
    {
 	*passage=0;
    }
}

void waitPointCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)     //echo waitPointCallback
{
    tb3_0_currentPoint.tb3_x = msg->data[0] + msg->data[4]; //currentX + waitPointX
    tb3_0_currentPoint.tb3_y = msg->data[1] + msg->data[5]; //currentY + waitPointY
    tb3_0_currentPoint.tb3_w = msg->data[2];//orientation
    tb3_0_currentPoint.tb3_z = msg->data[3];
    determine(tb3_0_currentPoint, tb3_0_passage);

    tb3_1_currentPoint.tb3_x = msg->data[6] + msg->data[10]; //currentX + waitPointX
    tb3_1_currentPoint.tb3_y = msg->data[7] + msg->data[11]; //currentY + waitPointY
    tb3_1_currentPoint.tb3_w = msg->data[8];//orientation
    tb3_1_currentPoint.tb3_z = msg->data[9];
    determine(tb3_1_currentPoint, tb3_1_passage);
}
void tb0_goalCallback(const geometry_msgs::PoseStamped::ConstPtr& goal)
{
    if(first_set_goal0 == 0)
    {
        tb3_0_goalPoint.tb3_x = goal->pose.position.x;
        tb3_0_goalPoint.tb3_y = goal->pose.position.y;
        tb3_0_goalPoint.tb3_w = goal->pose.orientation.w;
        tb3_0_goalPoint.tb3_z = goal->pose.orientation.z;
    }
    first_set_goal0++;
}
void tb1_goalCallback(const geometry_msgs::PoseStamped::ConstPtr& goal)
{
    if(first_set_goal1 == 0)
    {
        tb3_1_goalPoint.tb3_x = goal->pose.position.x;
        tb3_1_goalPoint.tb3_y = goal->pose.position.y;
        tb3_1_goalPoint.tb3_w = goal->pose.orientation.w;
        tb3_1_goalPoint.tb3_z = goal->pose.orientation.z;
    }
    first_set_goal1++;
}

void tb0_pathCallback(const nav_msgs::Path::ConstPtr& msg) 
{ 

	float length=0;
	int touch=0;
	for(auto i=0; i<msg->poses.size()-1;i++) 
	{ 
		float position_a_x = msg->poses[i].pose.position.x;
    		float position_b_x = msg->poses[i+1].pose.position.x;
    		float position_a_y = msg->poses[i].pose.position.y;
    		float position_b_y = msg->poses[i+1].pose.position.y;
		float distX = pow((position_b_x-position_a_x), 2.0);
		float distY = pow((position_b_y-position_a_y), 2.0);
		//ROS_INFO("%lf,  %lf", distX, distY);
		length += pow((distX + distY), 0.5);
		if(position_a_x >= passage_start_x && position_a_x <= passage_end_x && position_a_y >= passage_start_y && position_a_y <= passage_end_y)
		{
			touch=1;
		}
	}
	if(touch)
	{
		*occupy0=1;
	}
	else
	{
		*occupy0=0;
	} 
	tb0_length = length;
	//ROS_INFO("length:%d", l);
}
void tb1_pathCallback(const nav_msgs::Path::ConstPtr& msg) 
{ 

	float length=0;
	int touch=0;
	for(auto i=0; i<msg->poses.size()-1;i++) 
	{ 
		float position_a_x = msg->poses[i].pose.position.x;
    		float position_b_x = msg->poses[i+1].pose.position.x;
    		float position_a_y = msg->poses[i].pose.position.y;
    		float position_b_y = msg->poses[i+1].pose.position.y;
		float distX = pow((position_b_x-position_a_x), 2.0);
		float distY = pow((position_b_y-position_a_y), 2.0);
		//ROS_INFO("%lf,  %lf", distX, distY);
		length += pow((distX + distY), 0.5);
		if(position_a_x >= passage_start_x && position_a_x <= passage_end_x && position_a_y >= passage_start_y && position_a_y <= passage_end_y)
		{
			touch=1;
		}
	
	} 
	if(touch)
	{
		*occupy1=1;
	}
	else
	{
		*occupy1=0;
	} 
	tb1_length = length;
	//ROS_INFO("length:%d", tb1_length);
}

class Tb_csma                 //individual tb3
{
    private:
        ros::NodeHandle n;
	ros::Subscriber dp; //subscriber distance between two point
        ros::Subscriber wp; //subscriber waitPoint
        ros::Subscriber gp; //subscriber final goal point
	ros::Subscriber pa;

        ros::Publisher tp0; //tb3 0 Publisher to goal
        ros::Publisher tp1; //tb3 1 Publisher to goal
        ros::ServiceClient client = n.serviceClient<csma::WaitRandomTime>("wait_random_time"); //wait_random_time client

        csma::WaitRandomTime srv;                                          //self define srv
        geometry_msgs::PoseStamped msg;                                    //goal msg
        
        //tb3 current + waitPoint x,y,w
        currentPoint* tb3_currentPoint;  
        //tb3 final goal point x,y,w,z
        goal* tb3_goalPoint;
        //tb3 sleep time and flag
        int tb3_sleep_time = 0, flag = 0;      //flag is tb3_identity(tb3_0 == 0 so on..)
	int* in_passage=0;
	int* another_occupy=0;
	int navigation = 1;
    public:
        Tb_csma(){};
            Tb_csma(currentPoint* tb3_cp, goal* tb3_gp, int flag, string tb3_sub_goal)
            {
                dp = n.subscribe("distTwoPoint", 100, distCallback);       //subscribe distTwoPoint
                wp = n.subscribe("waitPoint", 100, waitPointCallback);     //subscribe waitPoint
                
                //which tb3 publish goal and sub_goal
                if(flag == 0)
                {
                    tp0 = n.advertise<geometry_msgs::PoseStamped>("/tb3_0/move_base_simple/goal", 100);
                    gp = n.subscribe<geometry_msgs::PoseStamped>(tb3_sub_goal, 100,  tb0_goalCallback);
		    pa = n.subscribe<nav_msgs::Path>("/tb3_0/move_base/NavfnROS/plan", 100, tb0_pathCallback);
		    in_passage=tb3_0_passage;
		    another_occupy=occupy1;
                }
                else if(flag == 1)
                {
                    tp1 = n.advertise<geometry_msgs::PoseStamped>("/tb3_1/move_base_simple/goal", 100);
                    gp = n.subscribe<geometry_msgs::PoseStamped>(tb3_sub_goal, 100,  tb1_goalCallback);
		    pa = n.subscribe<nav_msgs::Path>("/tb3_1/move_base/NavfnROS/plan", 100, tb1_pathCallback);
		    in_passage=tb3_1_passage;
		    another_occupy=occupy0;
                }
                //tb3 current???goal point structure
                tb3_currentPoint = tb3_cp;
                tb3_goalPoint = tb3_gp;
                this->flag = flag;

                //goal msg init
                msg.header.seq = 4;
                msg.header.stamp.sec = 0;
                msg.header.stamp.nsec = 0;
                msg.header.frame_id = "map";
                msg.pose.position.z = 0;
            }
        void run()
        {
            ros::Rate loop_rate(10);
            while(ros::ok())
            {
                ros::spinOnce();
                if(deadlock() && !(*in_passage) && *(another_occupy) && navigation > 0)                        //deadlock whether occur
                {
		    ROS_INFO("deadlock ocur!!");
                    tb3_sleep_time = callRandomTime();         //get randomSleepTime
		    clearOcuppy();		  
		    ROS_INFO("tb3_%d SLEEP:%d", this->flag, tb3_sleep_time);
                    if(tb3_sleep_time == -1)                   //error return -1
                    {
                        ROS_INFO("tb3_sleep_time -1 error!!");
                    }
                    else
                    {
			ROS_INFO("TB3_%d go to wait point(5s duration)",this->flag);
                        ros::Duration(5).sleep();
                        //sleep done
                        sleep(tb3_sleep_time);
                        //restart go to goal point
                        toFinalGoal(); 
                        
                    }    
                }
                loop_rate.sleep();
            }
        }
        bool deadlock()                                           //judge deadlock occur
        {
	    if(distTwoPoint <= tb3_between_distance)// tb3 between distance <= 3.25;
            {
		return true;
	    }
	    return false;          
        }
        int callRandomTime()                                      //call server get randomSleepTime
        {
            srv.request.x  =  floor((tb3_currentPoint->tb3_x)*100)/100;
            srv.request.y  =  floor((tb3_currentPoint->tb3_y)*100)/100;
            srv.request.ow =  floor((tb3_currentPoint->tb3_w)*100)/100;
            srv.request.oz =  floor((tb3_currentPoint->tb3_z)*100)/100;
            srv.request.flag = this->flag;
            
           
	        ROS_INFO("tb3_%d x:%lf, y:%lf", this->flag, tb3_currentPoint->tb3_x, tb3_currentPoint->tb3_y);
            if(client.call(srv))
            {
		ROS_INFO("TB3_%d CALL", this->flag);
                return srv.response.time;
            }
            return -1;
        }
        void toFinalGoal()                                        //restart go to goal point
        {	 
            msg.pose.position.x    = tb3_goalPoint->tb3_x;
            msg.pose.position.y    = tb3_goalPoint->tb3_y;
            msg.pose.orientation.w = tb3_goalPoint->tb3_w;
            msg.pose.orientation.z = tb3_goalPoint->tb3_z;   
           
            if(this->flag == 0 && !(*another_occupy))
	    {
		tp0.publish(msg);
		navigation--;
		ROS_INFO("tb3_%d_restart navigation", this->flag);
	    }   
            else if(this->flag == 1 && !(*another_occupy))
	    {
		tp1.publish(msg);
		navigation--;
		ROS_INFO("tb3_%d_restart navigation", this->flag);
		
	    }
                
        }
	void clearOcuppy()
	{
		if(this->flag == 0)
		    *occupy0=0;
		else if(this->flag == 1)
		    *occupy1=0;
	}
	void sleep(int s)
	{
		ros::Duration(s).sleep();
                ROS_INFO("Tb3_%d_sleep_done!!", this->flag); 
	}
};

class Control
{
    private:
        Tb_csma tb_0;
	    Tb_csma tb_1;        //tb3 obj t0,t1
        thread th0, th1;      //thread th0, th1
    public:
        Control()
        {
            //init tb0,tb1    tb0_flag == 0 tb1_flag == 1   individual x,y
            tb_0 = Tb_csma(&tb3_0_currentPoint, &tb3_0_goalPoint, 0, "tb3_0/move_base_simple/goal");
            tb_1 = Tb_csma(&tb3_1_currentPoint, &tb3_1_goalPoint, 1, "tb3_1/move_base_simple/goal");
            //assign thread
            th0 = thread(&Tb_csma::run, tb_0);
            th1 = thread(&Tb_csma::run, tb_1);
        }
        void exec()             //startup tb3_t0,t1
        {
            th0.join();
            th1.join();
        }
};



int main(int argc, char **argv)
{
    ros::init(argc, argv, "csma");
    Control Ct;  //init Control
    Ct.exec();   //startup t0,t1 use multi_thread
    return 0;
}
