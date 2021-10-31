# csma

#### Linux:
Ubuntu18.04

#### ROS version:
melodic

#### Gitlab
> https://gitlab.com/lab314-iiot/most/most109/multi_turtlebot3

#### multi tb3
##### Gazebo
 > roslaunch turtlebot3_gazebo multi_turtlebot3.launch

##### RVIZ(navigation)
> roslaunch turtlebot3_navigation multi_nav_bringup.launch


#### Step
1. mkdir -p csma_ws/src
2. cd csma_ws/src
3. git clone https://github.com/zxsedas/csma.git
4. cd ~/csma_ws && devel/setup.bash
5. roslaunch csma dist_wp.launch
6. rosrun csma csma

- Notice only accept first goal point, if execute second times,you need restart csma package.
![csma_test](https://user-images.githubusercontent.com/56986643/139579138-20b01229-bf19-48f9-8d37-17ba8a708999.gif)
