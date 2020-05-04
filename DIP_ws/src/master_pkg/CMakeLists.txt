cmake_minimum_required(VERSION 2.8.3)
project(wtrmaster)
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

add_executable(master  src/master.cpp)

add_executable(keyControl  src/keyControl.cpp)
add_executable(tcp_client  src/tcp_client.cpp)

target_link_libraries(master
  ${catkin_LIBRARIES}
)

target_link_libraries(keyControl
  ${catkin_LIBRARIES}
)
target_link_libraries(tcp_client
        ${catkin_LIBRARIES}
        )

