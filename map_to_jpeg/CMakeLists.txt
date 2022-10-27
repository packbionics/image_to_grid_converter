cmake_minimum_required(VERSION 3.5)
project(map_to_jpeg)

find_package(rclcpp REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs)
find_package(tf2 REQUIRED)
find_package(image_transport REQUIRED)
find_package(cv_bridge REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIRS} include)

add_executable(map_to_image_node src/map_to_image_node.cpp)
ament_target_dependencies(map_to_image_node
"geometry_msgs"
"nav_msgs"
"tf2"
"image_transport"
"cv_bridge"
"sensor_msgs"
"rclcpp"
)
target_link_libraries(map_to_image_node)
target_link_libraries(map_to_image_node ${OpenCV_LIBRARIES})

add_executable(image_to_map_node src/image_to_map_node.cpp)
ament_target_dependencies(image_to_map_node
"geometry_msgs"
"nav_msgs"
"tf2"
"image_transport"
"cv_bridge"
"sensor_msgs"
"rclcpp"
)
target_link_libraries (image_to_map_node )
