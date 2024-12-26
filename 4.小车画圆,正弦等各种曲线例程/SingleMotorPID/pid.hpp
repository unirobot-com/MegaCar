#ifndef _PID_HPP_
#define _PID_HPP_

// #define DEBUG
#include "config.h"

class MPID{
  public:
  Encoder *enc;    //指向编码器对象的指针，编码器通常用于测量电机的位置或速度
  float kP,kI,kD;  //PID 控制器的比例、积分和微分系数
  float error;       //当前误差值
  float currentError;//累积误差值
  float lastError;   //上一次的误差值
  unsigned long lastTime;    //记录上次的时间戳。
  unsigned long currentTime; //记录当前的时间戳。
  long lastEnc;    //记录上次的编码器读数
  long currentEnc; //记录当前的编码器读数
  bool reverse;    //反转标志
  long deltaEnc;   //编码器读数的变化量
  float deltaT;    //时间间隔
  long encoder_motor_sum;
  float output_pwm;
  float bias;
  float last_bias;
  float prev_error;

  MPID(Encoder *Enc,float kp,float ki,float kd,bool revs){
    enc=Enc;
    kP=kp;kI=ki;kD=kd;
    
    error=0;
    currentError=0;
    lastError=0;        
    reverse=revs;
    deltaEnc=0;
    deltaT=0.1;   //TODO  时间间隔
    encoder_motor_sum = 0;
    bias;
    last_bias;
    prev_error;
    output_pwm = 0.0;
  }

  void init(){    //init 方法将 lastTime 和 lastEnc 初始化为 0，准备开始新的控制周期
    lastTime=0;
    lastEnc=0;
  }

  //tic 方法更新当前时间戳 currentTime 和当前编码器读数 currentEnc
  void tic(){
    currentTime=millis();
    currentEnc=enc->read();
    currentEnc=reverse?-currentEnc:currentEnc; //如果 reverse 为真，则编码器读数取反。
    // encoder_motor_sum = currentEnc;
  }

  //getWheelRotatialSpeed 方法计算轮子的旋转速度。这里假设 resolution 和 r 是已知的常量，分别表示编码器的分辨率和轮子的半径
  float getWheelRotatialSpeed(){
    float current_linear_speed = deltaEnc*resolution/deltaT;
    return current_linear_speed;
  }

  //toc 方法更新 lastTime 和 lastEnc 为当前值，以便在下一次调用 tic 时使用
  void toc(){
    lastTime=currentTime;
    lastEnc=currentEnc;
  }

  //计算 PWM 输出
  int getPWM(float target_v){
    // target_v = pwma;
    deltaEnc=currentEnc-lastEnc;
    deltaT=(currentTime-lastTime)/1000.0;

    float current_v = deltaEnc*resolution/deltaT;

    //Serial.print("curv:");
    // float data1 = (current_v*1000);
    // float data2 = (target_v*1000);
    // Serial.print("data1:");Serial.print(" ");Serial.println(data1);
    // Serial.print("data2:");Serial.println(data2);
    bias=(current_v)-(target_v);
    // bias=(target_v)-(current_v);
    currentError += kI * bias;
    float kd_value = bias - last_bias;
    output_pwm += kP * bias + currentError + kD * kd_value;
    // output_pwm -= kP * bias + currentError + kD * kd_value;
    last_bias = bias;
    output_pwm = constrain(output_pwm,-255,255);
    // Serial.print("output_pwm:");Serial.println(output_pwm);
    return output_pwm;
  }

  float getDeltaT(){
    return deltaT;
  }
};
#endif
