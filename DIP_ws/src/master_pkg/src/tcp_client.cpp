
#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"





#define ADD0 "192.168.0.133"  //dell-4  OK
#define ADD1 "192.168.0.134"  //dell-5  OK
#define ADD2 "192.168.0.165"  //dell-7  OK
#define ADD3 "192.168.0.104"  //dell-10 OK
#define ADD4 "192.168.0.177"  //dell-11 OK
#define ADD5 "192.168.0.129"  //dell-13 OK
#define ADD6 "192.168.0.135"  //dell-14 OK


#define PORT "1234"
#define NUMBER_ADD 7


#define ADD_WIDTH 20
#define PORT_WIDTH 20
#define MESSAGE_FREQ 100


#define NONE "\033[m"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"

/*
 *     \033[显示方式;前景色;背景色m
      显示方式 :0（默认值）、1（高亮）、22（非粗体）、4（下划线）、24（非下划线）、5（闪烁）、25（非闪烁）、7（反显）、27（非反显）
     前景色:30（黑色）、31（红色）、32（绿色）、 33（黄色）、34（蓝色）、35（洋红）、36（青色) 、37（白色）
     背景色:40（黑色）、41（红色）、42（绿色）、 43（黄色）、44（蓝色）、45（洋红）、46（青色）、47（白色）
    \033[0m 默认
    \033[1;32;40m 绿色
    033[1;31;40m 红色
 * 字背景颜色范围:40~49
 *
40:黑41:深红42:绿43:黄色44:蓝色45:紫色46:深绿47:白色
字颜色:30~39
30:黑31:红32:绿33:黄34:蓝色35:紫色36:深绿37:白色

\33[0m 关闭所有属性\33[1m 设置高亮度\33[4m 下划线
\33[5m 闪烁\33[7m 反显\33[8m 消隐
\33[30m -- \33[37m 设置前景色
\33[40m -- \33[47m 设置背景色
\33[nA 光标上移n行
\33[nB 光标下移n行
\33[nC 光标右移n行
\33[nD 光标左移n行
\33[y;xH设置光标位置
\33[2J 清屏
\33[K 清除从光标到行尾的内容
\33[s 保存光标位置
\33[u 恢复光标位置
\33[?25l 隐藏光标
\33[?25h 显示光标
————————————————
版权声明：本文为CSDN博主「sparkstrike」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/sparkstrike/article/details/70853716
 */


using namespace std;

class Listener {
private:
    char topic_message[NUMBER_ADD][256] ;
public:
    int count=0;
    char *address[NUMBER_ADD];
    char *port;
    bool isconnect[7]={false};

    char* getMessageValue(int num){return topic_message[num];}
    void callback(const geometry_msgs::Pose::ConstPtr& msg)
    {
//        cout<<*msg<<endl;
        int whitch=msg->position.z;
        if(whitch != 99){bzero(topic_message[whitch],256);
            geometry_msgs::Pose pose=*msg;
            char * ptr=(char *)&pose;
            for (int i = 0; i < sizeof(geometry_msgs::Pose); ++i) {
                topic_message[whitch][i]=ptr[i];
            }
            topic_message[whitch][sizeof(geometry_msgs::Pose)]='\0';}
//            return;

    }
    Listener()
    {
        for (int i = 0; i <NUMBER_ADD ; ++i) {
            address[i]=new char[ADD_WIDTH];
            bzero((char *) &(topic_message[i]), sizeof(topic_message[i]));
        }
        port=new char[PORT_WIDTH];
        port="1234";
    }
    ~Listener()
    {
        cout<<"Say you~"<<endl;
////        for (int i = 0; i <NUMBER_ADD ; ++i) {
////            delete[] address[i];
//        }delete port;
    }
};


int main(int argc, char *argv[]) {
    ros::init(argc, argv, "tcp_client");
    ros::NodeHandle nh;
    ros::Rate loop_rate(MESSAGE_FREQ); // set the rate as defined in the macro MESSAGE_FREQ

    Listener listener;

    ros::Subscriber client_sub = nh.subscribe("/master_topic", 20, &Listener::callback, &listener);
    geometry_msgs::Pose pose;

    int sockfd, portno, n;
    struct sockaddr_in serv_addr[NUMBER_ADD];
    struct hostent *server[NUMBER_ADD];


    while(ros::ok()) {
        listener.count++;
        ros::spinOnce();
	printf("\33[2J  \33[0;0H \033[1;32;40m ");
        printf(" \033[1;31;40m  master:%d \033[0m\n",listener.count);
        for (int i = 0; i <NUMBER_ADD ; ++i) {

            listener.address[0]=ADD0;
            listener.address[1]=ADD1;
            listener.address[2]=ADD2;
            listener.address[3]=ADD3;
            listener.address[4]=ADD4;
            listener.address[5]=ADD5;
            listener.address[6]=ADD6;

            listener.port = "1234";
            portno = atoi(listener.port);
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd< 0)
            {
                listener.isconnect[i] = false;
                ROS_INFO("ERROR socket open : %d", i);
            } else
            {
                server[i] = gethostbyname(listener.address[i]);
                bzero((char *) &(serv_addr[i]), sizeof(serv_addr[i]));
                serv_addr[i].sin_family = AF_INET;
                bcopy((char *)server[i]->h_addr,(char *)&serv_addr[i].sin_addr.s_addr, server[i]->h_length);
                serv_addr[i].sin_port = htons(portno);

                if (connect(sockfd,(struct sockaddr *) &(serv_addr[i]),sizeof(serv_addr[i])) < 0)
                {
                   // ROS_INFO("ERROR connect : %d", i);
                    listener.isconnect[i] = false;
                }
                else
                    listener.isconnect[i]= true;
            }

            if(listener.isconnect[i] )
            {
                n = write(sockfd,listener.getMessageValue(i),56);//后面这个n取错了不行。是0会导致那边不受
                //if (n < 0)
                  //  ROS_INFO("ERROR write : %d", i);
            }

            loop_rate.sleep();
            close(sockfd);
        }
        for(int i=0;i<NUMBER_ADD;++i)
        {
            printf("IP: %-20s  state:%s\n",listener.address[i],listener.isconnect[i]?"\033[1;32;40m connect\33[0m":"\033[1;31;40m disconnect\33[0m");
        }
    }
    return 0;
}
