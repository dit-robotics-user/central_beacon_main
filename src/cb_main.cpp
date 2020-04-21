#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int32MultiArray.h>
#include <central_beacon_main/cup.h>
#include <central_beacon_main/ns.h>


#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <stdlib.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "cb_main");
    ros::NodeHandle n;
    ros::ServiceClient client_cup = n.serviceClient<central_beacon_main::cup>("cup");
    ros::ServiceClient client_ns = n.serviceClient<central_beacon_main::ns>("ns");
    central_beacon_main::cup srv_cup;
    central_beacon_main::ns srv_ns;
    double begin_time =ros::Time::now().toSec();
    int count = 0;
ros::Rate loop_rate(1);


    while(ros::ok){
        double clustering_time = ros::Time::now().toSec () - begin_time; 
        srv_cup.request.OUO = 1;
        srv_ns.request.OAO = 2;
        if(count == 0 ){
            if (client_cup.call(srv_cup)){
                ROS_INFO("Sum: %ld", (long int)srv_cup.response.CupResult[0]);
                ROS_INFO("Sum: %ld", (long int)srv_cup.response.CupResult[1]);
                ROS_INFO("Sum: %ld", (long int)srv_cup.response.CupResult[2]);
                ROS_INFO("Sum: %ld", (long int)srv_cup.response.CupResult[3]);
                ROS_INFO("Sum: %ld", (long int)srv_cup.response.CupResult[4]);                
                count = 1  ; 
            }else{
                ROS_ERROR("Failed to call cup");
            }
        }

        if(count == 1 ){
            if (client_ns.call(srv_ns)){
                ROS_INFO("Sum: %ld", (long int)srv_ns.response.ns);
                count == 2 ; 
            }else{
                ROS_ERROR("Failed to call service ns");
            }
        }
	ROS_INFO("count:%d",count);
	loop_rate.sleep();
        ros::spinOnce();
    }
    return 0;
}
