cmake_minimum_required(VERSION 2.8.3)
project(labpkg)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  geometry_msgs
  tf2
  amcl
  tf2_ros
)
catkin_package()
include_directories(  ${catkin_INCLUDE_DIRS})

#add_executable(nav src/nav.cpp)
add_executable(treasures src/treasures.cpp)

#target_link_libraries(nav ${catkin_LIBRARIES})
target_link_libraries(treasures ${catkin_LIBRARIES})
