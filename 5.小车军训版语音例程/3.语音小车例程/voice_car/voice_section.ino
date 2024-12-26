//语音模块函数部分---------------------------------------
//语音模块初始化
void voice_init(){
  voice_ser.begin(voice_rate);
}

////小车接收语音指令，执行相应动作
void get_voice(){
  reveive_voice = voice_ser.read();
  if (!(reveive_voice == -1)) {
    voice_section();
  }
  else{
    delay(100);
  }
}

void voice_section() {
  //Serial.println(reveive_voice);
  if (reveive_voice == 1) {     //如果接收到前进语音，小车前进
    Serial.println("forward");
    motor_move(FORWARD, action_delay);
    motor_move(STOP, 1000);
  }
  else if (reveive_voice == 2) {  //如果接收到后退语音，小车后退
    Serial.println("back");
    motor_move(BACK, action_delay);
    motor_move(STOP, 1000);
  }
  else if (reveive_voice == 3) {  //如果接收到左转语音，小车左转
    Serial.println("left");
    motor_move(LEFT, action_delay);
    motor_move(STOP, 1000);
  }
  else if (reveive_voice == 4) {  //如果接收到右转语音，小车右转
    Serial.println("right");
    motor_move(RIGHT, action_delay);
    motor_move(STOP, 1000);
  }
  else if (reveive_voice == 5) {  //如果接收到左平移语音，小车左平移
    Serial.println("transleft");
    motor_move(TRANS_LEFT, action_delay);
    motor_move(STOP, 1000);
  }
  else if (reveive_voice == 6) {  //如果接收到右平移语音，小车右平移
    Serial.println("transright");
    motor_move(TRANS_RIGHT, action_delay);
    motor_move(STOP, 1000);
  }
  else if (reveive_voice == 7) {  //如果接收到停止语音，小车停止
    Serial.println("stop");
    motor_move(STOP, action_delay);
    motor_move(STOP, 1000);
  }
  reveive_voice = 0;
}