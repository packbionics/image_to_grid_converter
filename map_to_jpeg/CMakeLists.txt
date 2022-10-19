cmake_minimum_required(VERSION 2.8.3)
project(map_to_jpeg)

find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(ament_cmake REQUIRED COMPONENTS sensor_msgs nav_msgs geometry_msgs tf image_transport cv_bridge)

include_directories(${EIGEN_INCLUDE_DIRS} include ${catkin_INCLUDE_DIRS})

add_executable(map_to_image_node src/map_to_image_node.cpp)
target_link_libraries(map_to_image_node )
target_link_libraries(map_to_image_node ${OpenCV_LIBRARIES})

add_executable(image_to_map_node src/image_to_map_node.cpp)
target_link_libraries (image_to_map_node )
