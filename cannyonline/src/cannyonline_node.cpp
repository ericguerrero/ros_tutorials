#include "ros/ros.h"
#include "std_msgs/Float64.h"

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc.hpp"


#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

using namespace cv;
using namespace std;



/// Global variables

Mat frame, original, gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";
clock_t t, last_t;
float freq;

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( gray, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  frame.copyTo( dst, detected_edges);
  imshow( window_name, dst );
 }


/** @function main */
int main(int argc, char **argv)
{
  ros::init(argc, argv, "cannyonline");
  ros::NodeHandle n;
  ros::Publisher freq_pub = n.advertise<std_msgs::Float64>("freq", 100);
  ros::Rate loop_rate(100);

  t = clock();

  // Get a handle to the Video device:
  VideoCapture cap(0);
  // Check if we can use this device at all:
  if(!cap.isOpened()) 
  {
      cerr << "Capture Device ID0 cannot be opened." << endl;
      return -1;
  }
  // Holds the current frame from the Video device:
  

  while (ros::ok())
  {
    std_msgs::Float64 msg;

    last_t = t;
    t = clock();
    freq = 1000000.0/(t-last_t);
    msg.data = freq;

    cerr << "freq: "<< freq << endl;

    freq_pub.publish(msg);

    cap >> frame;
    // Clone the current frame:
    original = frame.clone();
    // Convert the current frame to grayscale:
    cvtColor(original, gray, COLOR_BGR2GRAY);

    /// Create a window
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );

    /// Create a Trackbar for user to enter threshold
    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

    /// Show the image
    CannyThreshold(0, 0);

    // And display it:
    char key = (char) waitKey(20);
        if(key == 27)
        break;
    

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}