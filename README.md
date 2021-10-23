# csma

#### linux
Ubuntu18.04

#### ROS version
melodic

#### gitlab
> https://gitlab.com/lab314-iiot/most/most109/multi_turtlebot3

#### tb3
##### Gazebo
 > roslaunch turtlebot3_gazebo multi_turtlebot3.launch

##### RVIZ(navigation)
> roslaunch turtlebot3_navigation multi_nav_bringup.launch


#### Step
1. mkdir -p csma_ws/src
2. cd csma_ws/src
3. git clone https://github.com/zxsedas/csma.git
4. cd ~/csma_ws && devel/setup.bash
5. rosrun csma distTwoPoint
6. rosrun csma csma
