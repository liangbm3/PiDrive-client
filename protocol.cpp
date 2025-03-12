#include "protocol.h"
#include "string.h"

PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen=sizeof(PDU)+uiMsgLen;
    PDU *pdu =(PDU*)malloc(uiPDULen);
    if(NULL==pdu)
    {
        std::exit(EXIT_FAILURE);
    }
    memset(pdu,0,uiPDULen);//数据初始化为0
    pdu->uiPDULen=uiPDULen;//指定总长度
    pdu->uiMsgLen=uiMsgLen;//指定消息长度

    return pdu;
}
