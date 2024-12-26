#ifndef _MOTOR_H
#define _MOTOR_H
#include"config.h"
#define MOTORA_FORWARD(pwm)    do{digitalWrite(DIRA1,LOW); digitalWrite(DIRA2,HIGH);analogWrite(PWMA,pwm);}while(0)
// #define MOTORA_STOP(x)         do{digitalWrite(DIRA1,LOW); digitalWrite(DIRA2,LOW); analogWrite(PWMA,0);}while(0)
#define MOTORA_STOP(x)         do{digitalWrite(DIRA1,HIGH); digitalWrite(DIRA2,HIGH); analogWrite(PWMA,0);}while(0)
#define MOTORA_BACKOFF(pwm)    do{digitalWrite(DIRA1,HIGH);digitalWrite(DIRA2,LOW); analogWrite(PWMA,pwm);}while(0)

#define MOTORB_BACKOFF(pwm)    do{digitalWrite(DIRB1,LOW); digitalWrite(DIRB2,HIGH);analogWrite(PWMB,pwm);}while(0)
// #define MOTORB_STOP(x)         do{digitalWrite(DIRB1,LOW); digitalWrite(DIRB2,LOW); analogWrite(PWMB,0);}while(0)
#define MOTORB_STOP(x)         do{digitalWrite(DIRB1,HIGH); digitalWrite(DIRB2,HIGH); analogWrite(PWMB,0);}while(0)
#define MOTORB_FORWARD(pwm)    do{digitalWrite(DIRB1,HIGH);digitalWrite(DIRB2,LOW); analogWrite(PWMB,pwm);}while(0)

#define MOTORC_FORWARD(pwm)    do{digitalWrite(DIRC1,LOW); digitalWrite(DIRC2,HIGH);analogWrite(PWMC,pwm);}while(0)
// #define MOTORC_STOP(x)         do{digitalWrite(DIRC1,LOW); digitalWrite(DIRC2,LOW); analogWrite(PWMC,0);}while(0)
#define MOTORC_STOP(x)         do{digitalWrite(DIRC1,HIGH); digitalWrite(DIRC2,HIGH); analogWrite(PWMC,0);}while(0)
#define MOTORC_BACKOFF(pwm)    do{digitalWrite(DIRC1,HIGH);digitalWrite(DIRC2,LOW); analogWrite(PWMC,pwm);}while(0)

#define MOTORD_BACKOFF(pwm)    do{digitalWrite(DIRD1,LOW); digitalWrite(DIRD2,HIGH);analogWrite(PWMD,pwm);}while(0)
// #define MOTORD_STOP(x)         do{digitalWrite(DIRD1,LOW); digitalWrite(DIRD2,LOW); analogWrite(PWMD,0);}while(0)
#define MOTORD_STOP(x)         do{digitalWrite(DIRD1,HIGH); digitalWrite(DIRD2,HIGH); analogWrite(PWMD,0);}while(0)
#define MOTORD_FORWARD(pwm)    do{digitalWrite(DIRD1,HIGH);digitalWrite(DIRD2,LOW); analogWrite(PWMD,pwm);}while(0)

void STOP()
{
  MOTORA_STOP(Motor_PWM);MOTORB_STOP(Motor_PWM);
  MOTORC_STOP(Motor_PWM);MOTORD_STOP(Motor_PWM);
}

void MotorA(int pwm){
  if(pwm>=1){
    MOTORA_FORWARD(pwm);
  }else if(pwm<=-1){
    MOTORA_BACKOFF(abs(pwm));
  }else{
    MOTORA_STOP(x);
  }
}

void MotorB(int pwm){
  if(pwm>=1){
    MOTORB_FORWARD(pwm);
  }else if(pwm<=-1){
    MOTORB_BACKOFF(abs(pwm));
  }else{
    MOTORB_STOP(x);
  }
}
void MotorC(int pwm){
  if(pwm>=1){
    MOTORC_FORWARD(pwm);
  }else if(pwm<=-1){
    MOTORC_BACKOFF(abs(pwm));
  }else{
    MOTORC_STOP(x);
  }
}
void MotorD(int pwm){
  if(pwm>=1){
    MOTORD_FORWARD(pwm);
  }else if(pwm<=-1){
    MOTORD_BACKOFF(abs(pwm));
  }else{
    MOTORD_STOP(x);
  }
}

void motor_move(int lf, int rf, int lb, int rb){
  MotorA(rb);
  MotorB(lb);
  MotorC(rf);
  MotorD(lf);
}

void IO_init()
{
  pinMode(PWMA, OUTPUT);
  pinMode(DIRA1, OUTPUT);pinMode(DIRA2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRB1, OUTPUT);pinMode(DIRB2, OUTPUT);
  pinMode(PWMC, OUTPUT);
  pinMode(DIRC1, OUTPUT);pinMode(DIRC2, OUTPUT);
  pinMode(PWMD, OUTPUT);
  pinMode(DIRD1, OUTPUT);pinMode(DIRD2, OUTPUT);
  STOP();
}

#endif
