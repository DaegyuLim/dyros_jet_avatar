/**
  @title DYROS JET Controller
  @authors Jimin Lee, Suhan Park
  */

#include <ros/ros.h>
#include "dyros_jet_controller/simulation_interface.h"
// #include "hardware_interface.h"

using namespace dyros_jet_controller;


int main(int argc, char **argv)
{
    ros::init(argc, argv, "dyros_jet_controller");
    ros::NodeHandle nh("~");


    std::string mode;
    nh.param<std::string>("run_mode", mode, "simulation");
    controlBase *ctrObj;

    double Hz;
    nh.param<double>("control_frequency", Hz, 200.0);


    if(mode == "simulation")
    {
        ROS_INFO("DYROS JET MAIN CONTROLLER - !!! SIMULATION MODE !!!");
        ctrObj = new SimulationInterface(nh, Hz);
    }
    else if(mode == "real")
    {
        ROS_INFO("DYROS JET MAIN CONTROLLER - !!! REAL ROBOT MODE !!!");
        //ctrObj = new realrobot(nh);
        ROS_ERROR("REAL ROBOT MODE IS NOT IMPLEMENTED YET!!!");
    }
    else
    {
        ROS_FATAL("Please choose simulation or real");
    }

    while(ros::ok())
    {
        ctrObj->readdevice();
        ctrObj->update();
        ctrObj->compute();
        ctrObj->reflect();
        ctrObj->writedevice();
        ctrObj->wait();
    }

    delete ctrObj;

    return 0;
}
