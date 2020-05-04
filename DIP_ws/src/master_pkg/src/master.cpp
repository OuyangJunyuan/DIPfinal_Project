//
// Created by ou on 2019/12/15.
//

#include "ros/ros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "geometry_msgs/Pose.h"
#include "std_msgs/String.h"
#include "iostream"
#include "std_msgs/Char.h"
using namespace std;

//10-9-12-16-15-6-8-
//#define ADD0 "192.168.0.133"  //dell-4  OK  1
//#define ADD1 "192.168.0.134"  //dell-5  OK  2
//#define ADD2 "192.168.0.165"  //dell-7  OK  3
//#define ADD3 "192.168.0.104"  //dell-10 OK  4
//#define ADD4 "192.168.0.177"  //dell-11 OK  5
//#define ADD5 "192.168.0.129"  //dell-13 OK  6
//#define ADD6 "192.168.0.135"  //dell-14 OK  7
/*
 *
 * R(Ready)
 * |
 * |
 * |      
 * |   1  3 6 4 75  2
 * |____________________________
 *
 * H
 * |       1     2
 * |       3  4  5
 * |       6     7
 * |
 * |___________________________________
 * I
 * |          1       2
 * | 3        4       5
 * | 6        7
 * |
 * |___________________________________
 * T
 * |       3  1  2
 * |          4       5
 * | 6        7
 * |
 * |___________________________________
 *
 *
 *    GRID 栅格
 *    OFFSET 偏值
 *
 *
 * */
#define GRID 1.0
#define OFFSET 2.00
const int H = 0,I=1,T=2;
float YmapH[] = {0,-2,0,-1,-2,0,-2};
float XmapH[] = {2,2,1,1,1,0,0};
float YmapI[] = {-1,-4,2,-1,-4,2,-1};
float XmapI[] = {2,2,1,1,1,0,0};
float YmapT[] = {-1,-2,0,-1,-4,2,-1};
float XmapT[] = {2,2,2,1,1,0,0};
float YmapS[] = {0,-6,-1,-3,-5,-2,-4};
float XmapS[] = {9,9,9,9,9,9,9};

char Mode='S';
geometry_msgs::Pose send_pose;
int flag=0;
int timer=0;
#define  delay(n)  (50*n)
void setPose(int ID,char C){
	
    switch (C){
        case '1': {
            if((ID==5||ID==6)) {
                send_pose.position.x = XmapH[ID] * GRID + OFFSET;
                send_pose.position.y = YmapH[ID] * GRID - OFFSET;
                send_pose.position.z = ID;
            }
            break;
        }
        case '2': {

            if((ID==5||ID==6||ID==2||ID==3||ID==4)) {
                send_pose.position.x = XmapH[ID] * GRID + OFFSET;
                send_pose.position.y = YmapH[ID] * GRID - OFFSET;
                send_pose.position.z = ID;

            }
            break;
        }
        case '3':
        {
                send_pose.position.x = XmapH[ID] * GRID + OFFSET;
                send_pose.position.y = YmapH[ID] * GRID - OFFSET;
                send_pose.position.z = ID;

            break;
        }
        case 'I':
        {
            send_pose.position.x = XmapI[ID] * GRID + OFFSET;
            send_pose.position.y = YmapI[ID] * GRID - OFFSET;
            send_pose.position.z =ID;
            break;
        }
        case 'T':
        {
            send_pose.position.x = XmapT[ID] * GRID + OFFSET;
            send_pose.position.y = YmapT[ID] * GRID - OFFSET;
            send_pose.position.z = ID;
            break;
        }
case 'S':
        {
            send_pose.position.x = XmapS[ID] * GRID;
            send_pose.position.y = YmapS[ID] * GRID;
            send_pose.position.z = ID;
            break;
        }
        default:
            send_pose.position.z = 99;
    }
}

void modeCallback(const std_msgs::Char::ConstPtr& msg){
    Mode = msg->data;
}

int main (int argc, char** argv)
{
    ros::init(argc, argv, "formate_master");
    ros::NodeHandle nh;
    ros::Publisher server_pub = nh.advertise<geometry_msgs::Pose>("/master_topic", 1000);
    ros::Subscriber subCom  = nh.subscribe("/WTR/mode",1000,modeCallback);
    ros::Rate loop_rate(50);
    while(ros::ok()) {
        for(int id=0;id<7;id++){
            setPose(id,Mode);
            server_pub.publish(send_pose);
            loop_rate.sleep();
        }
        cout<<Mode<<endl;


        ros::spinOnce();

    }
    return 0;
}
