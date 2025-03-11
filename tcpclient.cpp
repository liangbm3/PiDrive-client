#include "tcpclient.h"
#include "./ui_tcpclient.h"
#include <QJsonArray>//用于处理json数组
#include <QJsonDocument>//用于解析json文档
#include <QJsonObject>//用于处理json对象
#include <QDebug>//用于调试
#include <QFile>//用于文件操作
#include <QMessageBox>
#include "protocol.h"
#include "string.h"
TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    loadConfig();
    connect(&m_tcpSocket,SIGNAL(connected()),this,SLOT(showConnect()));
    //连接服务器
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);
    connect(&m_tcpSocket,SIGNAL(readyRead()),this,SLOT(receiveMsg()));
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::loadConfig()
{
    //指定json资源文件路径
    QFile file(":/config.json");
    //尝试以只读方式打开
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning()<<"无法打开配置文件!";
        return;
    }

    //读取整个json文件内容
    QByteArray json_data =file.readAll();
    file.close();//关闭文件

    //解析json数据
    QJsonParseError json_error;//用于存储解析过程中的错误信息
    QJsonDocument json_doc=QJsonDocument::fromJson(json_data,&json_error);

    //检查json是否成功解析
    if (json_error.error!=QJsonParseError::NoError)
    {
        qWarning()<<"配置文件解析错误"<<json_error.errorString();
        return;
    }

    //确保json根元素是对象
    if(!json_doc.isObject())
    {
        qWarning()<<"配置文件格式错误！";
        return;
    }

    //获取json根对象
    QJsonObject json_object=json_doc.object();

    //读取ip字段，字符串类型
    if(json_object.contains("ip") && json_object["ip"].isString())
    {
        m_strIP=json_object["ip"].toString();
        qDebug()<<"ip:"<<m_strIP.toStdString();
    }

    //读取port字段，数字类型
    if(json_object.contains("port") && json_object["port"].isDouble())
    {
        m_usPort=json_object["port"].toInt();
        qDebug()<<"port:"<<m_usPort;
    }


}

void TcpClient::receiveMsg()
{
    qDebug()<<m_tcpSocket.bytesAvailable();
    uint uiPDUlen=0;
    m_tcpSocket.read((char*)&uiPDUlen,sizeof(uint));//读取总数据大小
    uint uiMsgLen=uiPDUlen-sizeof(PDU);
    PDU *pdu=mkPDU(uiMsgLen);
    m_tcpSocket.read((char*)pdu+sizeof(uint),uiPDUlen-sizeof(uint));

    //根据不同的消息类型执行不同的操作
    switch (pdu->uiMsgType)
    {
    case MSG_TYPE_REGIST_RESPONE://注册请求响应
        if(0==strcmp(pdu->caFileData,REGIST_OK))
        {
            QMessageBox::information(this,"注册",REGIST_OK);
        }
        else if(0==strcmp(pdu->caFileData,REGIST_FAILED))
        {
            QMessageBox::information(this,"注册",REGIST_FAILED);
        }
        break;
    case MSG_TYPE_LOGIN_RESPONE:
        if(0==strcmp(pdu->caFileData,LOGIN_OK))
        {
            QMessageBox::information(this,"登录",LOGIN_OK);
        }
        else if(0==strcmp(pdu->caFileData,LOGIN_FAILED))
        {
            QMessageBox::information(this,"登录",LOGIN_FAILED);
        }
    default:
        break;
    }
    free(pdu);
    pdu=NULL;
}

void TcpClient::showConnect()
{
    QMessageBox::information(this,"连接服务器","连接服务器成功");
}

// void TcpClient::on_send_clicked()
// {
//     QString strMsg =ui->lineEdit->text();
//     if(!strMsg.isEmpty())
//     {
//         PDU *pdu =mkPDU(strMsg.size());//申请空间
//         pdu->uiMsgType=888;
//         memcpy(pdu->caFileData,strMsg.toStdString().c_str(),strMsg.size());//拷贝消息内容
//         m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
//         free(pdu);
//         pdu=NULL;
//     }
//     else
//     {
//         QMessageBox::warning(this,"信息发送","信息发送不能为空");
//     }
// }


void TcpClient::on_cancel_Button_clicked()
{

}


void TcpClient::on_regist_Button_clicked()
{
    QString strName=ui->name_lineEdit->text();
    QString strPwd=ui->pwd_lineEdit->text();
    if(!strName.isEmpty()&&!strPwd.isEmpty())
    {
        PDU *pdu =mkPDU(0);
        pdu->uiMsgType=MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caFileData,strName.toStdString().c_str(),32);
        strncpy(pdu->caFileData+32,strPwd.toStdString().c_str(),32);
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
    else
    {
        QMessageBox::critical(this,"注册","注册失败：用户名或密码不能为空");
    }
}


void TcpClient::on_login_Button_clicked()
{
    QString strName =ui->name_lineEdit->text();
    QString strPwd =ui->pwd_lineEdit->text();
    if(!strName.isEmpty()&&!strPwd.isEmpty())
    {
        PDU *pdu =mkPDU(0);
        pdu->uiMsgType=MSG_TYPE_LOGIN_REQUEST;
        memcpy(pdu->caFileData,strName.toStdString().c_str(),32);
        memcpy(pdu->caFileData+32,strPwd.toStdString().c_str(),32);
        qDebug()<<pdu->uiMsgType<<" "<<pdu->caFileData<<" "<<pdu->caFileData+32;
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);//发送消息

        free(pdu);
        pdu=NULL;
    }
    else
    {
        QMessageBox::critical(this,"登录","登录失败：用户名或密码为空！");
    }
}

