/*************************************************************************
* Author: Abhinav Jain
* Contact: abhinavjain241@gmail.com, abhinav.jain@heig-vd.ch
* Date: 28/06/2016
*
* This file contains source code to the server node of the ROS package
* comm_tcp developed at LaRA (Laboratory of Robotics and Automation)
* as part of my project during an internship from May 2016 - July 2016.
*
* (C) All rights reserved. LaRA, HEIG-VD, 2016 (http://lara.populus.ch/)
***************************************************************************/
#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"

using namespace std;
geometry_msgs::Pose pose_get;

void error(const char *msg) {
    perror(msg);
    exit(1);
}


//template <typename T>
//void decode_2_T(const unsigned char *input,T& output)
//{
//    output=*(T *)(input);
//}
//
//template <typename T>
//void code_2_uc(T& input, unsigned char * output)
//{
//    unsigned char *ptr=(unsigned char *)&input;
//    for (int i = 0; i < sizeof(T) ; ++i) {
//        output[i]=ptr[i];
//    }
//}

void decode(char *buf) {
    char temp[sizeof(geometry_msgs::Pose)];
    for (int i = 0; i < sizeof(geometry_msgs::Pose); ++i) {
        temp[i] = buf[i];
    }
    pose_get = *(geometry_msgs::Pose *) temp;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "server_node");
    ros::NodeHandle nh;

    ros::Publisher server_pub = nh.advertise<geometry_msgs::Pose>("/WTR/target_pose", 1000);
    //Testing package is working fine
    int sockfd, newsockfd, portno; //Socket file descriptors and port number
    socklen_t clilen; //object clilen of type socklen_t
    char buffer[256]; //buffer array of size 256
    struct sockaddr_in serv_addr, cli_addr; ///two objects to store client and server address
    std_msgs::String message;
    std::stringstream ss;
    int n;
    ros::Duration d(0.01); // 100Hz
//    if (argc < 2) {
//        fprintf(stderr, "ERROR, no port provided\n");
//        exit(1);
//    }
    portno = atoi("1234");
//    cout << "Hello there! This node is listening on port " << portno << " for incoming connections" << endl;

    while (ros::ok()) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("ERROR opening socket");
        int enable = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            error("setsockopt(SO_REUSEADDR) failed");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                 sizeof(serv_addr)) < 0)
            error("ERROR on binding");
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd,
                           (struct sockaddr *) &cli_addr,
                           &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        ss.str(std::string()); //Clear contents of string stream
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        decode(buffer);
        cout<<n<<endl;
        cout << pose_get << endl;
        server_pub.publish(pose_get);
        if (n < 0) error("ERROR reading from socket");
        close(sockfd);
        close(newsockfd);
//     ss << buffer;
//     message.data = ss.str();
//     ROS_INFO("%s", message.data.c_str());
//     server_pub.publish(message);
//     n = write(newsockfd,"I got your message",18);
        //if (n < 0) error("ERROR writing to socket");
    }
    return 0;
}
