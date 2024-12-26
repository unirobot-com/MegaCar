/*
 * @file WonderK210.h
 * @author Wu TongXing
 * @brief Arduino与K210视觉模块通讯
 * @version 0.1
 * @date 2023-08-30
 */

#ifndef WONDERK210_H
#define WONDERK210_H

#include <Arduino.h>
#include <Wire.h>
// #include "SoftwareSerial.h" //软串口库

#define K210_rxPin 7      //arduino与K210的通信串口
#define K210_txPin 6      

//************* 数据结构定义 begin *************

#define K210_CONST_STARTBYTE1 0xAAu
#define K210_CONST_STARTBYTE2 0x55u
const uint16_t BUFFER_SIZE = 90;
const uint16_t DATA_SIZE = 30; //有效数据不应该超过 30
const uint16_t MSG_SIZE = 20; 

//解析器状态机状态枚举
enum k210_PacketControllerState {
    K210_CONTROLLER_STATE_STARTBYTE1, /**< 正在寻找帧头标记1 */
    K210_CONTROLLER_STATE_STARTBYTE2, /**< 正在寻找帧头标记2 */
    K210_CONTROLLER_STATE_FUNCTION, /**< 正在处理帧功能号 */
    K210_CONTROLLER_STATE_LENGTH, /**< 正在处理帧长度 */
    K210_CONTROLLER_STATE_DATA, /**< 正在处理帧数据 */
    K210_CONTROLLER_STATE_CHECKSUM, /** 正在处理数据校验 */
};

#pragma pack(1)
//命令内容参数
typedef
struct k210_PacketRawFrame {
    uint8_t start_byte1;
    uint8_t start_byte2;
    uint8_t function;   //功能号
    uint8_t data_length;
    uint8_t data[DATA_SIZE];  //数据
    uint8_t checksum;
}K210_packet_st;
#pragma pack(0)

/**
 * @brief 协议解析器
 * @details 协议解析器, 存储了解析器的工作状态、状态机状态、缓冲存储区等
 */
struct k210_PacketController {
    //解析协议的相关变量
    enum k210_PacketControllerState state;        //解析器状态机当前状态
    struct k210_PacketRawFrame frame;             //解析器正在处理的帧
    int data_index;   //解析时需要用到的数据下标

    //缓冲区相关变量
    uint8_t len;
    uint8_t index_head;
    uint8_t index_tail;
    uint8_t data[BUFFER_SIZE];
};

#pragma pack(1)

//方框数据结构体
typedef struct {
  uint16_t  x;
  uint16_t  y;
  uint16_t  w;
  uint16_t  h;
}Find_Box_st;

//有方框数据和消息数据
typedef struct {
  uint16_t  x;
  uint16_t  y;
  uint16_t  w;
  uint16_t  h;
  char msg[MSG_SIZE];
}Find_Box_Msg_st;

//只有消息数据
typedef struct {
  char msg[MSG_SIZE];
}Find_Msg_st;

#pragma pack()

//帧功能号枚举
enum k210_PACKET_FUNCTION {
  K210_FUNC_SYS = 0,
  K210_COLOR_RECOGNITION, //颜色学习识别 1
  K210_FIND_BARCODES,     //条形码识别 2
  K210_FIND_QRCODES,      //二维码识别 3
  K210_FIND_APRILTAGS,    //机器码识别 4
  K210_FIND_FACE_YOLO,    //人脸检测 5
  K210_FIND_FACEFEATURE,  //人脸特征检测 6
  K210_FIND_FACEMASK,     //口罩检测 7
  K210_FIND_OBJECT,       //物体检测 8
  K210_FIND_SELF_LEARNING,//自主学习分类 9
  K210_FIND_DIGITAL,      //数字识别 10
  K210_FIND_FACE_RECOGNITION, //人脸识别 11

  K210_FIND_RED_FOLLOW, //红色追踪 0x0C
  K210_FIND_SIGNPOST_FOLLOW, //路标追踪 0x0D
  K210_FIND_DIGITAL_CARD, //数字卡片识别做动作 0x0E
  K210_GARBAGE_SORTING, //垃圾分类 0x0F

  K210_COLOR_SORTING, //多种颜色识别 0x10

  K210_FUNC_NONE //结束
};

//************* 数据结构定义 end ***************

class WonderK210 {
  public:
    //构造函数
    WonderK210(void);

    //串口开启函数
    void serial_begin(void);

    //更新解析函数
    void update_data(void);

    void clear_rec(void);
    
    //用户使用函数
    bool recive_box(Find_Box_st* rec , enum k210_PACKET_FUNCTION func);
    bool recive_box_msg(Find_Box_Msg_st* rec , enum k210_PACKET_FUNCTION func);
    bool recive_msg(Find_Msg_st* rec , enum k210_PACKET_FUNCTION func);

  private:
    // 读取成功标志位
    bool read_succeed;
    // 接收解析控制器  
    struct k210_PacketController pk_ctl;
    // 解析结果储存变量
    K210_packet_st pk_result;

    HardwareSerial* k210_Serial;
};


#endif //WONDERK210_H
