#ifndef _CONFIG_H_
#define _CONFIG_H_
//电机引脚======================================================
#define PWMD 12   //设定D电机转速引脚
#define DIRD1 34
#define DIRD2 35  //设定D电机方向
#define PWMC 8    //设定C电机转速引脚
#define DIRC1 37
#define DIRC2 36  //设定C电机方向
#define PWMB 9    //设定B电机转速引脚
#define DIRB1 43
#define DIRB2 42  //设定B电机方向
#define PWMA 5    //设定A电机转速引脚
#define DIRA1 A4  //
#define DIRA2 A5  //设定A电机方向

//电机速度参数======================================================================================
#define CAR_MOVE_VX_FORWARD_SPEED_QUICKLY            -0.15    //设定小车前进最大速度-快，单位：m/s
#define CAR_MOVE_VX_FORWARD_SPEED_SLOWLY             -0.1     //设定小车前进最大速度-慢，单位：m/s
#define CAR_MOVE_VX_BACK_SPEED_QUICKLY                0.15    //设定小车后退最大速度-快，单位：m/s
#define CAR_MOVE_VX_BACK_SPEED_SLOWLY                 0.15    //设定小车后退最大速度-慢，单位：m/s
#define CAR_MOVE_VY_RIGHT_TRANSLATION_SPEED_QUICKLY  -0.15    //设定小车右平移最大速度-快，单位：m/s
#define CAR_MOVE_VY_RIGHT_TRANSLATION_SPEED_SLOWLY   -0.1     //设定小车右平移最大速度-慢，单位：m/s
#define CAR_MOVE_VY_LEFT_TRANSLATION_SPEED_QUICKLY    0.15    //设定小车左平移最大速度-快，单位：m/s
#define CAR_MOVE_VY_LEFT_TRANSLATION_SPEED_SLOWLY     0.1     //设定小车左平移最大速度-慢，单位：m/s

#define CAR_MOVE_VW_LEFT_TURN_SPEED_QUICKLY           0.4     //设定小车左转最大速度-快，单位：rad/s
#define CAR_MOVE_VW_LEFT_TURN_SPEED_SLOWLY            0.4     //设定小车左转最大速度-慢，单位：rad/s
#define CAR_MOVE_VW_RIGHT_TURN_SPEED_QUICKLY         -0.4     //设定小车右转最大速度-快，单位：rad/s
#define CAR_MOVE_VW_RIGHT_TURN_SPEED_SLOWLY          -0.4     //设定小车右转最大速度-慢，单位：rad/s
#define CAR_MOVE_STOP_SPEED                           0       //设定小车停止时速度，单位：m/s

//舵机参数设置=========================================================================================
#define SERVO_COUNT  3 //设置舵机数量
#define SERVO_STEP 50.0
#define SERVO_STEP_INT 50
#define SERVO_STEP_TIME 40 //舵机转动每个步数等待的时间
const int servo_pin[SERVO_COUNT] = {13, 45, 46}; //设置舵机引脚
int servo_angle[SERVO_COUNT] = {30, 100, 40};           //这一行和下一行需要一致
float current_angle[SERVO_COUNT] = {30.0, 100.0, 40.0}; //这一行需要和上一行一致
// int servo_angle[SERVO_COUNT] = {30, 25, 90};           //这一行和下一行需要一致
// float current_angle[SERVO_COUNT] = {30.0, 25.0, 90.0}; //这一行需要和上一行一致
int servo1_angle = 30;
int servo2_angle = 100;
int servo3_angle = 40;
int servo_step = 1;
int servo_action_delay = 10;

//设置电机相关参数======================================================================================
enum{FORWARD=1,BACK,TURN_LEFT,TURN_RIGHT,TRANS_LEFT,TRANS_RIGHT,CAR_STOP};
// float xn=0.08725, yn=0.105, r=0.03, K=abs(xn)+abs(yn); //m
float xn=0.0925, yn=0.1075, r=0.0475, K=abs(xn)+abs(yn); //m
int max_rmp= 330; //default: 330;
// float resolution=2*PI*r/1440.0;
float resolution=2*PI*r/1560.0;
float body_width = 0.215;
float body_length = 0.185;
float turn_contant = (body_length * body_length + body_width * body_width) / body_width / 2.0 * 0.98;
float pwma=0,pwmb=0,pwmc=0,pwmd=0;
float target_vel_lf = 0.0;
float target_vel_rf = 0.0;
float target_vel_lb = 0.0;
float target_vel_rb = 0.0;


//新添加的部分
float KP = 10.0;
float KI = 0.05;
float KD = 25.05;

float KP_D = 8.0;
float KI_D = 0.05;
float KD_D = 2.0;
float wA,wB,wC,wD;
float set_target_vx = 0.0;
float set_target_vy = 0.0;
float set_target_vw = 0.0;
unsigned int car_move_status = 0;
unsigned long calculate_time_period = 0;
unsigned long change_motor_action_time = 0;


bool car_move_vy_direction = false;
bool car_move_vx_direction = true;
unsigned long lastctrl;
double x=0,y=0,theta=0;

bool search_for_ballon_flag = true;
bool find_once_ballon_flag = false;
bool search_for_field_flag = false;
bool find_once_field_flag = false;
bool while_ps2_press = true;

#define constrain(amt,low,high) \
  ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))



#endif
