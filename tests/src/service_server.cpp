#include "ros/ros.h"
#include "std_srvs/Empty.h"


bool callback(std_srvs::Empty::Request& request, std_srvs::Empty::Response& response)
{
  ROS_INFO("SRV SERVER: Request recived");
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "service_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("empty_service", callback);
  ROS_INFO("SRV SERVER: The server is ready");
  ros::spin();

  return 0;
}