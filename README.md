# csma

#### Linux:
Ubuntu18.04

#### ROS version:
melodic

#### multi robot github reference
> https://github.com/airuchen/multi_turtlebot3

#### multi tb3
##### Gazebo
 > roslaunch turtlebot3_gazebo multi_turtlebot3.launch

##### RVIZ(navigation)
> roslaunch turtlebot3_navigation multi_nav_bringup.launch


#### Step
1. mkdir -p csma_ws/src
2. cd csma_ws/src
3. git clone https://github.com/zxsedas/csma.git
4. cd ~/csma_ws && catkin_make
5. devel/setup.bash
6. roslaunch csma dist_wp.launch
7. rosrun csma csma

- Notice only accept first goal point, if execute second times,you need restart csma package.


https://user-images.githubusercontent.com/56986643/143808679-2edd0073-5293-4fae-9114-9325e7ec69b1.mp4


