#define joystick_l_x_middle 128
#define joystick_l_y_middle 127
#define joystick_l_x_middle_left  100 //左侧摇杆模块-x轴-横向-左侧数值
#define joystick_l_x_middle_right 156 //左侧摇杆模块-x轴-横向-左侧数值

#define joystick_r_x_middle 128
#define joystick_r_y_middle 127
#define joystick_r_x_middle_left  100 //右侧摇杆模块-x轴-横向-左侧数值
#define joystick_r_x_middle_right 156 //右侧摇杆模块-x轴-横向-右侧数值
#define joystick_r_y_middle_up    100 //右侧摇杆模块-y轴-纵向-上侧数值
#define joystick_r_y_middle_down  156 //右侧摇杆模块-y轴-纵向-下侧数值
#define joystick_max_data 255

#define motor_vx_max_speed 255  //vx方向轮子最小速度
#define motor_vx_min_speed 100
#define motor_vx_zero_speed 0


#define PS2_DAT        52  //MISO
#define PS2_CMD        51  //MOSI
#define PS2_SEL        53  //SS
#define PS2_CLK        50  //SLKGHH
float motor_speed_coefficient = 0.1;
/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures   false
#define rumble      false
PS2X ps2x; // create PS2 Controller Class
int error = -1;
byte type = 0;
byte vibrate = 0;
int tryNum = 1;

int joystick_lx = 0;
int joystick_ly = 0;
int joystick_rx = 0;
int joystick_ry = 0;
int vx_flag = 0;
int vy_flag = 0;
int vw_flag = 0;

//PS2 Key
void ps2_init(){
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  if (error == 0) {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }
  else if (error == 1)
  {
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
    // resetFunc();
    
  }

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  type = ps2x.readType();
  switch (type) {
  case 0:
    Serial.print("Unknown Controller type found ");
    break;
  case 1:
    Serial.print("DualShock Controller found ");
    break;
  case 2:
    Serial.print("GuitarHero Controller found ");
    break;
  case 3:
    Serial.print("Wireless Sony DualShock Controller found ");
    break;
  }
}

void ps2_mode_change(){
  ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
  if(ps2x.ButtonPressed(PSB_START)){
    Serial.println("Start is being held");
    car_mode_change = car_mode_change + 1;
  }
}

void while_game_over(){
  while(while_ps2_press){
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    if(ps2x.ButtonPressed(PSB_START)){
      // Serial.println("Start is being held");
      car_mode_change = 1;
      while_ps2_press = false;
    }
  }
}

void get_ps_key(){
  // if(type == 1){ //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    // //will be TRUE as long as button is pressed
    // if(ps2x.Button(PSB_START)){
    //   Serial.println("Start is being held");
    // }
    // if(ps2x.Button(PSB_SELECT)){
    //   Serial.println("Select is being held");
    // }

    // //will be TRUE as long as button is pressed
    if(ps2x.Button(PSB_PAD_UP)) {
      delay(servo_action_delay);
      servo1_angle += servo_step;
      // servo3_angle+=servo_step;
      // Serial.print("Up held this hard: ");
      // Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
    }
    if(ps2x.Button(PSB_PAD_RIGHT)){
      delay(servo_action_delay);
      servo2_angle -= servo_step;
      // servo2_angle-=servo_step;
      // Serial.print("Right held this hard: ");
      // Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
    }
    if(ps2x.Button(PSB_PAD_LEFT)){
      delay(servo_action_delay);
      servo2_angle += servo_step;
      // servo2_angle+=servo_step;
      // Serial.print("LEFT held this hard: ");
      // Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
    }
    if(ps2x.Button(PSB_PAD_DOWN)){
      delay(servo_action_delay);
      servo1_angle -= servo_step;
      // servo3_angle-=servo_step;
      // Serial.print("DOWN held this hard: ");
      // Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    }

    // vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    // // if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
    //   if(ps2x.Button(PSB_L3)){
    //     Serial.println("L3 pressed");
    //   }
    //   if(ps2x.Button(PSB_R3)){
    //     Serial.println("R3 pressed");
    //   }
    if(ps2x.ButtonPressed(PSB_L2)){
      // Serial.println("L2 pressed");
      delay(10);
      motor_speed_coefficient = motor_speed_coefficient + 0.1;
      if(motor_speed_coefficient > 2.0){
        motor_speed_coefficient = 2.0;
      }
      if(motor_speed_coefficient < 0){
        motor_speed_coefficient = 0;
      }
    }
    if(ps2x.ButtonPressed(PSB_R2)){
      delay(10);
      motor_speed_coefficient = motor_speed_coefficient - 0.1;
      // Serial.println("R2 pressed");
      if(motor_speed_coefficient > 2.0){
        motor_speed_coefficient = 2.0;
      }
      if(motor_speed_coefficient < 0){
        motor_speed_coefficient = 0;
      }
    }

    // if(ps2x.Button(PSB_TRIANGLE)){
    //   Serial.println("△ pressed");
    //   // servo4_angle+= servo_step;
    // }
    //△□○×
    // if(ps2x.ButtonPressed(PSB_CIRCLE)){               //will be TRUE if button was JUST pressed
    if(ps2x.Button(PSB_CIRCLE)){               //will be TRUE if button was JUST pressed
      delay(servo_action_delay);
      Serial.println("○ just pressed");
      servo3_angle -= servo_step;
    }
    // if(ps2x.NewButtonState(PSB_CROSS)){              //will be TRUE if button was JUST pressed OR released
    // if(ps2x.Button(PSB_CROSS)){              //will be TRUE if button was JUST pressed OR released
    //   Serial.println("x just changed");
    //     // servo4_angle-=servo_step;
    // }
    // if(ps2x.ButtonReleased(PSB_SQUARE)){             //will be TRUE if button was JUST released
    if(ps2x.Button(PSB_SQUARE)){             //will be TRUE if button was JUST released
      delay(servo_action_delay);
      Serial.println("□ just released");
      servo3_angle += servo_step;
    }

    joystick_lx = ps2x.Analog(PSS_LX);
    joystick_ly = ps2x.Analog(PSS_LY);
    joystick_rx = ps2x.Analog(PSS_RX);
    joystick_ry = ps2x.Analog(PSS_RY);
    // Serial.print(joystick_lx);Serial.print(" ");
    // Serial.print(joystick_ly);Serial.print(" ");
    // Serial.print(joystick_rx);Serial.print(" ");
    // Serial.println(joystick_ry);
    car_set_vel_joystick_right(joystick_ry, joystick_lx, joystick_rx);
}

void car_set_vel_joystick_right(int vx, int vy, int vw){
  //-----------------------------------------------------------------------------------------
  if( (vw>=0) && (vw < joystick_r_x_middle_left) ){
    vw = map(vw, 0, joystick_r_x_middle_left, motor_vx_max_speed, motor_vx_min_speed);
    vw_flag = 1;
  }
  else if( (vw >= joystick_r_x_middle_left) && (vw <= joystick_r_x_middle_right) ){
    vw = 0;
    vw_flag = 0;
  }
  else{
    vw = map(vw, joystick_r_x_middle_right, joystick_max_data, motor_vx_min_speed, motor_vx_max_speed);
    vw_flag = -1;
  }

  //-----------------------------------------------------------------------------------------
  if( (vx>=0) && (vx < joystick_r_y_middle_up) ){
    vx = map(vx, 0, joystick_r_y_middle_up, motor_vx_max_speed, motor_vx_min_speed);
    vx_flag = 1;
  }
  else if( (vx >= joystick_r_y_middle_up) && (vx <= joystick_r_y_middle_down) ){
    vx = 0;
    vx_flag = 0;
  }
  else{
    vx = map(vx, joystick_r_y_middle_down, joystick_max_data, motor_vx_min_speed, motor_vx_max_speed);
    vx_flag = -1;
  }

  //=====================================================================================================
  if( (vy>=0) && (vy < joystick_l_x_middle_left) ){
    vy = map(vy, 0, joystick_l_x_middle_left, motor_vx_max_speed, motor_vx_min_speed);
    vy_flag = -1;
  }
  else if( (vy >= joystick_l_x_middle_left) && (vy <= joystick_l_x_middle_right) ){
    vy = 0;
    vy_flag = 0;
  }
  else{
    vy = map(vy, joystick_l_x_middle_right, joystick_max_data, motor_vx_min_speed, motor_vx_max_speed);
    vy_flag = 1;
  }

  //-----------------------------------------------------------------------------------------
  if( (vx>0) && (vy == 0) && (vw == 0)){
    vx = vx * vx_flag;
  }
  else if( (vx == 0) && (vy > 0) && (vw == 0) ){
    vy = vy * vy_flag;
  }
  else if( (vx == 0) && (vy == 0) && (vw > 0) ){
    vw = vw * vw_flag;    
  }
  else{
    vx = 0;
    vy = 0;
    vw = 0;
  }
  
  // Serial.print("vx:");Serial.print(vx);Serial.print(" ");
  // Serial.print("vy:");Serial.print(vy);Serial.print(" ");
  // Serial.print("vw:");Serial.println(vw);
  
  // int v1 = int( (vx + vy + vw) * motor_speed_coefficient ) ;        //lf
  // int v2 = int( (vx - vy - vw) * motor_speed_coefficient ) ;   //rf
  // int v3 = int( (vx - vy + vw) * motor_speed_coefficient ) ;         //lb
  // int v4 = int( (vx + vy - vw) * motor_speed_coefficient ) ;   //rb


  float v1,v2,v3,v4;
  float dir1 = (vx + vy + vw);
  float dir2 = (vx - vy - vw);
  float dir3 = (vx - vy + vw);
  float dir4 = (vx + vy - vw);

  if(dir1 >0){ dir1 = 1;}else if(dir1==0){dir1 = 0;}else{dir1 = -1;}
  if(dir2 >0){ dir2 = 1;}else if(dir2==0){dir2 = 0;}else{dir2 = -1;}
  if(dir3 >0){ dir3 = 1;}else if(dir3==0){dir3 = 0;}else{dir3 = -1;}
  if(dir4 >0){ dir4 = 1;}else if(dir4==0){dir4 = 0;}else{dir4 = -1;}
  v1 = - dir1 * motor_speed_coefficient;
  v2 = - dir2 * motor_speed_coefficient;
  v3 = - dir3 * motor_speed_coefficient;
  v4 = - dir4 * motor_speed_coefficient;
  // Serial.print(v1);Serial.print(" | ");
  // Serial.print(v2);Serial.print(" | ");
  // Serial.print(v3);Serial.print(" | ");
  // Serial.println(v4);
  PIDA.tic();
  MotorA(PIDA.getPWM(v1));
  wA=PIDA.getWheelRotatialSpeed();
  PIDA.toc();

  PIDB.tic();
  MotorB(PIDB.getPWM(v2));
  wB=PIDB.getWheelRotatialSpeed();
  PIDB.toc();

  PIDC.tic();
  MotorC(PIDC.getPWM(v3));
  wC=PIDC.getWheelRotatialSpeed();
  PIDC.toc();

  PIDD.tic();
  MotorD(PIDD.getPWM(v4));
  wD=PIDD.getWheelRotatialSpeed();
  PIDD.toc();
  lastctrl=millis();
  // Serial.print("v1:");Serial.print(v1);Serial.print(" ");
  // Serial.print("v2:");Serial.print(v2);Serial.print(" ");
  // Serial.print("v3:");Serial.print(v3);Serial.print(" ");
  // Serial.print("v4:");Serial.println(v4);
  // Serial.println(motor_speed_coefficient);
  // motor_move(v1, v2, v3, v4);
  servo1_angle = (servo1_angle >= 100) ? 100 : ((servo1_angle <= 25) ? 25 : servo1_angle);
  servo2_angle = (servo2_angle >= 100) ? 100 : ((servo2_angle <= 25) ? 25 : servo2_angle);
  servo3_angle = (servo3_angle >= 90) ? 90 : ((servo3_angle <= 10) ? 10 : servo3_angle);
  servo_direct_mode(servo1_angle, servo2_angle, servo3_angle);
  // motor_move(80, 80, 80, 80); delay(1000);
  // motor_move(0, 0, 0, 0); delay(1000);
  // cmdVelCb();
  delay(10);
}
