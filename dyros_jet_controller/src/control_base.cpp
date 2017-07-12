
#include "dyros_jet_controller/control_base.h"

namespace dyros_jet_controller
{

const string JOINT_NAME[40] = {"WaistPitch","WaistYaw",
                               "R_ShoulderPitch","R_ShoulderRoll","R_ShoulderYaw","R_ElbowRoll","R_WristYaw","R_WristRoll","R_HandYaw",
                               "L_ShoulderPitch","L_ShoulderRoll","L_ShoulderYaw","L_ElbowRoll","L_WristYaw","L_WristRoll","L_HandYaw",
                               "R_HipYaw","R_HipRoll","R_HipPitch","R_KneePitch","R_AnklePitch","R_AnkleRoll",
                               "L_HipYaw","L_HipRoll","L_HipPitch","L_KneePitch","L_AnklePitch","L_AnkleRoll",
                               "HeadYaw", "HeadPitch", "R_Gripper", "L_Gripper"};

const int JOINT_ID[40] = {28, 27, // waist yaw - roll order
                          1,3,5,7,9,11,13,
                          2,4,6,8,10,12,14,
                          15,17,19,21,23,25,
                          16,18,20,22,24,26,
                          29,30,31,32};


int jointOccupancy[40] = {WAIST, WAIST,
                          UPPER, UPPER, UPPER, UPPER, UPPER, UPPER, UPPER,
                          UPPER, UPPER, UPPER, UPPER, UPPER, UPPER, UPPER,
                          WALKING, WALKING, WALKING, WALKING, WALKING, WALKING,
                          WALKING, WALKING, WALKING, WALKING, WALKING, WALKING,
                          HEAD, HEAD, UPPER, UPPER};

// Constructor
controlBase::controlBase(ros::NodeHandle &nh, double Hz) :
  uiUpdateCount(0), isFirstBoot(true), Hz_(Hz), total_dof(32)
{
  parameter_initialize();
}


void controlBase::make_id_inverse_list()
{
  jointInvID.resize(50);
  for(int i=0;i<total_dof; i++)
  {
    jointID.push_back(JOINT_ID[i]);
    jointInvID[JOINT_ID[i]] = i;
  }
}


void controlBase::updateDesired(body_select body, VectorXd &update_q)
{
  for(int i=0; i<total_dof; i++)
  {
    if(jointOccupancy[i] == body)
    {
      desired_q_(i) = update_q(i);
    }
  }
}

void controlBase::update()
{

}

void controlBase::compute()
{

}

void controlBase::reflect()
{
}

void controlBase::parameter_initialize()
{
  q_.resize(total_dof); q_.setZero();
  q_dot_.resize(total_dof); q_dot_.setZero();
  torque_.resize(total_dof); torque_.setZero();
  leftFootFT_.setZero();  leftFootFT_.setZero();
  desired_q_.resize(total_dof); desired_q_.setZero();
  target_q_.resize(total_dof); target_q_.setZero();

}
void controlBase::readdevice()
{
  ros::spinOnce();
}

int controlBase::getch()
{
  fd_set set;
  struct timeval timeout;
  int rv;
  char buff = 0;
  int len = 1;
  int filedesc = 0;
  FD_ZERO(&set);
  FD_SET(filedesc, &set);

  timeout.tv_sec = 0;
  timeout.tv_usec = 100;

  rv = select(filedesc + 1, &set, NULL, NULL, &timeout);

  struct termios old = {0};
  if (tcgetattr(filedesc, &old) < 0)
    ROS_ERROR("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(filedesc, TCSANOW, &old) < 0)
    ROS_ERROR("tcsetattr ICANON");

  if(rv == -1)
  { }
  else if(rv == 0)
  {}
  else
    if(read(filedesc, &buff, len )) {} // just for unused

  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(filedesc, TCSADRAIN, &old) < 0)
    ROS_ERROR ("tcsetattr ~ICANON");
  return (buff);
}
double controlBase::Rounding( double x, int digit )
{
  return ( floor( (x) * pow( float(10), digit ) + 0.5f ) / pow( float(10), digit ) );
}

}