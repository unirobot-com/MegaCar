#ifndef _KINEMATICS_HPP_
#define _KINEMATICS_HPP_
#include"config.h"

//wABCD m/s
void InverseKinematic(float vx,float vy,float vw, float &pwmA,float &pwmB,float &pwmC,float &pwmD){
  // 书写方式一
  // target_vel_lf = vx + vy - vw*(body_length / 2.0);
  // target_vel_lb = vx - vy + vw*(body_length / 2.0);
  // target_vel_rf = vx - vy - vw*(body_length / 2.0);
  // target_vel_rb = vx + vy + vw*(body_length / 2.0);
  // pwmA = target_vel_lf;
  // pwmB = target_vel_lb;
  // pwmC = target_vel_rf;
  // pwmD = target_vel_rb;

  // if( (vx == 0) && (vy == 0) && (vw != 0) ){
  //   pwmA = vx + vy + vw*(xn + yn);
  //   pwmB = vw*(xn + yn);
  //   pwmC = vx - vy + vw*(xn + yn);
  //   pwmD = vx + vy + vw*(xn + yn);
  // }
  // else{
  //   //书写方式二
  //   pwmA = vx + vy + vw*(xn + yn);
  //   pwmB = vx - vy + vw*(xn + yn);
  //   pwmC = vx - vy - vw*(xn + yn);
  //   pwmD = vx + vy - vw*(xn + yn);
  // }

  //书写方式二
  pwmA = vx + vy + vw*(xn + yn);
  pwmB = vx - vy - vw*(xn + yn);
  pwmC = vx - vy + vw*(xn + yn);
  pwmD = (vx + vy - vw*(xn + yn));

//   if( (vx > 0) && (vy == 0) && (vw == 0) ){
//     Serial.println("i am forward");
//     pwmA = vx;
//     pwmB = vx;
//     pwmC = vx;
//     pwmD = vx;
//   }
//   if( (vx < 0) && (vy == 0) && (vw == 0) ){
//     Serial.println("i am back");
//     pwmA = vx;
//     pwmB = vx;
//     pwmC = vx;
//     pwmD = vx;
//   }
// if( (vx == 0) && (vy == 0) && (vw < 0) ){
//     Serial.println("i am left");
//     pwmA = -vw*(xn + yn);
//     pwmB = -vw*(xn + yn);
//     pwmC = vw*(xn + yn);
//     pwmD = -vw*(xn + yn);
//   }
  // Serial.print(pwmA);
  // Serial.print(" : ");
  // Serial.print(pwmB);
  // Serial.print(" : ");
  // Serial.print(pwmC);
  // Serial.print(" : ");
  // Serial.println(pwmD);
}

void ForwardKinematic(float wA,float wB,float wC,float wD,float &vx,float &vy,float &omega){
  vx=r/4.0*(wA+wB+wC+wD);
  vy=r/4.0*(+wA-wB-wC+wD);
  omega=r/(4.0*K)*(-wA+wB-wC+wD);
}

#endif
