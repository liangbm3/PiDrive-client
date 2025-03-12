#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>

#define REGIST_OK "regist ok"
#define REGIST_FAILED "regist fail"
#define LOGIN_OK "login ok"
#define LOGIN_FAILED "login fail"

using uint =unsigned int;

enum MSG_TYPE
{
    MSG_TYPE_MIN=0,
    MSG_TYPE_REGIST_REQUEST,//注册请求
    MSG_TYPE_REGIST_RESPONE,//注册回复
    MSG_TYPE_LOGIN_REQUEST,//登录请求
    MSG_TYPE_LOGIN_RESPONE,//登录响应
    MSG_TYPE_MAX=0x00ffffff
};

//订阅传输协议弹性结构体
struct PDU
{
    uint uiPDULen; //总的消息大小
    uint uiMsgType; //消息类型
    char caFileData[64];//文件的其他信息，例如文件名等
    uint uiMsgLen;//实际消息长度
    int caMsg[];//实际消息

};

//通过消息长度来计算结构体大小
PDU *mkPDU(uint uiMsgLen);

#endif // PROTOCOL_H
