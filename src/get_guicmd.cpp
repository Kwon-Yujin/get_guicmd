#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt16.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

// 터미널에 입력되는 명령줄 저장하는 글로벌 변수
std::string temp_cmd;    // 그때그때 실행 명령줄은 덮어씌워짐

// 시스템에 명령줄을 전달하는 포인터 변수
const char* sys_cmd;
const char* sys_stop_cmd;

void Exec_pkg_Callback(const std_msgs::UInt16& msg)
{
  if (msg.data == 0) {
    ROS_INFO("Mission start!");

    sys_cmd = temp_cmd.c_str();
    system(sys_cmd);
  }

  else if (msg.data == 1) {
    ROS_INFO("Joystick start!");

    temp_cmd = "gnome-terminal -- rosrun rosjoy_to_cmdvel rosjoy_to_cmdvel";
    sys_cmd = temp_cmd.c_str();
    system(sys_cmd);
  }

  else if (msg.data == 2) {
    ROS_INFO("MD driver start!");

    temp_cmd = "gnome-terminal -- rosrun md_driver md_driver_demo_node";
    sys_cmd = temp_cmd.c_str();
    system(sys_cmd);
  }

  else if (msg.data == 3) {
    ROS_INFO("Manipulator control start!");

    temp_cmd = "gnome-terminal -- rosrun xenoxeno xenoxeno_node";
  }

  else if (msg.data == 4) {
    ROS_INFO("Cam dynamixel control start!");

    temp_cmd = "gnome-terminal -- rosrun back_hand_controller back_hand_controller_node";
    sys_cmd = temp_cmd.c_str();
    system(sys_cmd);
  }

  else if (msg.data == 5) {
    ROS_INFO("Front Webcam drive start!");

    temp_cmd = "gnome-terminal -- chmod 777 /dev/video* && roslaunch usb_cam compress.launch && rosrun webcam_sub webcam_sub_node";
    //temp_cmd = "gnome-terminal -- roslaunch usb_cam usb_cam-test.launch";
    sys_cmd = temp_cmd.c_str();
    system(sys_cmd);
  }

  else if (msg.data == 6) {
    ROS_INFO("Front YoloCam drive start!");

    temp_cmd = "gnome-terminal -- cd ~/catkin_ws/src/kudos_edgetpu_yolo && python3 detect_change.py --model best-int8_edgetpu_s3.tflite --stream";
    sys_cmd = temp_cmd.c_str();
    system(sys_cmd);
  }

  else if (msg.data == 7) {
    ROS_INFO("Side RealSense drive start!");

    temp_cmd = "gnome-terminal -- chmod 777 /dev/video* && roslaunch usb_cam compress.launch && rosrun webcam_sub webcam_sub_node";
    sys_cmd = temp_cmd.c_str();
    system(sys_cmd);
  }

  else if (msg.data == 8) {
    ROS_INFO("web_video_server start!");

    temp_cmd = "gnome-terminal -- rosrun web_video_server web_video_server";
    sys_cmd = temp_cmd.c_str();
    system(sys_cmd);
  }

  else if (msg.data == 9) {
    ROS_INFO("Velodyne Lidar pkg start!");

    temp_cmd = "gnome-terminal -- ";
  }

  else if (msg.data == 11) {
    ROS_INFO("Auto drive start!");

    temp_cmd = "gnome-terminal -- roslaunch microstrain_inertial_driver microstrain.launch && rosrun self_imu imu_control";
    sys_cmd = temp_cmd.c_str();
    system(sys_cmd);
  }

  if (msg.data == 100) {
    // 본디 자율 주행 미션 allstop 명령
    ROS_INFO("Mission all stop!");
  }

  else if (msg.data == 101) {
    ROS_INFO("Terminate joystick control node");

    temp_cmd = "gnome-terminal -- rosnode kill /rosjoy_2_cmdvel";
    sys_stop_cmd = temp_cmd.c_str();
    system(sys_stop_cmd);
  }

  else if (msg.data == 102) {
    ROS_INFO("Terminate MD driver node");

    temp_cmd = "gnome-terminal -- rosnode kill /md_driver_node";
    sys_stop_cmd = temp_cmd.c_str();
    system(sys_stop_cmd);
  }

  else if (msg.data == 103) {
    ROS_INFO("Terminate manipulator control node");

    temp_cmd = "gnome-terminal -- rosnode kill /xenoxeno_node";
    sys_stop_cmd = temp_cmd.c_str();
    system(sys_stop_cmd);
  }

  else if (msg.data == 104) {
    ROS_INFO("Terminate cam dynamixel control node");

    temp_cmd = "gnome-terminal -- rosnode kill /back_hand_controller_node";
    sys_stop_cmd = temp_cmd.c_str();
    system(sys_stop_cmd);
  }

  else if (msg.data == 105) {
    ROS_INFO("Terminate front webcam drive node");

    temp_cmd = "gnome-terminal -- rosnode kill /usb_cam /webcam_sub_node";
    sys_stop_cmd = temp_cmd.c_str();
    system(sys_stop_cmd);
  }

  else if (msg.data == 106) {
    ROS_INFO("Terminate front yoloCam drive node");

    temp_cmd = "gnome-terminal -- rosnode kill /usb_cam /webcam_sub_node";
    sys_stop_cmd = temp_cmd.c_str();
    system(sys_stop_cmd);
  }

  else if (msg.data == 107) {
    ROS_INFO("Terminate side RealSense drive node");

    temp_cmd = "gnome-terminal -- rosnode kill /usb_cam /webcam_sub_node";
    sys_stop_cmd = temp_cmd.c_str();
    system(sys_stop_cmd);
  }

  else if (msg.data == 108) {
    ROS_INFO("Terminate web_video_server node");

    temp_cmd = "gnome-terminal -- rosnode kill /web_video_server";
    sys_stop_cmd = temp_cmd.c_str();
    system(sys_stop_cmd);
  }
}

void Cmd_string_Callback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "get_guicmd");
  ros::NodeHandle nh;

  // Publish /get_guicmd_ready topic to Control PC to confirm Comm rate
  //ros::Publisher Get_guicmd_publisher = nh.advertise<std_msgs::String>("chatter", 1000);

  // Subscribe execute pkg command topic from Control PC
  ros::Subscriber Exec_pkg_subscriber = nh.subscribe("exec_pkg", 100, Exec_pkg_Callback);
  ros::Subscriber Command_subscriber = nh.subscribe("gui_terminal_command", 100, Cmd_string_Callback);

  ros::spin();

  return 0;
}
