  
//编码电机引脚初始化
void motor_init(){
  motor1.init(); //编码电机1初始化设置
  motor2.init(); //编码电机2初始化设置
  motor3.init(); //编码电机3初始化设置
  motor4.init(); //编码电机4初始化设置
}


//更新电机参数
void motor_update(){
  motor1.update();
  motor2.update();
  motor3.update();
  motor4.update();
  delay(10);//由于电机库中电机周期设置的时间为10毫秒，故这里的延时不可删除.如若想要修改此处延时，需修改库中电机周期
}


//小车前进
void car_forward(){
  motor1.set_vel(motor_speed_lf);
  motor2.set_vel(-motor_speed_rf);
  motor3.set_vel(motor_speed_lb);
  motor4.set_vel(-motor_speed_rb);
}


//小车后退
void car_back(){
  motor1.set_vel(-motor_speed_lf);
  motor2.set_vel(motor_speed_rf);
  motor3.set_vel(-motor_speed_lb);
  motor4.set_vel(motor_speed_rb);
}


//小车左转
void car_left(){
  motor1.set_vel(motor_speed_lf);
  motor2.set_vel(motor_speed_rf);
  motor3.set_vel(motor_speed_lb);
  motor4.set_vel(motor_speed_rb);
}


//小车右转
void car_right(){ 
  motor1.set_vel(-motor_speed_lf);
  motor2.set_vel(-motor_speed_rf);
  motor3.set_vel(-motor_speed_lb);
  motor4.set_vel(-motor_speed_rb);
}


//小车左平移
void car_trans_left(){
  motor1.set_vel(-motor_speed_lf);
  motor2.set_vel(-motor_speed_rf);
  motor3.set_vel(motor_speed_lb);
  motor4.set_vel(motor_speed_rb);
}


//小车右平移
void car_trans_right(){
  motor1.set_vel(motor_speed_lf);
  motor2.set_vel(motor_speed_rf);
  motor3.set_vel(-motor_speed_lb);
  motor4.set_vel(-motor_speed_rb);
}

//小车停止
void car_stop(){
  motor1.set_vel(0);
  motor2.set_vel(0);
  motor3.set_vel(0);
  motor4.set_vel(0);
}


/*小车按照mode模式运行多长时间
mode: FORWARD:前进    BACK:后退  LEFT:左转  RIGHT:右转  
      TRANS_LEFT:左平移  TRANS_RIGHT:右平移  STOP:小车停止
time_delay:按照上面的mode模式，小车需要运行多长时间，单位毫秒 */
void motor_move(int mode, unsigned long time_delay){
  unsigned long time_now = millis();
  switch(mode){
    case FORWARD:{ while( (millis() - time_now) < time_delay ){
        car_forward();
        motor_update();
      }
    } break;
    case BACK:{ while( (millis() - time_now) < time_delay ){
        car_back();
        motor_update();
      }
    } break;
    case LEFT:{ while( (millis() - time_now) < time_delay ){
        car_left();
        motor_update();
      }
    } break;
    case RIGHT:{ while( (millis() - time_now) < time_delay ){
        car_right();
        motor_update();
      }
    } break;
    case TRANS_LEFT:{ while( (millis() - time_now) < time_delay ){
        car_trans_left();
        motor_update();
      }
    } break;
    case TRANS_RIGHT:{ while( (millis() - time_now) < time_delay ){
        car_trans_right();
        motor_update();
      }
    } break;
    case STOP:{ while( (millis() - time_now) < time_delay ){
        car_stop();
        motor_update();
      }
    } break;
    default: break;
  }
}


//电机动作测试
void motor_move_action_test(){
  motor_move(FORWARD, action_delay);    //前进5秒
  motor_move(STOP, 1000);               //停止1秒

  motor_move(BACK, action_delay);       //后退5秒
  motor_move(STOP, 1000);               //停止1秒

  motor_move(LEFT, action_delay);       //左转5秒
  motor_move(STOP, 1000);               //停止1秒

  motor_move(RIGHT, action_delay);      //右转5秒
  motor_move(STOP, 1000);               //停止1秒

  motor_move(TRANS_LEFT, action_delay); //左平移5秒
  motor_move(STOP, 1000);               //停止1秒

  motor_move(TRANS_RIGHT, action_delay);//右平移5秒
  motor_move(STOP, 1000);               //停止1秒
}



