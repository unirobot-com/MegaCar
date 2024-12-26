/**
 * @file WonderK210.cpp
 * @author Wu TongXing
 * @brief Arduino与K210视觉模块通讯
 * @version 0.1
 * @date 2023-08-30
 */

#include "WonderK210.h"

// SoftwareSerial k210_Serial(K210_rxPin, K210_txPin);  //实例化软串口

/* CRC8 字节表 */
static const uint8_t crc8_table[] = {
    0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
    157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
    35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
    190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
    70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
    219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
    101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
    248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
    140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
    17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
    175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
    50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
    202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
    87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
    233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};

/* CRC校验 */
static uint16_t checksum_crc8(const uint8_t *buf, uint16_t len)
{
    uint8_t check = 0;
    while (len--) {
        check = crc8_table[check ^ (*buf++)];
    }
    return ((uint16_t) check) & 0x00FF;
}

WonderK210::WonderK210(void)
{
  
}

void WonderK210::serial_begin(void)
{
  k210_Serial = &Serial2;
  k210_Serial->begin(115200);
  pk_ctl.index_head = 0;
  pk_ctl.index_tail = 0;
  read_succeed = false;
  Serial.println("WonderK210_serial_begin");
}

//更新解析函数
void WonderK210::update_data(void)
{
  //读取数据
  uint32_t len = k210_Serial->available();
  while(len--)
  {
    int rd = k210_Serial->read();
    pk_ctl.data[pk_ctl.index_tail] = (char)rd;
    pk_ctl.index_tail++;
    if(BUFFER_SIZE <= pk_ctl.index_tail)
    {
      pk_ctl.index_tail = 0;
    }
    if(pk_ctl.index_tail == pk_ctl.index_head)
    {
      pk_ctl.index_head++;
      if(BUFFER_SIZE <= pk_ctl.index_head)
      {
        pk_ctl.index_head = 0;
      }
    }else{
      pk_ctl.len++;
    }
  }
  
  uint8_t crc = 0;
  //解析数据
  while(pk_ctl.len > 0)
  {
    switch(pk_ctl.state)
    {
      case K210_CONTROLLER_STATE_STARTBYTE1: /* 处理帧头标记1 */
        pk_ctl.state = K210_CONST_STARTBYTE1 == pk_ctl.data[pk_ctl.index_head] ? K210_CONTROLLER_STATE_STARTBYTE2 : K210_CONTROLLER_STATE_STARTBYTE1;
        break;
      case K210_CONTROLLER_STATE_STARTBYTE2: /* 处理帧头标记2 */
        pk_ctl.state = K210_CONST_STARTBYTE2 == pk_ctl.data[pk_ctl.index_head] ? K210_CONTROLLER_STATE_FUNCTION : K210_CONTROLLER_STATE_STARTBYTE1;
        break;
      case K210_CONTROLLER_STATE_FUNCTION: /* 处理帧功能号 */
        pk_ctl.state = K210_FUNC_NONE > pk_ctl.data[pk_ctl.index_head] ? K210_CONTROLLER_STATE_LENGTH : K210_CONTROLLER_STATE_STARTBYTE1;
        if(K210_CONTROLLER_STATE_LENGTH == pk_ctl.state) {
            pk_ctl.frame.function = pk_ctl.data[pk_ctl.index_head];
        }
        break;
      case K210_CONTROLLER_STATE_LENGTH: /* 处理帧数据长度 */
        if(pk_ctl.data[pk_ctl.index_head] >= DATA_SIZE) //若（包含具体信息）信息数据长度>150,则有问题
        {
          pk_ctl.state = K210_CONTROLLER_STATE_STARTBYTE1;
          continue;
        }else{
          pk_ctl.frame.data_length = pk_ctl.data[pk_ctl.index_head];
          pk_ctl.state = (0 == pk_ctl.frame.data_length) ? K210_CONTROLLER_STATE_CHECKSUM : K210_CONTROLLER_STATE_DATA;
          pk_ctl.data_index = 0;
          break;
        }
      case K210_CONTROLLER_STATE_DATA: /* 处理帧数据 */
        pk_ctl.frame.data[pk_ctl.data_index] = pk_ctl.data[pk_ctl.index_head];
        ++pk_ctl.data_index;
        if(pk_ctl.data_index >= pk_ctl.frame.data_length) {
            pk_ctl.state = K210_CONTROLLER_STATE_CHECKSUM;
            pk_ctl.frame.data[pk_ctl.data_index] = '\0';
        }
        break;
      case K210_CONTROLLER_STATE_CHECKSUM: /* 处理校验值 */
        pk_ctl.frame.checksum = pk_ctl.data[pk_ctl.index_head];
        crc = checksum_crc8((uint8_t*)&pk_ctl.frame.function, pk_ctl.frame.data_length + 2);
        if(crc == pk_ctl.frame.checksum) { /* 校验失败, 跳过执行 */
            pk_result.start_byte1 = pk_ctl.frame.start_byte1;
            pk_result.start_byte2 = pk_ctl.frame.start_byte2;
            pk_result.function = pk_ctl.frame.function;
            pk_result.data_length = pk_ctl.frame.data_length;
            memcpy(pk_result.data , pk_ctl.frame.data , DATA_SIZE);
            pk_result.checksum = pk_ctl.frame.checksum;
            read_succeed = true; //读取成功标志位
        }
        memset(&pk_ctl.frame, 0, sizeof(struct k210_PacketRawFrame)); //清除
        pk_ctl.state = K210_CONTROLLER_STATE_STARTBYTE1;
        break;
      default:
        pk_ctl.state = K210_CONTROLLER_STATE_STARTBYTE1;
        break;
    }

    if(pk_ctl.index_head != pk_ctl.index_tail)
        pk_ctl.index_head++;
    if(BUFFER_SIZE <= pk_ctl.index_head)
        pk_ctl.index_head = 0;
    
    pk_ctl.len--;
  }
}

void WonderK210::clear_rec(void)
{
  //清空缓冲区
  uint32_t len = k210_Serial->available();
  while(len--)  //默认缓冲区始终有空间
  {
    k210_Serial->read();
  }
}

//只有方框数据
bool WonderK210::recive_box(Find_Box_st* rec , enum k210_PACKET_FUNCTION func)
{
  if(false == read_succeed) //未解析完成
    return false;
      
  if(func == pk_result.function)
  {
    read_succeed = false;
    
    Find_Box_st *temp = (Find_Box_st*)pk_result.data;
    rec->x = temp->x;
    rec->y = temp->y;
    rec->w = temp->w;
    rec->h = temp->h;
    return true;
  }else{ //不是对应的功能号
    return false;
  }
}

//有方框数据和消息数据
bool WonderK210::recive_box_msg(Find_Box_Msg_st* rec , enum k210_PACKET_FUNCTION func)
{
  if(false == read_succeed) //未解析完成
      return false;
      
  if(func == pk_result.function)
  {
    read_succeed = false;
    
    Find_Box_Msg_st *temp = (Find_Box_Msg_st*)pk_result.data;
    rec->x = temp->x;
    rec->y = temp->y;
    rec->w = temp->w;
    rec->h = temp->h;
    strcpy(rec->msg , temp->msg);
    return true;
  }else{ //不是对应的功能号
    return false;
  }
}

//只有消息数据
bool WonderK210::recive_msg(Find_Msg_st* rec , enum k210_PACKET_FUNCTION func)
{
  if(false == read_succeed) //未解析完成
      return false;
      
  if(func == pk_result.function)
  {
    read_succeed = false;
    
    Find_Msg_st *temp = (Find_Msg_st*)pk_result.data;
    strcpy(rec->msg , temp->msg);
    return true;
  }else{ //不是对应的功能号
    return false;
  }
}
