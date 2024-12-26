//程序管理部分

void program_init(){
  //init section
  motor_init();   //agv底盘小车引脚初始化
  k210_serial_init();   //k210串口初始化
  servo_init();         //舵机引脚初始化
  change_motor_action_time = millis();
  lastctrl=millis();
  Serial.println("judge_isnot_debug");delay(program_systemctl_time);

  //debug section
  #ifdef DEBUG
    Serial.println("The program enters debugs modes");
    delay(program_systemctl_time);
    while(1){
      Serial.println("debug");
      #ifdef DEBUG_MOTOR
        car_move_test();
        cmdVelCb();
        delay(10);
      #endif

      #ifdef DEBUG_SERVO
        servo_test();
      #endif

      #ifdef DEBUG_MOTOR_SERVO
        motor_and_servo_test();
        cmdVelCb();
        delay(10);
      #endif

      #ifdef DEBUG_K210
        k210_test();
        delay(10);
      #endif

      #ifdef DEBUG_MOTOR_SERVO_K210
        motor_and_servo_test();
        k210_test();
        cmdVelCb();
        delay(10);
      #endif
    }
  #endif
  Serial.println("The program has not entered debugging mode");
  delay(program_systemctl_time);
}

//================================================================================================================================================
//小车进入寻找物体程序=============================================================================================================================
//================================================================================================================================================

/*
void search_for_something(){
  while(search_for_ballon_flag){
    wk->update_data();//更新消息
    bool rt = wk->recive_box(result , K210_FIND_RED_FOLLOW);//获取消息
    if(rt) //若获取到新的消息
    {
      // Serial.println("i find sth");
      rt = false;
      find_once_ballon_flag = true;
    }
    if(find_once_ballon_flag){
      search_for_ballon_flag = false;
      tracking_something();
    }
    else{
      // Serial.println("else");
      find_once_ballon_flag = false;
      set_target_vx = CAR_MOVE_STOP_SPEED;
      set_target_vy = CAR_MOVE_STOP_SPEED;
      set_target_vw = CAR_MOVE_VW_LEFT_TURN_SPEED_SLOWLY;
    }
    set_vel(set_target_vx, set_target_vy, set_target_vw);
    cmdVelCb();
    delay(10);
  }
}

//================================================================================================================================================
//小车进入追踪物体程序=============================================================================================================================
//================================================================================================================================================
void tracking_something(){
  while(find_once_ballon_flag){
    // Serial.println("tracking_something");
    wk->update_data();//更新消息
    bool rt = wk->recive_box(result , K210_FIND_RED_FOLLOW);//获取消息
    if(rt) //若获取到新的消息
    {
      rt = false;
      int data_x = result->x + (result->w)/2;
      int data_y = result->y + (result->h)/2;

      //小车进入Vy方向路径判断
      if( (data_x >=0) && (data_x<128)){
        car_move_vy_direction = false;
        set_target_vx = CAR_MOVE_STOP_SPEED;
        set_target_vy = CAR_MOVE_VY_LEFT_TRANSLATION_SPEED_SLOWLY;
        set_target_vw = CAR_MOVE_STOP_SPEED;
      }
      else if( (data_x>=128) && (data_x<=192) ){
        car_move_vy_direction = true;
        set_target_vy = CAR_MOVE_STOP_SPEED;
        set_target_vw = CAR_MOVE_STOP_SPEED;
      }
      else if( (data_x>192) && (data_x<=320) ){
        car_move_vy_direction = false;
        set_target_vx = CAR_MOVE_STOP_SPEED;
        set_target_vy = CAR_MOVE_VY_RIGHT_TRANSLATION_SPEED_SLOWLY;
        set_target_vw = CAR_MOVE_STOP_SPEED;
      }

      //小车进入Vx方向路径判断
      if(car_move_vy_direction)
      {
        if( (data_y>=0) && (data_y<200) ){
          set_target_vx = CAR_MOVE_VX_FORWARD_SPEED_SLOWLY;
          set_target_vy = CAR_MOVE_STOP_SPEED;
        }
        else if( (data_y>=200) && (data_y<=240) ){
          set_target_vx = CAR_MOVE_STOP_SPEED;
          prepare_process_something();
          // search_for_ballon_flag = true;
          // find_once_ballon_flag = false;
        }
      }
    }
    set_vel(set_target_vx, set_target_vy, set_target_vw);
    // Serial.print("vx:");Serial.print(set_target_vx);Serial.print(" ");
    // Serial.print("vy:");Serial.print(set_target_vy);Serial.print(" ");
    // Serial.print("vw:");Serial.println(set_target_vw);
    cmdVelCb();
    delay(10);
  }
  // Serial.print("-------------over--------------");delay(500);
}

void prepare_process_something(){
  search_for_field_flag = true;
  Serial.println("car stop 2s");
  car_move_mode(CAR_STOP, 2000);

  servo_uniform_mode(30,  100, 40, SERVO_STEP_TIME);   //爪子复位
  servo_uniform_mode(30,  100, 40, SERVO_STEP_TIME);   //臂爪放下
  servo_uniform_mode(100, 100, 35, SERVO_STEP_TIME);   //爪子闭合
  servo_uniform_mode(100, 100, 35, SERVO_STEP_TIME);   //爪子抓住物体回复到原位置

  Serial.println("turn_left 3s");
  car_move_mode(TURN_LEFT, 3000);
  Serial.println("car stop 1s");
  car_move_mode(CAR_STOP, 1000);
  // // Serial.println("forward 3s");
  // // car_move_mode(TURN_LEFT, 3000);

  // servo_uniform_mode(100,  100, 50, SERVO_STEP_TIME);  //准备放下物体
  // servo_uniform_mode(30,   100, 50, SERVO_STEP_TIME);  //爪子张开
  // servo_uniform_mode(30,  30,  50, SERVO_STEP_TIME);   //爪子复位
  // car_move_mode(TURN_LEFT, 2000);

  // wk->clear_rec();

  search_for_field();
}

void search_for_field(){
  // search_for_field_flag = true;
  while(search_for_field_flag){
    wk->update_data();//更新消息
    bool rts = wk->recive_box(result , K210_COLOR_SORTING);//获取消息
    // bool rts = wk->recive_box(result , K210_FIND_RED_FOLLOW);//获取消息
    if(rts){
      rts = false;
      find_once_field_flag = true;
      // find_something_coordinate();
    }
    if(find_once_field_flag){
      Serial.println("field_true");
      find_something_coordinate();
    }
    else{
      // Serial.println("field_false");
      find_once_field_flag = false;
      set_target_vx = CAR_MOVE_STOP_SPEED;
      set_target_vy = CAR_MOVE_STOP_SPEED;
      set_target_vw = CAR_MOVE_VW_LEFT_TURN_SPEED_SLOWLY;
    }
    set_vel(set_target_vx, set_target_vy, set_target_vw);
    cmdVelCb();
    delay(10);
  }
}

void find_something_coordinate(){
  while(find_once_field_flag){
    wk->update_data();//更新消息
    bool rts = wk->recive_box(result , K210_COLOR_SORTING);//获取消息
    int data_x = result->x + (result->w)/2;
    int data_y = result->y + (result->h)/2;
    //小车进入Vy方向路径判断
    if( (data_x >=0) && (data_x<128)){
      car_move_vy_direction = false;
      set_target_vy = CAR_MOVE_VY_LEFT_TRANSLATION_SPEED_SLOWLY;
    }
    else if( (data_x>=128) && (data_x<=192) ){
      car_move_vy_direction = true;
      set_target_vy = CAR_MOVE_STOP_SPEED;
    }
    else if( (data_x>192) && (data_x<=320) ){
      car_move_vy_direction = false;
      set_target_vy = CAR_MOVE_VY_RIGHT_TRANSLATION_SPEED_SLOWLY;
    }
    //小车进入Vx方向路径判断
    if(car_move_vy_direction)
    {
      if( (data_y>=0) && (data_y<180) ){
        set_target_vx = CAR_MOVE_VX_FORWARD_SPEED_SLOWLY;
      }
      else if( (data_y>=180) && (data_y<=240) ){
        set_target_vx = CAR_MOVE_STOP_SPEED;
        put_down_object();
        search_for_field_flag = false;
        // search_for_ballon_flag = true;
        find_once_field_flag = false;
      }
    }
    // wk->clear_rec();
  }
}

void put_down_object(){
  car_move_mode(CAR_STOP, 2000);
  servo_uniform_mode(100,  100, 35, SERVO_STEP_TIME);  //准备放下物体
  servo_uniform_mode(30,   100, 35, SERVO_STEP_TIME);  //爪子张开
  servo_uniform_mode(30,   100,  40, SERVO_STEP_TIME);   //爪子复位
  car_move_mode(TURN_LEFT, 2000);
  car_move_mode(CAR_STOP, 2000);
  while_ps2_press = true;
  while_game_over();
  car_move_mode(CAR_STOP, 2000);
  search_for_ballon_flag = true;
  find_once_ballon_flag = false;
}

void motor_and_servo_test(){
  if( (millis() - change_motor_action_time) > program_systemctl_motor_debug_time ){
    car_move_status += 1;
    change_motor_action_time = millis();
    switch(car_move_status){
      case 1: {servo_direct_mode(20, 70, 85); set_vel( -0.1,  0.0,  0.0);} break;  //forward
      case 2: {set_vel(  0.0,  0.0,  0.0);} break;  //stop
      case 3: {servo_direct_mode(70, 90, 30);set_vel(  0.1,  0.0,  0.0);} break;  //back
      case 4: {set_vel(  0.0,  0.0,  0.0);} break;  //stop
      case 5: {servo_direct_mode(20, 70, 85);set_vel(  0.0,  0.0,  0.5);} break;  //left
      case 6: {set_vel(  0.0,  0.0,  0.0);} break;  //stop
      case 7: {servo_direct_mode(70, 90, 30);set_vel(  0.0,  0.0, -0.5);} break;  //right
      case 8: {set_vel(  0.0,  0.0,  0.0); } break;  //stop
      case 9: {servo_direct_mode(20, 70, 85); set_vel(  0.0,  0.1,  0.0);} break;  //left_translation
      case 10: {set_vel( 0.0,  0.0,  0.0);} break;  //stop
      case 11: {servo_direct_mode(70, 90, 30);set_vel( 0.0, -0.1,  0.0);} break;  //right_translation
      case 12: {set_vel( 0.0,  0.0,  0.0);} break;  //stop
      default: break;
    }
    if(car_move_status >= 12){
      car_move_status = 0;
    }
  }
}

*/


void search_for_ballon(){
  while(search_for_ballon_flag){
    wk->update_data();//更新消息
    bool rt = wk->recive_box(result , K210_FIND_RED_FOLLOW);//获取消息
    if(rt) //若获取到新的消息
    {
      rt = false;
      find_once_ballon_flag = true;
    }
    if(find_once_ballon_flag){
      find_something_ballon();
      // Serial.println("ballon_true");
      // Serial.println("find");
    }
    else{
      // Serial.println("left");
      // Serial.println("ballon_true");
      find_once_ballon_flag = false;
      set_target_vx = CAR_MOVE_STOP_SPEED;
      set_target_vy = CAR_MOVE_STOP_SPEED;
      set_target_vw = CAR_MOVE_VW_LEFT_TURN_SPEED_SLOWLY;
    }
    set_vel(set_target_vx, set_target_vy, set_target_vw);
    cmdVelCb();
    delay(10);
  }
}

void find_something_ballon(){
    int data_x = result->x + (result->w)/2;
    int data_y = result->y + (result->h)/2;

    //小车进入Vy方向路径判断
    if( (data_x >=0) && (data_x<128)){
      set_target_vx = CAR_MOVE_STOP_SPEED;
      set_target_vy = CAR_MOVE_VY_LEFT_TRANSLATION_SPEED_SLOWLY;
      set_target_vw = CAR_MOVE_STOP_SPEED;
      car_move_vy_direction = false;
    }
    else if( (data_x>=128) && (data_x<=192) ){
      car_move_vy_direction = true;
      set_target_vx = CAR_MOVE_STOP_SPEED;
      set_target_vy = CAR_MOVE_STOP_SPEED;
      set_target_vw = CAR_MOVE_STOP_SPEED;
    }
    else if( (data_x>192) && (data_x<=320) ){
      car_move_vy_direction = false;
      set_target_vx = CAR_MOVE_STOP_SPEED;
      set_target_vy = CAR_MOVE_VY_RIGHT_TRANSLATION_SPEED_SLOWLY;
      set_target_vw = CAR_MOVE_STOP_SPEED;
    }

    //小车进入Vx方向路径判断
    if(car_move_vy_direction)
    {
      if( (data_y>=0) && (data_y<220) ){
        set_target_vx = CAR_MOVE_VX_FORWARD_SPEED_SLOWLY;
        set_target_vy = CAR_MOVE_STOP_SPEED;
        set_target_vw = CAR_MOVE_STOP_SPEED;
      }
      else if( (data_y>=220) && (data_y<=240) ){
        set_target_vx = CAR_MOVE_STOP_SPEED;
        set_target_vy = CAR_MOVE_STOP_SPEED;
        set_target_vw = CAR_MOVE_STOP_SPEED;
        pick_up_object(1250);
        search_for_ballon_flag = false;
        search_for_field_flag = true;
        find_once_ballon_flag = false;
      }
    }
}

//================================================================================================================================================
//小车寻找红色场地=================================================================================================================================
//================================================================================================================================================
void search_for_field(){
  while(search_for_field_flag){
    wk->update_data();//更新消息
    bool rts = wk->recive_box(result , K210_COLOR_SORTING);//获取消息
    if(rts){
      rts = false;
      find_once_field_flag = true;
      // find_something_coordinate();
    }
    if(find_once_field_flag){
      // Serial.println("field_true");
      find_something_coordinate();
    }
    else{
      // Serial.println("field_false");
      find_once_field_flag = false;
      set_target_vx = CAR_MOVE_STOP_SPEED;
      set_target_vy = CAR_MOVE_STOP_SPEED;
      set_target_vw = CAR_MOVE_VW_LEFT_TURN_SPEED_SLOWLY;
    }
    set_vel(set_target_vx, set_target_vy, set_target_vw);
    cmdVelCb();
    delay(10);
  }
}

void find_something_coordinate(){
    int data_x = result->x + (result->w)/2;
    int data_y = result->y + (result->h)/2;
    //小车进入Vy方向路径判断
    if( (data_x >=0) && (data_x<128)){
      car_move_vy_direction = false;
      set_target_vx = CAR_MOVE_STOP_SPEED;
      set_target_vy = CAR_MOVE_VY_LEFT_TRANSLATION_SPEED_SLOWLY;
      set_target_vw = CAR_MOVE_STOP_SPEED;
    }
    else if( (data_x>=128) && (data_x<=192) ){
      car_move_vy_direction = true;
      set_target_vx = CAR_MOVE_STOP_SPEED;
      set_target_vy = CAR_MOVE_STOP_SPEED;
      set_target_vw = CAR_MOVE_STOP_SPEED;
    }
    else if( (data_x>192) && (data_x<=320) ){
      car_move_vy_direction = false;
      set_target_vx = CAR_MOVE_STOP_SPEED;
      set_target_vy = CAR_MOVE_VY_RIGHT_TRANSLATION_SPEED_SLOWLY;
      set_target_vw = CAR_MOVE_STOP_SPEED;
    }
    //小车进入Vx方向路径判断
    if(car_move_vy_direction)
    {
      if( (data_y>=0) && (data_y<180) ){
        set_target_vx = CAR_MOVE_VX_FORWARD_SPEED_SLOWLY;
        set_target_vy = CAR_MOVE_STOP_SPEED;
        set_target_vw = CAR_MOVE_STOP_SPEED;
      }
      else if( (data_y>=180) && (data_y<=240) ){
        set_target_vx = CAR_MOVE_STOP_SPEED;
        set_target_vy = CAR_MOVE_STOP_SPEED;
        set_target_vw = CAR_MOVE_STOP_SPEED;
        put_down_object(1250);
        search_for_field_flag = false;
        search_for_ballon_flag = true;
        find_once_field_flag = false;
      }
    }
    // wk->clear_rec();
}


void pick_up_object(unsigned long delay_time){
  //让小车停下来
  unsigned long time_now = millis();
  set_target_vy = CAR_MOVE_STOP_SPEED;
  set_target_vw = CAR_MOVE_STOP_SPEED;
  set_target_vx = CAR_MOVE_STOP_SPEED;
  set_vel(set_target_vx, set_target_vy, set_target_vw);
  while( (millis() - time_now) < 2000 ){
    cmdVelCb();
    delay(10);
  }

  servo_uniform_mode(100, 100, 35, SERVO_STEP_TIME);delay(2000);
  // claw_close();delay(2000);
  // claw_open();delay(2000);
  wk->clear_rec();
}

void put_down_object(unsigned long delay_time){
  //让小车停下来
  unsigned long time_now = millis();
  set_target_vy = CAR_MOVE_STOP_SPEED;
  set_target_vw = CAR_MOVE_STOP_SPEED;
  set_target_vx = CAR_MOVE_STOP_SPEED;
  set_vel(set_target_vx, set_target_vy, set_target_vw);
  while( (millis() - time_now) < 2000 ){
    cmdVelCb();
    delay(10);
  }
  // claw_open();delay(2000);
  servo_uniform_mode(30, 100, 35, SERVO_STEP_TIME);delay(2000);
  wk->clear_rec();

  far_away_put_area(3000, 3200);
}

void far_away_put_area(unsigned long back_delay_time, unsigned long left_delay_time){
  //让小车停下来
  unsigned long time_now = millis();
  set_target_vy = CAR_MOVE_STOP_SPEED;
  set_target_vw = CAR_MOVE_STOP_SPEED;
  set_target_vx = CAR_MOVE_STOP_SPEED;
  set_vel(set_target_vx, set_target_vy, set_target_vw);
  while( (millis() - time_now) < 2000 ){
    cmdVelCb();
    delay(10);
  }
  wk->clear_rec();

  time_now = millis();
  set_target_vy = CAR_MOVE_STOP_SPEED;
  set_target_vw = CAR_MOVE_STOP_SPEED;
  set_target_vx = CAR_MOVE_VX_BACK_SPEED_SLOWLY;
  set_vel(set_target_vx, set_target_vy, set_target_vw);
  while( (millis() - time_now) < back_delay_time ){
    cmdVelCb();
    delay(10);
  }
  wk->clear_rec();

  time_now = millis();
  set_target_vy = CAR_MOVE_STOP_SPEED;
  set_target_vw = CAR_MOVE_STOP_SPEED;
  set_target_vx = CAR_MOVE_STOP_SPEED;
  set_vel(set_target_vx, set_target_vy, set_target_vw);
  while( (millis() - time_now) < 2000 ){
    cmdVelCb();
    delay(10);
  }
  wk->clear_rec();


  time_now = millis();
  set_target_vy = CAR_MOVE_STOP_SPEED;
  set_target_vw = CAR_MOVE_VW_LEFT_TURN_SPEED_SLOWLY;
  set_target_vx = CAR_MOVE_STOP_SPEED;
  set_vel(set_target_vx, set_target_vy, set_target_vw);
  while( (millis() - time_now) < left_delay_time ){
    cmdVelCb();
    delay(10);
  }
  wk->clear_rec();

  time_now = millis();
  set_target_vy = CAR_MOVE_STOP_SPEED;
  set_target_vw = CAR_MOVE_STOP_SPEED;
  set_target_vx = CAR_MOVE_STOP_SPEED;
  set_vel(set_target_vx, set_target_vy, set_target_vw);
  while( (millis() - time_now) < 2000 ){
    cmdVelCb();
    delay(10);
  }
  wk->clear_rec();
}

// void claw_open(){
//   servopulse(CLAW_OPEN);
// }

// void claw_close(){
//   servopulse(CLAW_CLOSE);
// }

// void servopulse(int angle)//定义一个脉冲函数
// {
//   int pulsewidth=(angle*11)+500;  //将角度转化为500-2480的脉宽值
//   digitalWrite(CLAW_PIN,HIGH);    //将舵机接口电平至高
//   delayMicroseconds(pulsewidth);  //延时脉宽值的微秒数
//   digitalWrite(CLAW_PIN,LOW);     //将舵机接口电平至低
//   delayMicroseconds(20000-pulsewidth);
// }