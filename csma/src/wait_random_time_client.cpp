#include "ros/ros.h"
#include "csma/WaitRandomTime.h"
#include <cstdlib>
#include <math.h>
int main(int argc, char **argv)
{
  ros::init(argc, argv, "wait_random_time_client");
  if (argc != 6)
  {
    ROS_INFO("usage: wait_random_time_client x,y,ow,oz,flag");
    return 1;
  }
  
  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<csma::WaitRandomTime>("wait_random_time");
  csma::WaitRandomTime srv;
  
 
  srv.request.x =  floor(atof(argv[1])*100)/100;
  srv.request.y =  floor(atof(argv[2])*100)/100;
  //Quaternion can only accept two decimal place. like 0.99 is fine ,but 0.999999 is not fine.
  srv.request.ow =  floor(atof(argv[3])*100)/100;
  srv.request.oz = floor(atof(argv[4])*100)/100;
  srv.request.flag = atoll(argv[5]);
 
 

  if (client.call(srv))
  {
    ROS_INFO("response:[robot:%ld,waiting %ld]", (long int)srv.request.flag,(long int)srv.response.time);
  }
  else
  {
    ROS_ERROR("Failed to call service wait_random_time");
    return 1;
  }

  return 0;
}
