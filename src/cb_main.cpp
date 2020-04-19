#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int32MultiArray.h>
#include <aruco_pose/cup.h>
#include <aruco_pose/ns.h>


#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <stdlib.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "cb_main");
    ros::NodeHandle n;
    ros::ServiceClient client_cup = n.serviceClient<aruco_pose::cup>("client_cup_");
    ros::ServiceClient client_ns = n.serviceClient<aruco_pose::ns>("client_ns_");
    aruco_pose::cup srv_cup;
    aruco_pose::ns srv_ns;
    double begin_time =ros::Time::now().toSec();
    int count = 0;

    while(ros::ok){
        double clustering_time = ros::Time::now().toSec () - begin_time; 
        srv_cup.request.OUO = 1;
        srv_ns.request.OAO = 2;
        if(count == 0 && clustering_time>5){
            if (client_cup.call(srv_cup){
                ROS_INFO("Sum: %ld", (long int)srv.response.CupResult[0];
                ROS_INFO("Sum: %ld", (long int)srv.response.CupResult[1];
                ROS_INFO("Sum: %ld", (long int)srv.response.CupResult[2];
                ROS_INFO("Sum: %ld", (long int)srv.response.CupResult[3];
                ROS_INFO("Sum: %ld", (long int)srv.response.CupResult[4];                
                count = 1  ; 
            }
            else{
                ROS_ERROR("Failed to call client_cup_");
            }
        }
        if(count == 1 && clustering_time>30){
            if (client_ns.call(srv_ns){
                ROS_INFO("Sum: %ld", (long int)srv.response.ns);
                count == 2 ; 
            }
            else{
                ROS_ERROR("Failed to call service client_ns_");
            }
        }

        ros::spinOnce();
    }
    return 0;
}