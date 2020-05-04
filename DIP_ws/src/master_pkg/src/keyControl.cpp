//
// Created by ou on 2019/12/15.
//

#include "ros/ros.h"
#include <stdio.h>
#include <string.h>
#include "std_msgs/Char.h"
#include "iostream"

using namespace std;


int main (int argc, char** argv)
{
    ros::init(argc, argv, "keyControl");
    ros::NodeHandle nh;
    ros::Publisher mode_pub = nh.advertise<std_msgs::Char>("/WTR/mode", 1000);
    char C;
    std_msgs::Char pub;
    pub.data = 'S';
    while(ros::ok()) {
        cin>>C;

            cout<<"Please input H, I， T or S！"<<endl;
           // continue;

        pub.data = C;
        mode_pub.publish(pub);
        ros::spinOnce();
    }
    return 0;
}
