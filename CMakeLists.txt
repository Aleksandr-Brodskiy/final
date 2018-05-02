cmake_minimum_required(VERSION 2.8.3)
project(final)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  move_base
  sensor_msgs
  map_server
  geometry_msgs
  tf2
  amcl
  tf2_ros
)
catkin_package()
include_directories(  ${catkin_INCLUDE_DIRS})

add_executable(nav src/nav.cpp)
add_executable(treasures src/treasures.cpp)
add_executable(saferandomwalk src/saferandomwalk.cpp)
add_executable(planner src/planner.cpp)

target_link_libraries(nav ${catkin_LIBRARIES})
target_link_libraries(treasures ${catkin_LIBRARIES})
target_link_libraries(saferandomwalk ${catkin_LIBRARIES})
target_link_libraries(planner ${catkin_LIBRARIES})

