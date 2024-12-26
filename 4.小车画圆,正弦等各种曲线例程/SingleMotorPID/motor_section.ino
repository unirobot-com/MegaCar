//电机相关

//电机引脚初始化
void motor_init(){
  IO_init();
  PIDA.init();
  PIDB.init();
  PIDC.init();
  PIDD.init();
  set_vel(0.0, 0.0, 0.0);
}

//电机速度设置
void set_vel(float speed_vx, float speed_vy, float speed_vw){
  InverseKinematic(speed_vx, speed_vy, speed_vw, pwma, pwmb, pwmc, pwmd);
}

//pid速度控制相关函数
void cmdVelCb(){
  PIDA.tic();
  MotorA(PIDA.getPWM(pwma));
  wA=PIDA.getWheelRotatialSpeed();
  PIDA.toc();

  PIDB.tic();
  MotorB(PIDB.getPWM(pwmb));
  wB=PIDB.getWheelRotatialSpeed();
  PIDB.toc();

  PIDC.tic();
  MotorC(PIDC.getPWM(pwmc));
  wC=PIDC.getWheelRotatialSpeed();
  PIDC.toc();

  PIDD.tic();
  MotorD(PIDD.getPWM(pwmd));
  wD=PIDD.getWheelRotatialSpeed();
  PIDD.toc();
  lastctrl=millis();
}

void car_move_mode(int mode, int delay_time){
  unsigned long current_time = millis();
  while( (millis() - current_time) < delay_time ){
    switch(mode){
      case FORWARD:    { set_vel(CAR_MOVE_VX_FORWARD_SPEED_SLOWLY, 0.0, 0.0);           } break; //forward
      case BACK:       { set_vel(CAR_MOVE_VX_BACK_SPEED_SLOWLY,    0.0, 0.0);           } break; //back
      case TURN_LEFT:  { set_vel(0.0, 0.0, CAR_MOVE_VW_LEFT_TURN_SPEED_SLOWLY);         } break; //left
      case TURN_RIGHT: { set_vel(0.0, 0.0, CAR_MOVE_VW_RIGHT_TURN_SPEED_SLOWLY);        } break; //right
      case TRANS_LEFT: { set_vel(0.0, CAR_MOVE_VY_LEFT_TRANSLATION_SPEED_SLOWLY, 0.0);  } break; //trans_left
      case TRANS_RIGHT:{ set_vel(0.0, CAR_MOVE_VY_RIGHT_TRANSLATION_SPEED_SLOWLY, 0.0); } break; //trans_right
      case CAR_STOP:   { set_vel(0.0, 0.0, 0.0); } break; //stop
    }
    cmdVelCb();
    delay(10);
  }
}

//小车运动测试
void car_move_test(){
  if( (millis() - change_motor_action_time) > 3000 ){
    car_move_status += 1;
    change_motor_action_time = millis();
    switch(car_move_status){
      case 1: {set_vel( -0.1,  0.0,  0.0);} break;  //forward
      case 2: {set_vel(  0.0,  0.0,  0.0);} break;  //stop
      case 3: {set_vel(  0.1,  0.0,  0.0);} break;  //back
      case 4: {set_vel(  0.0,  0.0,  0.0);} break;  //stop
      case 5: {set_vel(  0.0,  0.0,  0.5);} break;  //left
      case 6: {set_vel(  0.0,  0.0,  0.0);} break;  //stop
      case 7: {set_vel(  0.0,  0.0, -0.5);} break;  //right
      case 8: {set_vel(  0.0,  0.0,  0.0);} break;  //stop
      case 9: {set_vel(  0.0,  0.1,  0.0);} break;  //left_translation
      case 10: {set_vel( 0.0,  0.0,  0.0);} break;  //stop
      case 11: {set_vel( 0.0, -0.1,  0.0);} break;  //right_translation
      case 12: {set_vel( 0.0,  0.0,  0.0);} break;  //stop
      default: break;
    }
    if(car_move_status >= 12){
      car_move_status = 0;
    }
  }
}

