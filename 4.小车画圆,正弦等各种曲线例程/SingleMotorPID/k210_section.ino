//k210部分

void k210_serial_init(){
  wk = malloc(sizeof(WonderK210));//分配内存
  //开启串口接收
  //这里不需要再调用初始化串口函数 Serial.begin(115200); 在 serial_begin() 函数里已经初始化了  
  wk->serial_begin();
  result = malloc(sizeof(Find_Box_st));//分配内存
}

void k210_test(){
  wk->update_data();//更新消息
  bool rt = wk->recive_box(result , K210_FIND_RED_FOLLOW);//读取色块部分是否为黄色
  if(rt){ //如果读取到黄色物体
    // search_for_something = true;
    Serial.println("I find yellow color");
    // put_and_down_something();
    // delay(2000);
    wk->clear_rec();
  }
}