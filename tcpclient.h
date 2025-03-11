#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QFile>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui {
class TcpClient;
}
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void loadConfig();

public slots:
    void showConnect();
    void receiveMsg();
private slots:
    // void on_send_clicked();

    void on_cancel_Button_clicked();

    void on_regist_Button_clicked();

    void on_login_Button_clicked();

private:
    Ui::TcpClient *ui;
    QString m_strIP;
    quint16 m_usPort;
    QTcpSocket m_tcpSocket;
};
#endif // TCPCLIENT_H
