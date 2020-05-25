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

//availible form 
//N S : 0 1 
//color green red : 0 1 
//cup color : 0 0 0 0 0
//transfer form 
//0 : don't eat
//1 : eat data
//2 : finish 
//3 : eat garbage


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
    int cup_suck = 0;
    int ns_suck = 0;
    srv_cup.request.OUO = 0;
    srv_ns.request.OAO = 0;
    
  
    ros::Rate loop_rate(1);

    while(ros::ok){
        double clustering_time = ros::Time::now().toSec () - begin_time; 

        if(clustering_time>0.0001){
            srv_cup.request.OUO=1;
        }
        if(clustering_time>30){
            srv_ns.request.OAO=1;
        }
        if(srv_cup.request.OUO==1){
            if(client_cup.call(srv_cup)){
                for(int i;i<5;i++){
                    if(srv_cup.response.CupResult[i]!=0 && srv_cup.response.CupResult[i]!=1 ){
                        cup_suck = 1 ;     
                    }else{
                        srv_cup.request.OUO = 2 ;//finish
                    } 
                }
            }   
        }
        if(cup_suck == 1 ){
            srv_cup.request.OUO = 3;
        }



        if(srv_ns.request.OAO==1){
            if(client_ns.call(srv_ns)){
                if(srv_ns.response.ns !=0 && srv_ns.response.ns!=1 ){
                    ns_suck = 1 ;     
                }else{
                    srv_ns.request.OAO = 2 ;//finish 
                } 
            }   
        }
        if(cup_suck == 1 ){
            srv_cup.request.OUO = 3;
        }







        if(count == 0 && clustering_time>4){
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
