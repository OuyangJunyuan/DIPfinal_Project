#include <string>
#include <iostream>
#include <math.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Twist.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "eigen3/Eigen/Geometry"
#include "tf/tf.h"
const int ID = 0x00;
using namespace std;
geometry_msgs::Pose target_pose,fdb_pose;
geometry_msgs::Twist pub_vel;
#define Kp_x 0.3
#define Kp_y 1.5
#define Kd_y 0.1
#define Ki_y 0
/*
 * ^x
 * |
 * |
 * |
 * |
 * ----------------------------->
 *                               y-
 *
 * ^y
 * |
 * |
 * |
 * |
 * ----------------------------->
 *                                x
 * */


float delta_an[3];
void tarCallback(const geometry_msgs::Pose::ConstPtr& msg)
{
    target_pose.position.x = msg->position.x;
    target_pose.position.y = msg->position.y;
    target_pose.position.z = msg->position.z;
}

void fdbCallback(const geometry_msgs::Pose::ConstPtr& msg)
{
    fdb_pose.position.x = msg->position.x;
    fdb_pose.position.y = msg->position.y;
    fdb_pose.position.z = msg->position.z;

    fdb_pose.orientation.y = msg->orientation.y;
    fdb_pose.orientation.x = msg->orientation.x;
    fdb_pose.orientation.w = msg->orientation.w;
    fdb_pose.orientation.z = msg->orientation.z;
}

float error[3];
float output;
float PID_control(float fdb,float target)
{
    error[0] = fdb-target;
    output=Kp_y*(error[0])+Kd_y*(error[1]-error[0]);
    error[2]=error[1];
    error[1]=error[0];

    cout<<output<<endl;
    if(output>0.5)
    {
        output=0.5;
    }
    else if(output<-0.5){
        output=-0.5;
    }

    for(int i =1 ;i<1000;i++)
    {

    }
return output;

}
float dis(){
    float x2 = (target_pose.position.x-fdb_pose.position.x) * (target_pose.position.x-fdb_pose.position.x);
    float y2 = (target_pose.position.y-fdb_pose.position.y) * (target_pose.position.y-fdb_pose.position.y);
    return sqrt(x2+y2);
}
float positon_N[2];
int i_1=0;
ros::Publisher   vel_pub;
void volcility_pid()
{
    tf::Quaternion quet;
    tf::quaternionMsgToTF(fdb_pose.orientation,quet);
    double yaw,pitch,roll;
    tf::Matrix3x3(quet).getRPY(roll,pitch,yaw);
//    cout<<yaw<<" "<<fdb_pose.orientation.w<<endl;
//    cout<<"DIS:"<<dis()<<"Angle"<<eulerAngle<<endl;
    pub_vel.linear.x=dis()*Kp_x;
    cout<<"DIS:"<<dis()<<"    linear.x"<<pub_vel.linear.x<<endl;
    float tar_an;
    float delt_x,delt_y;
    delt_x = target_pose.position.x-fdb_pose.position.x;
    delt_y = target_pose.position.y-fdb_pose.position.y;
    float theta = atan2( abs(delt_x) , abs(delt_y) );//-3.141592/2;
//      cout<<target_pose<<endl;
//    cout<<fdb_pose<<endl;
//    cout<<"delta x:"<<abs(delt_x)<<"    delt y"<<abs(delt_y)<<"    theta:" <<theta/6.28*360<<endl;
//    cout<<(yaw /6.28*360) <<" "<<tar_an*360/6.28<<endl;

    if(delt_x>0&&delt_y>0)
    {
        tar_an= 3.1415/2-theta;
    }
    if (delt_x>0&&delt_y<0)
    {
        tar_an=-3.1415/2+theta;
    }
    else if (delt_x<0&&delt_y>0)
    {
tar_an=3.1415/2+theta;
    }
    else if (delt_x<0&&delt_y<0)
    {
tar_an=-3.1415/2-theta;
    }



    delta_an[0] = yaw+3.1415926*2 - tar_an;
    delta_an[1] = yaw-3.1415926*2 - tar_an;
    delta_an[2] = yaw - tar_an;
    int index = 0;
    if(abs(delta_an[0]) > abs(delta_an[1])){
        index = 1;
    }else index = 0;

    if(abs(delta_an[2]) < abs(delta_an[index])){
        index = 2;
    }

    //cout<<"YAW:"<<yaw/6.28*360<<"    Target:"<<tar_an/6.28*360<<"     Delta:"<<delta_an[index]/6.28*360<<endl;
    //pub_vel.angular.z =-(delta_an[index])*Kp_y+Kd_y*; //fu shun
    pub_vel.angular.z =-PID_control(delta_an[index],0.0);

//    cout<<(yaw /6.28*360) <<" "<<(yaw - tar_an)*360/6.28<<endl;
    if(pub_vel.linear.x>0.5)
    {
        pub_vel.linear.x=0.5;
    }

//    if(abs(pub_vel.angular.z)>0.1)
//    {
//        pub_vel.linear.x = 0;
//    }
    if(dis()<0.05){
        pub_vel.linear.x = 0;
        pub_vel.angular.z=0;
    }
    if(abs(delta_an[index])>0.13)
    {
        pub_vel.linear.x=0;
    }
//    cout<<pub_vel<<endl;
    cout<<pub_vel.angular.z<<endl;
//    cout<<"robot"<<yaw/6.28*360<<"    theta:" <<theta/6.28*360<<"   Target:"<<tar_an/6.28*360<<endl;
    vel_pub.publish(pub_vel);


}
int main(int argc, char *argv[]) {
    ros::init(argc, argv, "formate_slave");
    ros::NodeHandle n;
   vel_pub = n.advertise<geometry_msgs::Twist>("/smoother_cmd_vel", 1000);
    ros::Subscriber  tar_sub = n.subscribe("/WTR/target_pose", 1000, tarCallback);
    ros::Subscriber  fdb_sub = n.subscribe("/robot_pose", 1000, fdbCallback);
    ros::Rate loop(10);
    while (ros::ok()) {


        volcility_pid();
        loop.sleep();
        ros::spinOnce();
    }

    return 0;
}

