#include <ros/ros.h>
#include "std_msgs/String.h"

class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    //Topic you want to publish
    pub_ = n_.advertise<std_msgs::String>("listener", 4);

    //Topic you want to subscribe
    sub_ = n_.subscribe("chatter", 1, &SubscribeAndPublish::callback, this);
  }

  void callback(const std_msgs::String& input)
  {
    std_msgs::String output;
    // Use de input......................
    output.data = "How are you Modem?";
    

    pub_.publish(output);
  }

private:
  ros::NodeHandle n_; 
  ros::Publisher pub_;
  ros::Subscriber sub_;

};//End of class SubscribeAndPublish

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "pubsubs");

  //Create an object of class SubscribeAndPublish that will take care of everything
  SubscribeAndPublish SAPObject;

  ros::spin();

  return 0;
}