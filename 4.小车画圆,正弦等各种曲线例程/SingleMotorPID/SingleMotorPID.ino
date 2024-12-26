/**************************************************************************************************************************
实验功能：实现四麦克纳姆轮小车追踪黄色小球实验
实验日期：2024.11.13
实验成员：Unirobot
实验接线：如下简易图所示
                    车  头
                    x+方向
                    .___.
                    |   |   爪子舵机引脚：PL5
                  ._______.
                  |       | 爪子后方舵机引脚：PL4
      *------------------------------*
      |   [5,A4,A5]        [9,43,42] |
      |     (A)\\            //(B)   |
      |                              |
  Y+  |                              | Y-方向
      |                              |
      |     (C)//            \\(D)   |
      |   [8,37,36]        [12,34,35]|
      *------------------------------*
                    X-方向
                    车  尾

摄像头处的舵机引脚：PL3
***********************************************************************************************************************/

/****************
 程序需要的头文件
****************/
#include<PS2X_lib.h>
#include<Servo.h>
#include"encoder.h"
#include"kinematics.hpp"
#include"motor.h"
#include"pid.hpp"
#include"WonderK210.h"

#define program_systemctl_open 5000  //程序开机等待时间（等待k210模块上电）
#define program_systemctl_time 500   //程序部分函数执行时间
#define program_systemctl_motor_debug_time 3000 //程序电机调试部分动作时间

//k210模块声明部分----------------------------------------------------------------------------
WonderK210* wk;     //接收控制器
Find_Box_st* result;//消息存储变量

//AGV底盘四轮声明，相关参数配置部分------------------------------------------------------------
MPID PIDA(encA,KP,KI,KD,true);
MPID PIDB(encB,KP,KI,KD,false);
MPID PIDC(encC,KP,KI,KD,true);
MPID PIDD(encD,KP,KI,KD,false);
// MPID PIDD(encD,KP_D,KI_D,KD_D,false);

//声明舵机部分--------------------------------------------------------------------------------
Servo myservo1;
Servo myservo2;
Servo myservo3;

//程序是否要进入调试模式，以及进入哪种调试模式，需要调试，把相应的声明打开即可---------------------
// #define DEBUG                    //程序是否进入调试
// #define DEBUG_MOTOR              //程序进入电机部分调试
// #define DEBUG_SERVO              //程序进入舵机部分调试
// #define DEBUG_MOTOR_SERVO        //程序进入电机舵机同时运动部分调试
// #define DEBUG_K210               //程序进入k210部分调试
// #define DEBUG_MOTOR_SERVO_K210   //程序进入电机、舵机、k210同时运行部分调试
int car_mode_change = 0;

//程序初始化部分-------------------------------------------------------------------------------
void setup() {
  delay(program_systemctl_open);  //程序开机等待（主要时等待k210模块上电）
  Serial.begin(115200); //设置串口波特率
  Serial.println("serial open success");
  Serial.println("Wait for program init....");
  program_init();       //程序初始化部分
  Serial.print("Program init Success");delay(program_systemctl_time);
  ps2_init();
}

//程序主循环部分--------------------------------------------------------------------------------
void loop() {
  if(car_mode_change == 1){
    get_ps_key();
  }
  if(car_mode_change == 2){
    // car_mode_change = 0;
    // search_for_something();
    // cmdVelCb();
    // delay(10);
    car_mode_change = 0;
    search_for_ballon();
    search_for_field();
    cmdVelCb();
    delay(10);
  }
  ps2_mode_change();
}
