cmake_minimum_required(VERSION 2.8.3)
project(wtrslave)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

catkin_package(

)

add_executable(slave  src/slave.cpp)
add_executable(server_node  src/server_node.cpp)


target_link_libraries(slave
  ${catkin_LIBRARIES}
)
target_link_libraries(server_node
        ${catkin_LIBRARIES}
        )
