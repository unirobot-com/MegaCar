//舵机控制相关
// #include "config.h"

void servo_init(){
  servo_attach();
  delay(100);
  servo_write_init_angle();
  delay(1000);
  servo_detach();

  // for(int i=0;i<SERVO_COUNT;i++){
  //   current_angle[i] = servo_angle[i];
  // }
}

void servo_attach(){
  myservo1.attach(servo_pin[0]);
  myservo2.attach(servo_pin[1]);
  myservo3.attach(servo_pin[2]);  
}

void servo_write_init_angle(){
  myservo1.write(servo_angle[0]);
  myservo2.write(servo_angle[1]);
  myservo3.write(servo_angle[2]);
}

void servo_detach(){
  myservo1.detach();
  myservo2.detach();
  myservo3.detach();
  // digitalWrite(servo_pin[0], LOW);
  // digitalWrite(servo_pin[1], LOW);
  // digitalWrite(servo_pin[2], LOW);

  // digitalWrite(servo_pin[0], HIGH);
  // digitalWrite(servo_pin[1], HIGH);
  // digitalWrite(servo_pin[2], HIGH);
}

void servo_test(){
  servo_attach();
  servo_direct_mode(20, 70, 85);delay(1000);
  servo_direct_mode(70, 90, 30);delay(1000);
  servo_detach();
}

void claw_open(){

}

void claw_close(){

}

//舵机角度直接模式转动
void servo_direct_mode(int target_angle1, int target_angle2, int target_angle3){
  servo_attach();
  myservo1.write(target_angle1);
  myservo2.write(target_angle2);
  myservo3.write(target_angle3);
}

void servo_uniform_mode(float target_angle1, float target_angle2, float target_angle3, int delay_time){
  float target_angle[3] = {target_angle1, target_angle2, target_angle3};
  float delta_angle[3];
  // Serial.println("--------------------------------------------------------------------------------");
  // Serial.print("current_angle:");
  // for(int i=0;i<SERVO_COUNT;i++){ Serial.print(current_angle[i]); Serial.print("  ");}Serial.print("  ");
  // Serial.print("target_angle:");
  // for(int i=0;i<SERVO_COUNT;i++){ Serial.print(target_angle[i]); Serial.print("  ");}Serial.println();

  for(int i=0;i<SERVO_COUNT;i++){
    delta_angle[i] = (target_angle[i] - current_angle[i] == 0) ? 0 : (target_angle[i] - current_angle[i]) / SERVO_STEP;
    // Serial.print(delta_angle[i]);Serial.print("  ");
  }
  // Serial.println();

  for(int i=0;i<SERVO_STEP_INT;i++){
    // Serial.print(i);Serial.print(": ");
    for(int j=0;j<SERVO_COUNT;j++){
      current_angle[j] = current_angle[j] + delta_angle[j];
      // Serial.print(int(current_angle[j]));Serial.print("  ");
    }
    // Serial.println();
    servo_direct_mode( int(current_angle[0]), int(current_angle[1]), int(current_angle[2]) );
    delay(25);
  }
  for(int k=0;k<SERVO_COUNT;k++){
    current_angle[k] = target_angle[k];
  }
  servo_direct_mode( int(current_angle[0]), int(current_angle[1]), int(current_angle[2]) );
  // Serial.println();
}



////自定义舵机库函数部分-----------------------------------------------------------------------------------------------------------------------
// void servo_init(){
//   for(int i=0;i<SERVO_COUNT; i++){
//     pinMode(servo_pin[i], OUTPUT);
//     servopulse(servo_pin[i], servo_angle[i]);
//     delay(50);
//   }
//   for(int i=0;i<10;i++){
//     servopulse(servo_pin[i], servo_angle[i]);
//     delay(50);
//   }
// }

// void servo_test(){
//   for(int i=0;i<3;i++){
//     servopulse(servo_pin[i], servo_angle[i]);
//     delay(350);
//   }
// }

// void claw_open(){
//   // servopulse(CLAW_OPEN);
// }

// void claw_close(){
//   // servopulse(CLAW_CLOSE);
// }

// void servo_direct_mode(int target_angle1, int target_angle2, int target_angle3){
//   // myservo1.write(target_angle1);
//   // myservo2.write(target_angle2);
//   // myservo3.write(target_angle3);
//   servopulse(servo_pin[0], target_angle1);
//   servopulse(servo_pin[1], target_angle2);
//   servopulse(servo_pin[2], target_angle3);
// }

// void servopulse(int pins, int angle)//定义一个脉冲函数
// {
//   int pulsewidth=(angle*11)+500;  //将角度转化为500-2480的脉宽值
//   digitalWrite(pins,HIGH);    //将舵机接口电平至高
//   delayMicroseconds(pulsewidth);  //延时脉宽值的微秒数
//   digitalWrite(pins,LOW);     //将舵机接口电平至低
//   delayMicroseconds(20000-pulsewidth);
// }