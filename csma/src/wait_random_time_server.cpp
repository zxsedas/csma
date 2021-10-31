#include "ros/ros.h"
#include "csma/WaitRandomTime.h"
#include "geometry_msgs/PoseStamped.h"
#include "time.h"


//not used, maybe can be used to limit the maximum trying times.
int count_0 = 0;
int count_1 = 0;
int count_2 =0;


//tb3_0~3 's waiting time, init is 0 .
int time_0 = 0;
int time_1 = 0;
int time_2 = 0;

class waiting_class{
private:
	ros::NodeHandle n;
        

	geometry_msgs::PoseStamped msg;
	

	ros::Publisher tb3_0_pub = n.advertise<geometry_msgs::PoseStamped>("/tb3_0/move_base_simple/goal", 1000);
	ros::Publisher tb3_1_pub = n.advertise<geometry_msgs::PoseStamped>("/tb3_1/move_base_simple/goal", 1000);
	ros::Publisher tb3_2_pub = n.advertise<geometry_msgs::PoseStamped>("/tb3_2/move_base_simple/goal", 1000);


public:
        bool cal_time(csma::WaitRandomTime::Request  &req,
         csma::WaitRandomTime::Response &res)
        {
		//msg's data 
		msg.header.seq = 4;
		msg.header.stamp.sec = 0;
		msg.header.stamp.nsec = 0;
		msg.header.frame_id = "map";
		msg.pose.position.x = req.x;
		msg.pose.position.y = req.y;
		msg.pose.position.z = 0;
		msg.pose.orientation.w = req.ow;
		msg.pose.orientation.z = req.oz;
          



		//judge which tb3 should be control.
		switch(req.flag){
			case 0:
				//pub to navigation topic
				tb3_0_pub.publish(msg);	

				//random a waitingtime from 0 to 9 at first time.
				//waiting time*2 since second time.
	        		if(count_0 == 0)random(0);
				else time_0*=2;

				//add 3 sec lets robot go to waiting point.
				res.time = time_0;	
				
				
				count_0++;
				break;
			case 1:
				tb3_1_pub.publish(msg);	
	        		if(count_1 == 0)random(1);
				else time_1*=2;
				res.time = time_1;

				count_1++;
				break;
			case 2:
				tb3_2_pub.publish(msg);
				if(count_2 == 0)random(2);
				else time_2*=2;
				res.time = time_2;

				count_2++;
				break;
			case 99: //reset all data
				count_0=count_1=count_2=0;
				time_0=time_1=time_2=0;
				ROS_INFO("Reset");
				break;
			default:
				ROS_INFO("Wrong Flag Value.");
				return true;
		}
                                           
		//ros::Duration(5).sleep();
		
		return true;
        }
        void random(int flag){
		//ini waiting times from 1 to 10
                int A[10] ={1,2,3,4,5,6,7,8,9,10};
                int i,pos,temp;
               
		//shuffle 10 times
                for(i = 0; i < 10; i++){
                        //random pos 0 to 9 
                	pos =  rand()%10;
                	temp = A[i];
                	A[i] = A[pos];
               		A[pos] = temp;
                }    
		//choose A[0] as the waiting time.
		if(flag==0) time_0=A[0];
		else if(flag==1) time_1=A[5];
		else if(flag==2) time_2=A[9];
                
        }
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "wait_random_time_server");
  ros::NodeHandle n;
  srand((unsigned)time(NULL));
		
  waiting_class a;

  ros::ServiceServer service = n.advertiseService("wait_random_time",&waiting_class::cal_time,&a);
  ROS_INFO("Ready to accecpt x,y,ow,oz,flag.");
  ros::spin();

  return 0;
}
