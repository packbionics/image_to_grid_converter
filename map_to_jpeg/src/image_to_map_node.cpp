/*
 * Copyright (c) 2009, CoroWare
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Stanford U. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <rclcpp/rclcpp.hpp>

#include <nav_msgs/srv/get_map.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <sensor_msgs/image_encodings.hpp>
#include <image_transport/image_transport.hpp>
#include <Eigen/Geometry>
#include <HectorMapTools.h>
#include <math.h>
#include <tf2/transform_datatypes.h>

using namespace std;

//resolution of the map
double resolution = 0.05; 

/**
 * @brief This node provides images as occupancy grid maps.
 */
class ImageAsMapProvider : public rclcpp::Node {
public:

  // Publisher for the map
  rclcpp::Publisher <nav_msgs::msg::OccupancyGrid>::SharedPtr map_publisher;

  //Subscriber to the image topic
  rclcpp::Subscription <sensor_msgs::msg::Image>::SharedPtr image_subscriber;

  ImageAsMapProvider(): rclcpp::Node("map_to_image_pub") {

    RCLCPP_INFO(this->get_logger(), "Hello Me");

    // publish the map as an occupancy grid
    map_publisher = this->create_publisher<nav_msgs::msg::OccupancyGrid>("map_raw", rclcpp::SensorDataQoS());

    // subscribe to the image topic representing the map
    auto subCallback = std::bind(&ImageAsMapProvider::mapCallback, this, std::placeholders::_1);
    image_subscriber = this->create_subscription<sensor_msgs::msg::Image>("image", rclcpp::SensorDataQoS(), subCallback);


    RCLCPP_INFO(this->get_logger(),"Image to Map node started.");
  }

  //The map->image conversion runs every time a new map is received
  void mapCallback(const sensor_msgs::msg::Image::SharedPtr image)
  {

    RCLCPP_INFO(this->get_logger(), "Hello World");
    nav_msgs::msg::OccupancyGrid map;
    //fill in some map parameters
    map.header.stamp = image->header.stamp;
    map.header.frame_id = "odom";
    map.info.width = image->width;
    map.info.height = image->height;
    map.info.origin.orientation.w = 1;
    map.info.resolution = resolution;
    map.info.origin.position.x = -((image->width + 1) * map.info.resolution * 0.5f);
    map.info.origin.position.y = -((image->height + 1) * map.info.resolution * 0.5f);

    //read the pixels of the image and fill the map table
    int data;
    for(int i=image->height - 1; i>=0; i--) {
      for (unsigned int j=0; j<image->width;j++) {

          data = image->data[i*image->width + j];
          if(data >=123 && data <= 131){
            map.data.push_back(-1);
          } else if (data >=251 && data <= 259){
            map.data.push_back(0);
          } else {
            map.data.push_back(100);
          }
      }
    }
    
    //publish the map
    map_publisher->publish(map); 
  }

};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<ImageAsMapProvider>();
  node->declare_parameter("resolution", 0.05); 

  rclcpp::spin(node);

  return 0;
}
